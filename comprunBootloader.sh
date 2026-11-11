nodependencies="-ffreestanding -fno-pie -fno-pic -fno-stack-protector -fno-builtin" 
cppdependencies="$nodependencies -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics"

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
	nasm -f elf32 -o "./build/syscalls.o" "./kernel/syscalls.s"
	# gcc -m32 -c ./kernel/kernel_init.c -o ./build/kernel_init.o $nodependencies -O2 -nostdlib
	# gcc -m32 -c ./kernel/console.c -o ./build/console.o $nodependencies -O2 -nostdlib
	# gcc -m32 -c ./drivers/vga.c -o ./build/vga.o $nodependencies -O2 -nostdlib
	# gcc -m32 -c ./drivers/vbe.c -o ./build/vbe.o $nodependencies -O2 -nostdlib
	# gcc -m32 -c ./drivers/keyboard.c -o ./build/keyboard.o $nodependencies -O2 -nostdlib
	# gcc -m32 -c ./drivers/crypto.c -o ./build/crypto.o $nodependencies -O2 -nostdlib
	# gcc -m32 -c ./drivers/string.c -o ./build/string.o $nodependencies -O2 -nostdlib
	# gcc -m32 -c ./kernel/syscall_wrappers.c -o ./build/syscall_wrappers.o $nodependencies -O2 -nostdlib

	for file in ./feature_testing_and_debug/*.asm; do #TO ADD A DIR TO COMPULOE STUFF, ADD HERE AND IN THE CPP ONE
		if [ -f "$file" ]; then
			name=$(basename "$file" .asm) # cool linux command htat gives the name
			nasm -f elf32 -o "./build/$name.o" $file
		fi
	done

	for file in ./kernel/*.c ./drivers/*.c ./feature_testing_and_debug/*.c ; do #TO ADD A DIR TO COMPULOE STUFF, ADD HERE AND IN THE CPP ONE
		if [ -f "$file" ]; then
			name=$(basename "$file" .c) # cool linux command htat gives the name
			gcc -m32 -c "$file" -o ./build/"$name".o $nodependencies -O2 -nostdlib
		fi
	done

	for file in ./kernel/*.cpp ./drivers/*.cpp ./feature_testing_and_debug/*.cpp ; do #TO ADD A DIR TO COMPULOE STUFF, ADD HERE AND IN THE CPP ONE
		if [ -f "$file" ]; then
			name=$(basename "$file" .cpp) # cool linux command htat gives the name
			g++ -m32 -c "$file" -o ./build/"$name".o $cppdependencies -O2 -nostdlib
		fi
	done


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

	# 64 sectors padding:
	truncate -s 32768 ./build/image.bin

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
