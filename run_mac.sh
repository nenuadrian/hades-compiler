nasm -f macho64 build/xcode4/bin/x86_64/Debug/output.asm

ld -o build/xcode4/bin/x86_64/Debug/output build/xcode4/bin/x86_64/Debug/output.o -lSystem -syslibroot `xcrun --sdk macosx --show-sdk-path` -e _start -arch x86_64 -platform_version macos 10.15 14.1

build/xcode4/bin/x86_64/Debug/output

echo $?