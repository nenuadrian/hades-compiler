# Getting Started

## Prerequisites

- Java 11+
- Python 3.10+ (for docs tooling)

## Build

```bash
./gradlew build
```

## Run

```bash
./gradlew run --args="--help"
```

## Test

```bash
./gradlew test
```

## Build Documentation

```bash
pip install -r docs/requirements.txt
python3 scripts/generate_docs.py
mkdocs build --strict
```

## Preview Documentation

```bash
mkdocs serve
```
