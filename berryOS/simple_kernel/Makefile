ARMGNU ?= aarch64-linux-gnu
COPS = -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only

ARMGNURP2 ?= arm-none-eabi-gcc
CRP2OPS = -mcpu=cortex-a7 -Iinclude -fpic -ffreestanding -std=gnu99 -O2 -Wall -Wextra
ASRP2OP = -mcpu=cortex-a7 -fpic -ffreestanding

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
ASMOPS = -Iinclude

BUILD_DIR = build
SRC_DIR = src
AARCH64 = arch/ARMv8
AARCHARMv7 = arch/ARMv7

all : kernel8.img

clean :
	rm -rf $(BUILD_DIR) *.img

#target for .c files
$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(COPS) -MMD -c $< -o $@

#target for .s files from ARMv8 architecture
$(BUILD_DIR)/%_s.o: $(SRC_DIR)/${AARCH64}/%.S
	$(ARMGNU)-gcc $(ASMOPS) -MMD -c $< -o $@

#target for .s file from ARMv7 architecture
$(BUILD_DIR)/%_rp2s.o: $(SRC_DIR)/${AARCHARMv7}/%.S
	$(ARMGNURP2) $(ASRP2OP) -MMD -c $< -o $@

#target for .c files to compile to ARMv7 architecture
$(BUILD_DIR)/%_rp2c.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(ARMGNURP2) $(CRP2OPS) -MMD -c $< -o $@

C_FILES = $(wildcard $(SRC_DIR)/*.c)

ASM64_FILES = $(wildcard $(SRC_DIR)/${AARCH64}/*.S)
ASM32_FILES = $(wildcard $(SRC_DIR)/${AARCHARMv7}/*.S)

OBJ64_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ64_FILES += $(ASM64_FILES:$(SRC_DIR)/${AARCH64}/%.S=$(BUILD_DIR)/%_s.o)

OBJ32_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_rp2c.o)
OBJ32_FILES += $(ASM32_FILES:$(SRC_DIR)/${AARCHARMv7}/%.S=$(BUILD_DIR)/%_rp2s.o)

DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

kernel8.img: $(SRC_DIR)/${AARCH64}/linker.ld $(OBJ64_FILES)
	$(ARMGNU)-ld -T $(SRC_DIR)/${AARCH64}/linker.ld -o $(BUILD_DIR)/kernel8.elf  $(OBJ64_FILES)
	$(ARMGNU)-objcopy $(BUILD_DIR)/kernel8.elf -O binary kernel8.img

myos.elf: $(SRC_DIR)/${AARCHARMv7}/linker.ld $(OBJ32_FILES)
	${ARMGNURP2} -T ${SRC_DIR}/${AARCHARMv7}/linker.ld -o ${BUILD_DIR}/myos.elf -ffreestanding -O2 -nostdlib ${OBJ32_FILES}

#arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -c boot.S -o boot.o
#arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -std=gnu99 -c kernel.c -o kernel.o -O2 -Wall -Wextra
#arm-none-eabi-gcc -T linker.ld -o myos.elf -ffreestanding -O2 -nostdlib boot.o kernel.o
#to run qemu
#qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel myos.elf