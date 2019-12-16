#
#	ARMv8 COMPILER
#
ARMGNU ?= aarch64-linux-gnu
COPS = -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only
ASMOPS = -Iinclude

#
#	ARMv7 COMPILER
#
ARMGNURP2 ?= ../compiler/bin/arm-none-eabi
CRP2OPS = -mcpu=cortex-a7 -O2 -Wall -Wextra -fpic -ffreestanding -std=gnu99 -nostdlib -I include 
ASRP2OP = -ffreestanding -O2 -nostdlib

#             EXPLICATION OPTION BY OPTION OF THE COPS VARIABLE
# -Wall warning all
# -nostdlib don't use the C standar library because most of
#   the calls in the C standard library eventually interact with the OS
# -nostartfiles don't use standard startup file (Files responsible for setting
#   an initial stack pointer, initializing static data and jumping to the main
#   entry point)
# -ffreestanding A ffreestanding evinronment is one in which the standar
#   library may not exist and program startup may not necessarily be at main
# -Iinclude search for headers file in the include folder
# -mgeneral-regs-only only use general purpose registers. This is because ARM
#   processors also have NEON registers (something about the gpu i think)


BUILD_DIR = build
SRC_DIR = src
AARCH64 = arch/ARMv8
AARCHARMv7 = arch/ARMv7

COMP = compiler.zip

all : unzip
	@echo "You have to specify what version to build or run"
	@echo "build3 --> Raspberry 3"
	@echo "build2 --> Raspberry 2"
	@echo "run --> Raspberry 2"

clean :
	rm -rf $(BUILD_DIR) *.img

# unzips compiler
unzip:
	unzip $(COMP)

#
#	ARMv8 RULES FOR COMPILING
#
#target for .c files for ARMv8
$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c 
	@mkdir -p $(@D)
	$(ARMGNU)-gcc $(COPS) -MMD -c $< -o $@

#target for .s files from ARMv8 architecture
$(BUILD_DIR)/%_s.o: ${AARCH64}/%.S
	$(ARMGNU)-gcc $(ASMOPS) -MMD -c $< -o $@

#
#	ARMv7 RULES FOR COMPILING
#
$(BUILD_DIR)/%_rp2c.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(ARMGNURP2)-gcc $(CRP2OPS) -MMD -c $< -o $@

#target for .s file from ARMv7 architecture
$(BUILD_DIR)/%_rp2s.o: ${AARCHARMv7}/%.S
	$(ARMGNURP2)-gcc $(ASRP2OP) -MMD -c $< -o $@

#target for .c files to compile to ARMv7 architecture

IO_FILES = $(wildcard $(SRC_DIR)/io/*.c)
C_FILES = $(wildcard $(SRC_DIR)/*.c)
C_FILES += $(IO_FILES)

#
#	ARMv8 list of .c and .S files to compile
#
ASM64_FILES = $(wildcard ${AARCH64}/*.S)

OBJ64_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ64_FILES += $(ASM64_FILES:${AARCH64}/%.S=$(BUILD_DIR)/%_s.o)

#
#	ARMv7 list of .c and .S files to compile
#
ASM32_FILES = $(wildcard ${AARCHARMv7}/*.S)

OBJ32_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_rp2c.o)
OBJ32_FILES += $(ASM32_FILES:${AARCHARMv7}/%.S=$(BUILD_DIR)/%_rp2s.o)

DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

build3: ${AARCH64}/linker.ld $(OBJ64_FILES)
	@echo "RP3: Linking .o files..."
	$(ARMGNU)-ld -T ${AARCH64}/linker.ld -o $(BUILD_DIR)/kernel8.elf  $(OBJ64_FILES)
	@echo ""
	@echo "Dumping .img file..."
	$(ARMGNU)-objcopy $(BUILD_DIR)/kernel8.elf -O binary kernel8.img
	@echo ""

build2: ${AARCHARMv7}/linker.ld $(OBJ32_FILES)
	@echo "RP2: Linking .o files..."
	${ARMGNURP2}-gcc -T ${AARCHARMv7}/linker.ld -o ${BUILD_DIR}/myos.elf -ffreestanding -O2 -nostdlib ${OBJ32_FILES}
	@echo ""
	@echo "Dumping .bin file..."
	$(ARMGNURP2)-objcopy $(BUILD_DIR)/myos.elf -O binary $(BUILD_DIR)/myos.bin
	@echo ""

run: build2
	@echo ""
	@echo "Running qemu..."
	qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel $(BUILD_DIR)/myos.elf
	
#arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -c boot.S -o boot.o
#arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -std=gnu99 -c kernel.c -o kernel.o -O2 -Wall -Wextra
#arm-none-eabi-gcc -T linker.ld -o myos.elf -ffreestanding -O2 -nostdlib boot.o kernel.o
#to run qemu
#qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel myos.elf