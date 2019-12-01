CC = compiler/bin/arm-none-eabi
C_OPT = -mcpu=cortex-a7 -O2 -Wall -Wextra -fpic -ffreestanding -std=gnu99 -nostdlib -I include 
L_OPT = -ffreestanding -O2 -nostdlib

#-fpic position independent code
SRC_DIR = src
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
	@echo ""

C_FILES = $(wildcard $(SRC_DIR)/*.c)

ASM_FILES = $(wildcard $(SRC_DIR)/*.S)

OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)

build: $(SRC_DIR)/linker.ld $(OBJ_FILES)
	@echo "Linking .o files..."
	$(CC)-gcc -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/myos.elf $(L_OPT) $(OBJ_FILES)
	$(CC)-objcopy $(BUILD_DIR)/myos.elf -O binary $(BUILD_DIR)/myos.bin
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
