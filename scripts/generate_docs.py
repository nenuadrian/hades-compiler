#!/usr/bin/env python3
"""Generate MkDocs reference pages from Java and ANTLR sources."""

from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable

ROOT = Path(__file__).resolve().parents[1]
ANTLR_FILE = ROOT / "src/main/antlr/Hades.g4"
JAVA_DIR = ROOT / "src/main/java"
OUTPUT_DIR = ROOT / "docs/reference"


@dataclass
class AntlrRule:
    name: str
    body: str

    @property
    def is_lexer(self) -> bool:
        return bool(self.name and self.name[0].isupper())


@dataclass
class JavaMethod:
    visibility: str
    return_type: str
    name: str
    params: str


@dataclass
class JavaClassDoc:
    package: str
    class_name: str
    source_path: Path
    source: str
    methods: list[JavaMethod]
    references: set[str]


def ensure_paths() -> None:
    if not ANTLR_FILE.exists():
        raise FileNotFoundError(f"ANTLR file not found: {ANTLR_FILE}")
    if not JAVA_DIR.exists():
        raise FileNotFoundError(f"Java source directory not found: {JAVA_DIR}")
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)


def parse_antlr_rules(grammar: str) -> list[AntlrRule]:
    def split_unquoted_semicolon(text: str) -> tuple[str, str] | None:
        in_single_quote = False
        escaped = False

        for index, char in enumerate(text):
            if char == "\\" and in_single_quote and not escaped:
                escaped = True
                continue

            if char == "'" and not escaped:
                in_single_quote = not in_single_quote
            elif char == ";" and not in_single_quote:
                return text[:index], text[index + 1 :]

            escaped = False

        return None

    rules: list[AntlrRule] = []
    current_name: str | None = None
    current_body: list[str] = []

    for raw_line in grammar.splitlines():
        line = raw_line.split("//", 1)[0].strip()
        if not line:
            continue

        if current_name is None:
            match = re.match(r"^([A-Za-z_][A-Za-z0-9_]*)\s*:\s*(.*)$", line)
            if not match:
                continue
            current_name = match.group(1)
            remainder = match.group(2).strip()
        else:
            remainder = line

        split = split_unquoted_semicolon(remainder)
        if split is None:
            current_body.append(remainder)
            continue

        body_prefix, _ = split
        current_body.append(body_prefix.strip())
        normalized = re.sub(r"\s+", " ", " ".join(current_body)).strip()
        if normalized and current_name is not None:
            rules.append(AntlrRule(name=current_name, body=normalized))

        current_name = None
        current_body = []

    return rules


def parser_dependency_edges(parser_rules: Iterable[AntlrRule]) -> list[tuple[str, str]]:
    rule_names = {rule.name for rule in parser_rules}
    edges: set[tuple[str, str]] = set()

    for rule in parser_rules:
        for ref in set(re.findall(r"\b([a-z][A-Za-z0-9_]*)\b", rule.body)):
            if ref in rule_names and ref != rule.name:
                edges.add((rule.name, ref))

    return sorted(edges)


def parse_java_file(path: Path, all_class_names: set[str]) -> JavaClassDoc:
    source = path.read_text(encoding="utf-8")

    package_match = re.search(r"(?m)^package\s+([\w.]+);", source)
    package = package_match.group(1) if package_match else ""

    class_match = re.search(r"\bclass\s+([A-Za-z_][A-Za-z0-9_]*)", source)
    class_name = class_match.group(1) if class_match else path.stem

    method_pattern = re.compile(
        r"(?m)^\s*(public|protected|private)\s+"
        r"(?:static\s+)?(?:final\s+)?([A-Za-z_][\w<>\[\],.?\s]*)\s+"
        r"([A-Za-z_][A-Za-z0-9_]*)\s*\(([^)]*)\)"
    )

    methods: list[JavaMethod] = []
    for visibility, return_type, name, params in method_pattern.findall(source):
        methods.append(
            JavaMethod(
                visibility=visibility,
                return_type=re.sub(r"\s+", " ", return_type).strip(),
                name=name,
                params=re.sub(r"\s+", " ", params).strip(),
            )
        )

    references: set[str] = set()
    for candidate in all_class_names:
        if candidate == class_name:
            continue
        if re.search(rf"\b{re.escape(candidate)}\b", source):
            references.add(candidate)

    return JavaClassDoc(
        package=package,
        class_name=class_name,
        source_path=path,
        source=source.rstrip(),
        methods=methods,
        references=references,
    )


def mermaid_graph(edges: list[tuple[str, str]], direction: str = "LR") -> str:
    lines = [f"flowchart {direction}"]
    if not edges:
        lines.append('  empty["No dependencies detected"]')
        return "\n".join(lines)

    seen_nodes: set[str] = set()
    for source, target in edges:
        seen_nodes.add(source)
        seen_nodes.add(target)

    for node in sorted(seen_nodes):
        lines.append(f'  {node}["{node}"]')

    for source, target in edges:
        lines.append(f"  {source} --> {target}")

    return "\n".join(lines)


