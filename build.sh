#!/usr/bin/env bash
set -euo pipefail

OS="$(uname -s)"
ARCH="$(uname -m)"
QEMU=qemu-system-i386

CFLAGS=(
  -ffreestanding
  -fno-pie
  -fno-pic
  -fno-stack-protector
  -fno-builtin
  -O2
  -nostdlib
)

case "$OS" in
  Linux)
    CC=(gcc -m32)
    LD=(ld -m elf_i386)

    if [[ -w /dev/kvm ]]; then
      QEMU_ARGS=(-accel kvm -cpu host)
    else
      QEMU_ARGS=(-accel tcg,thread=multi -cpu qemu32)
    fi
    ;;

  Darwin)
    # Homebrew's i686-elf toolchain emits ELF, unlike Apple's clang.
    CC=(i686-elf-gcc)
    LD=(i686-elf-ld -m elf_i386)

    if [[ "$ARCH" == "x86_64" ]]; then
      QEMU_ARGS=(-accel hvf -cpu qemu32)
    else
      QEMU_ARGS=(-accel tcg,thread=multi -cpu qemu32)
    fi
    ;;

  *)
    echo "Unsupported host OS: $OS" >&2
    exit 1
    ;;
esac

for tool in nasm "${CC[0]}" "${LD[0]}" "$QEMU"; do
  command -v "$tool" >/dev/null 2>&1 || {
    echo "Required tool not found: $tool" >&2
    exit 1
  }
done

compileandrun() {
  rm -rf ./build
  mkdir -p ./build

  nasm -f bin   -o ./build/stage1.bin stage1.asm
  nasm -f elf32 -o ./build/stage2.o stage2.asm
  nasm -f elf32 -o ./build/kernel.o ./kernel/kernel.s

  "${CC[@]}" "${CFLAGS[@]}" -c ./kernel/console.c          -o ./build/console.o
  "${CC[@]}" "${CFLAGS[@]}" -c ./kernel/kernel_init.c          -o ./build/kernel_init.o
  "${CC[@]}" "${CFLAGS[@]}" -c ./drivers/vga.c             -o ./build/vga.o
  "${CC[@]}" "${CFLAGS[@]}" -c ./drivers/vbe.c             -o ./build/vbe.o
  "${CC[@]}" "${CFLAGS[@]}" -c ./drivers/keyboard.c        -o ./build/keyboard.o
  "${CC[@]}" "${CFLAGS[@]}" -c ./drivers/crypto.c          -o ./build/crypto.o
  "${CC[@]}" "${CFLAGS[@]}" -c ./drivers/string.c          -o ./build/string.o
  "${CC[@]}" "${CFLAGS[@]}" -c ./kernel/syscall_wrappers.c -o ./build/syscall_wrappers.o

  "${LD[@]}" -T linker.ld \
    ./build/*.o \
    -o ./build/linked.bin --oformat binary

  cat ./build/stage1.bin ./build/linked.bin > ./build/image.bin
  truncate -s 10240 ./build/image.bin

  "$QEMU" \
    "${QEMU_ARGS[@]}" \
    -smp 2 \
    -m 2G \
    -vga std \
    -drive file=./build/image.bin,if=ide,format=raw \
    -nic user
}

compileandrun

