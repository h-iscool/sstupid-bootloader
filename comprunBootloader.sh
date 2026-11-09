nodependencies="-ffreestanding -fno-pie -fno-pic -fno-stack-protector -fno-builtin" 

compile(){

	rm -rf ./build/
	mkdir -p ./build
	if [[ ! $? ]]; then
		exit
	fi

	nasm -f bin -o "./build/stage1.bin" "stage1.asm"

	#elf32 allows to be linked together
	nasm -f elf32 -o "./build/stage2.o" "stage2.asm"
	nasm -f elf32 -o "./build/kernel.o" "./kernel/kernel.s"
	gcc -m32 -c ./kernel/kernel_init.c -o ./build/kernel_init.o $nodependencies -O2 -nostdlib
	gcc -m32 -c ./kernel/console.c -o ./build/console.o $nodependencies -O2 -nostdlib
	gcc -m32 -c ./drivers/screen.c -o ./build/screen.o $nodependencies -O2 -nostdlib
	gcc -m32 -c ./drivers/vga.c -o ./build/vga.o $nodependencies -O2 -nostdlib
	gcc -m32 -c ./drivers/vbe.c -o ./build/vbe.o $nodependencies -O2 -nostdlib
	gcc -m32 -c ./drivers/keyboard.c -o ./build/keyboard.o $nodependencies -O2 -nostdlib
	gcc -m32 -c ./drivers/crypto.c -o ./build/crypto.o $nodependencies -O2 -nostdlib
	gcc -m32 -c ./drivers/string.c -o ./build/string.o $nodependencies -O2 -nostdlib
	gcc -m32 -c ./kernel/syscall_wrappers.c -o ./build/syscall_wrappers.o $nodependencies -O2 -nostdlib


	ld -m elf_i386 --no-pie -T linker.ld \
	./build/*.o \
	-o ./build/linked.bin --oformat binary


	if [[ ! -f "./build/stage1.bin" ]]; then
		echo "Stage1 didnt compile"
		exit
	fi
	if [[ ! $? ]]; then
		echo "Linker didn't link. Maybe something didn't compile?"
		exit
	fi

	cat ./build/stage1.bin ./build/linked.bin > ./build/image.bin

	# 20 sectors padding:
	truncate -s 10240 ./build/image.bin

}

run() {
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
	  -vga std -display gtk \
	  -drive file="./build/image.bin",if=ide,format=raw \
	  -nic user

}


if [[ $0 != "*-s*" ]]; then
	compile
fi


run $0
