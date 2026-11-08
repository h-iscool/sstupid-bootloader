compileandrun(){

mkdir -p ./build

nasm -f bin -o "./build/stage1.bin" "main.asm"
nasm -f elf32 -o "./build/stage2.o" "stage2.asm"
# cat main.bin stage2.bin > out.bin

gcc -m32 -c kernel.c -o ./build/kernel.o -ffreestanding -fno-pie -fno-pic -O2 -nostdlib
gcc -m32 -c vga_funcs.c -o ./build/vga_funcs.o -ffreestanding -fno-pie -fno-pic -O2 -nostdlib

ld -m elf_i386 -T linker.ld ./build/stage2.o ./build/kernel.o ./build/vga_funcs.o -o ./build/linked.bin --oformat binary

cat ./build/stage1.bin ./build/linked.bin > ./build/out.bin

# 20 sectors padding:
truncate -s 10240 ./build/out.bin


local kvmargs=""
if [ -w /dev/kvm ]; then
    kvmargs="-enable-kvm -cpu host"
else
    kvmargs="-accel tcg,thread=multi -cpu qemu64"
fi
qemu-system-x86_64 \
  $kvmargs \
  -smp 2 \
  -m 2G \
  -vga virtio -display gtk \
  -drive file="./build/out.bin",if=ide,format=raw \
  -nic user

}

compileandrun