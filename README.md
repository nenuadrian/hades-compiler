# Hades Compiler

# Language



# MacOS

```
ld -macosx_version_min 14.1 -o output output.o
ld -o output output.o -lSystem -syslibroot `xcrun --sdk macosx --show-sdk-path` -e _start -arch x86_64 -platform_version macos 10.15 14.1
```

# Linux

# Winws