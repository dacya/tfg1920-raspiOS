
run: build
	qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel myos.elf

build:
	arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -c boot.S -o boot.o
	arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -std=gnu99 -c kernel.c -o kernel.o -O2 -Wall -Wextra
	arm-none-eabi-gcc -T linker.ld -o myos.elf -ffreestanding -O2 -nostdlib boot.o kernel.o
	arm-none-eabi-objcopy myos.elf -O binary myos.bin

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