def write_antlr_page(grammar_text: str, rules: list[AntlrRule]) -> None:
    parser_rules = [rule for rule in rules if not rule.is_lexer]
    lexer_rules = [rule for rule in rules if rule.is_lexer]
    edges = parser_dependency_edges(parser_rules)

    parser_table = "\n".join(
        f"| `{rule.name}` | `{rule.body}` |" for rule in parser_rules
    )
    lexer_table = "\n".join(
        f"| `{rule.name}` | `{rule.body}` |" for rule in lexer_rules
    )

    page = f"""# ANTLR Grammar Reference

_This page is auto-generated from `src/main/antlr/Hades.g4` by `scripts/generate_docs.py`._

## Parser Rule Dependency Graph

```mermaid
{mermaid_graph(edges)}
```

## Parser Rules

| Rule | Definition |
| --- | --- |
{parser_table if parser_table else '| _none_ | _none_ |'}

## Lexer Tokens

| Token | Definition |
| --- | --- |
{lexer_table if lexer_table else '| _none_ | _none_ |'}

## Full Grammar

```antlr
{grammar_text.rstrip()}
```
"""

    (OUTPUT_DIR / "antlr.md").write_text(page, encoding="utf-8")


def write_code_page(class_docs: list[JavaClassDoc]) -> None:
    classes_table = "\n".join(
        f"| `{doc.class_name}` | `{doc.package}` | `{doc.source_path.relative_to(ROOT)}` |"
        for doc in class_docs
    )

    edges = sorted(
        {
            (doc.class_name, ref)
            for doc in class_docs
            for ref in doc.references
            if any(other.class_name == ref for other in class_docs)
        }
    )

    sections: list[str] = []
    for doc in class_docs:
        methods_rows = "\n".join(
            f"| `{method.visibility}` | `{method.return_type}` | `{method.name}` | `{method.params or '()'}` |"
            for method in doc.methods
        )

        sections.append(
            f"""### `{doc.class_name}`

**Source:** `{doc.source_path.relative_to(ROOT)}`

| Visibility | Return Type | Method | Parameters |
| --- | --- | --- | --- |
{methods_rows if methods_rows else '| _n/a_ | _n/a_ | _n/a_ | _n/a_ |'}

```java
{doc.source}
```
"""
        )

    sections_content = "\n".join(sections) if sections else "No Java classes were found."

    page = f"""# Java Code Reference

_This page is auto-generated from `src/main/java` by `scripts/generate_docs.py`._

## Classes

| Class | Package | Source |
| --- | --- | --- |
{classes_table if classes_table else '| _none_ | _none_ | _none_ |'}

## Class Dependency Graph

```mermaid
{mermaid_graph(edges, direction='TD')}
```

## Source Details

{sections_content}
"""

    (OUTPUT_DIR / "code.md").write_text(page, encoding="utf-8")


def write_diagrams_page(parser_edges: list[tuple[str, str]], code_edges: list[tuple[str, str]]) -> None:
    # Keep this high-level and source-derived to avoid manual drift.
    page = f"""# Generated Diagrams

_This page is auto-generated from grammar and Java source relationships._

## Grammar Relationships

```mermaid
{mermaid_graph(parser_edges, direction='LR')}
```

## Java Class Relationships

```mermaid
{mermaid_graph(code_edges, direction='TD')}
```

## Build Pipeline

```mermaid
flowchart LR
  source[".hades file"] --> parser["ANTLR lexer/parser"]
  parser --> tree["ParseTree"]
  tree --> visitor["HadesBytecodeGenerator"]
  visitor --> bytecode["JVM bytecode"]
  bytecode --> classfile[".class output"]
```
"""

    (OUTPUT_DIR / "diagrams.md").write_text(page, encoding="utf-8")


def main() -> None:
    ensure_paths()

    grammar_text = ANTLR_FILE.read_text(encoding="utf-8")
    antlr_rules = parse_antlr_rules(grammar_text)

    java_files = sorted(JAVA_DIR.rglob("*.java"))
    all_class_names = {path.stem for path in java_files}

    class_docs = [parse_java_file(path, all_class_names) for path in java_files]
    class_docs.sort(key=lambda doc: doc.class_name)

    parser_rules = [rule for rule in antlr_rules if not rule.is_lexer]
    parser_edges = parser_dependency_edges(parser_rules)

    code_edges = sorted(
        {
            (doc.class_name, ref)
            for doc in class_docs
            for ref in doc.references
            if any(other.class_name == ref for other in class_docs)
        }
    )

    write_antlr_page(grammar_text, antlr_rules)
    write_code_page(class_docs)
    write_diagrams_page(parser_edges, code_edges)

    print("Generated docs/reference/antlr.md, code.md, and diagrams.md")


if __name__ == "__main__":
    main()
