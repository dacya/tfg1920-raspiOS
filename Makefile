CC = compiler/bin/arm-none-eabi
OBJCOPY = compiler/bin/arm-none-eabi-objcopy
C_OPT = -mcpu=cortex-a7 -O2 -Wall -Wextra -fpic -ffreestanding -std=gnu99 -nostdlib -I berryOS/include -g
L_OPT = -ffreestanding -O2 -nostdlib -mcpu=cortex-a7

#-fpic position independent code
SRC_DIR = berryOS/src
SRC_ARCH = berryOS/arch
BUILD_DIR = build

run: build
	@echo ""
	@echo "Running qemu..."
	qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel $(BUILD_DIR)/myos.elf

#target for .c files
$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c 
	@mkdir -p $(@D)
	@echo "Compiling .c files..."
	$(CC)-gcc $(C_OPT) -MMD -c $< -o $@
	@echo ""

#target for .s files 
$(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
	@mkdir -p $(@D)
	@echo "Compiling .S files..."
	$(CC)-gcc $(C_OPT) -MMD -c $< -o $@
	@echo ""$(SRC_DIR)/

C_FILES = $(wildcard $(SRC_DIR)/*/*/*.c)
C_FILES += $(wildcard $(SRC_DIR)/*/*.c)
C_FILES += $(wildcard $(SRC_DIR)/*.c)

ASM_FILES = $(wildcard $(SRC_ARCH)/ARMv7/*.S) #Remember to add the context.S when using processes
ASM_FILES += $(wildcard $(SRC_DIR)/proc/*/*.S) 
ASM_FILES += $(wildcard $(SRC_DIR)/proc/*.S) 

OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)

OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)

build: $(SRC_ARCH)/ARMv7/linker.ld $(OBJ_FILES)
	@echo "Linking .o files..."
	$(CC)-gcc -T $(SRC_ARCH)/ARMv7/linker.ld -o $(BUILD_DIR)/myos.elf $(L_OPT) $(OBJ_FILES)
	#$(CC)-objcopy $(BUILD_DIR)/myos.elf -O binary $(BUILD_DIR)/myos.bin
	@echo "Done!"

build_hard: build
	$(OBJCOPY) $(BUILD_DIR)/myos.elf -O binary $(BUILD_DIR)/kernel7.img
	@echo "Done!"

variable_test: 
	@echo "C_FILES:"
	@echo $(C_FILES)
	@echo "ASM_FILES:"
	@echo $(ASM_FILES)
	@echo "OBJ_FILES:"
	@echo $(OBJ_FILES)
	
clean:
	rm -rf $(BUILD_DIR)/