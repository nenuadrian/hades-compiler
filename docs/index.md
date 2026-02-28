# Hades Compiler

The Hades Compiler translates the custom **Hades** language into JVM bytecode.

This documentation site is generated with **MkDocs Material** and includes auto-generated reference pages from:

- `src/main/antlr/Hades.g4`
- `src/main/java/com/example/*.java`

## What Is Included

- Build, run, and test instructions.
- Compiler architecture overview.
- Full ANTLR grammar and parser-token summaries.
- Java source documentation with class/method summaries.
- Automatically generated Mermaid diagrams for grammar and code relationships.

## Regenerating Reference Docs

Run the generator whenever grammar or Java sources change:

```bash
python3 scripts/generate_docs.py
```

Then build locally:

```bash
pip install -r docs/requirements.txt
mkdocs serve
```
