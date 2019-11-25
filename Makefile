CC = arm-none-eabi 
C_OPT = -mcpu=cortex-a7 -fpic -ffreestanding
L_OPT = -ffreestanding -O2 -nostdlib
run: build
	qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel myos.elf

build:
	$(CC)-gcc $(C_OPT) -c boot.S -o boot.o
	$(CC)-gcc $(C_OPT) -std=gnu99 -c kernel.c -o kernel.o -O2 -Wall -Wextra
	$(CC)-gcc -T linker.ld -o myos.elf $(L_OPT) boot.o kernel.o
	$(CC)-objcopy myos.elf -O binary myos.bin

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
