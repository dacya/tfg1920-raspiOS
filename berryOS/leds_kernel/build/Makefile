# Don't use normal gcc, use the arm cross compiler
CC = ./gcc-arm-none-eabi-8-2019-q3-update/bin/arm-none-eabi-gcc
OBJCOPY = ./gcc-arm-none-eabi-8-2019-q3-update/bin/arm-none-eabi-objcopy
# Set any constants based on the raspberry pi model.  Version 1 has some differences to 2 and 3

CPU = cortex-a7

CFLAGS= -mcpu=$(CPU) -fpic -ffreestanding 
CSRCFLAGS= -O2 -Wall -Wextra
LFLAGS= -ffreestanding -O2 -nostdlib

# Location of the files
KER_SRC = ../src/kernel
KER_HEAD = ../include
COMMON_SRC = ../src/common
OBJ_DIR = objects
KERSOURCES = $(wildcard $(KER_SRC)/*.c)
COMMONSOURCES = $(wildcard $(COMMON_SRC)/*.c)
ASMSOURCES = $(wildcard $(KER_SRC)/*.S)
OBJECTS = $(patsubst $(KER_SRC)/%.c, $(OBJ_DIR)/%.o, $(KERSOURCES))
OBJECTS += $(patsubst $(COMMON_SRC)/%.c, $(OBJ_DIR)/%.o, $(COMMONSOURCES))
OBJECTS += $(patsubst $(KER_SRC)/%.S, $(OBJ_DIR)/%.o, $(ASMSOURCES))
HEADERS = $(wildcard $(KER_HEAD)/*.h)

IMG_NAME=kernel.img

build_hard: 	$(OBJECTS) $(HEADERS) 
	echo $(OBJECTS)
	$(CC) -T linker.ld -o kernel.elf $(LFLAGS) $(OBJECTS) 
	$(OBJCOPY) kernel.elf -O binary kernel.img

build_sim:	$(OBJECTS) $(HEADERS)
	echo $(OBJECTS)
	$(CC) -T linker.ld -o $(IMG_NAME) $(LFLAGS) $(OBJECTS)

$(OBJ_DIR)/%.o: $(KER_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -I$(KER_HEAD) -c $< -o $@ $(CSRCFLAGS)

$(OBJ_DIR)/%.o: $(KER_SRC)/%.S
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -c $< -o $@

$(OBJ_DIR)/%.o: $(COMMON_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -I$(KER_HEAD) -c $< -o $@ $(CSRCFLAGS)

clean:	
	if test -f kernel.elf; then \
		rm kernel.elf; \
	fi	
	rm -rf $(OBJ_DIR)	
	rm $(IMG_NAME)

run:	build
	qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel kernel.img

