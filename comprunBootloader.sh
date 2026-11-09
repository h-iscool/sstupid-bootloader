compileandrun(){

	rm -rf ./build/
	mkdir -p ./build
	if [[ ! $? ]]; then
		exit
	fi

	nasm -f bin -o "./build/stage1.bin" "stage1.asm"

	#elf32 allows to be linked together
	nasm -f elf32 -o "./build/stage2.o" "stage2.asm"
	nasm -f elf32 -o "./build/kernel.o" "./kernel/kernel.s"
	gcc -m32 -c ./kernel/kernel_init.c -o ./build/kernel_init.o -ffreestanding -fno-pie -fno-pic -O2 -nostdlib
	gcc -m32 -c ./kernel/console.c -o ./build/console.o -ffreestanding -fno-pie -fno-pic -O2 -nostdlib
	gcc -m32 -c ./drivers/vga.c -o ./build/vga.o -ffreestanding -fno-pie -fno-pic -O2 -nostdlib
	gcc -m32 -c ./drivers/keyboard.c -o ./build/keyboard.o -ffreestanding -fno-pie -fno-pic -O2 -nostdlib


	ld -m elf_i386 -T linker.ld \
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

compileandrun
