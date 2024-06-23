RESULT ?= bootloader

DEFINES = \
	-D__SAME54P20A__ \
	-DUSE_CMSIS_INIT \
	-DUSE_SAME54_XPLAINED_PRO \
	-D_WINSOCK_H \
	-D__error_t_defined

INCLUDES = \
	-I../src \
	-I../../../../../../third_party/cmsis/include \
	-I../../../../../../third_party/microchip/devices/same54 \
	-I../../../../../../third_party/microchip/boards/same54_xplained_pro \
	-I../../../../../../common \
	-I../../../../../../cyclone_crypto \
	-I../../../../../../cyclone_boot

ASM_SOURCES =

C_SOURCES = \
	./startup_same54.c \
	../src/system_same54.c \
	./syscalls.c \
	../src/main.c \
	../src/debug.c \
	../../../../../../common/cpu_endian.c \
	../../../../../../common/os_port_none.c \
	../../../../../../common/date_time.c \
	../../../../../../common/str.c \
	../../../../../../cyclone_boot/core/crc32.c \
	../../../../../../cyclone_boot/drivers/mcu/arm/sam_ed_5x_mcu_driver.c \
	../../../../../../cyclone_boot/drivers/memory/flash/internal/sam_ed_5x_flash_driver.c \
	../../../../../../cyclone_boot/image/image.c \
	../../../../../../cyclone_boot/memory/memory.c \
	../../../../../../cyclone_boot/security/cipher.c \
	../../../../../../cyclone_boot/bootloader/boot.c \
	../../../../../../cyclone_boot/bootloader/boot_fallback.c \
	../../../../../../cyclone_boot/bootloader/boot_common.c \
	../../../../../../cyclone_crypto/hash/sha256.c \
	../../../../../../cyclone_crypto/cipher/aes.c \
	../../../../../../cyclone_crypto/cipher_modes/cbc.c

HEADERS = \
	../src/os_port_config.h \
	../src/boot_config.h \
	../src/crypto_config.h \
	../../../../../../common/cpu_endian.h \
	../../../../../../common/os_port.h \
	../../../../../../common/os_port_none.h \
	../../../../../../common/date_time.h \
	../../../../../../common/str.h \
	../../../../../../common/error.h \
	../../../../../../common/debug.h \
	../../../../../../cyclone_boot/core/cboot_error.h \
	../../../../../../cyclone_boot/core/crc32.h \
	../../../../../../cyclone_boot/core/flash.h \
	../../../../../../cyclone_boot/core/mcu.h \
	../../../../../../cyclone_boot/drivers/mcu/arm/sam_ed_5x_mcu_driver.h \
	../../../../../../cyclone_boot/drivers/memory/flash/internal/sam_ed_5x_flash_driver.h \
	../../../../../../cyclone_boot/image/image.h \
	../../../../../../cyclone_boot/memory/memory.h \
	../../../../../../cyclone_boot/memory/memory_ex.h \
	../../../../../../cyclone_boot/security/cipher.h \
	../../../../../../cyclone_boot/bootloader/boot.h \
	../../../../../../cyclone_boot/bootloader/boot_fallback.h \
	../../../../../../cyclone_boot/bootloader/boot_common.h \
	../../../../../../cyclone_crypto/core/crypto.h \
	../../../../../../cyclone_crypto/cipher/aes.h \
	../../../../../../cyclone_crypto/cipher_modes/cbc.h \
	../../../../../../third_party/microchip/boards/same54_xplained_pro/same54_xplained_pro.h

ASM_OBJECTS = $(patsubst %.S, %.o, $(ASM_SOURCES))

C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))

OBJ_DIR = obj_build

LINKER_SCRIPT = same54_flash.ld

CFLAGS += -fno-common -Wall -Os -g3
CFLAGS += -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CFLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections
CFLAGS += $(DEFINES)
CFLAGS += $(INCLUDES)

CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy
SIZE = $(CROSS_COMPILE)size

THIS_MAKEFILE := $(lastword $(MAKEFILE_LIST))

all:
	$(MAKE) pre-build build size
	$(MAKE) post-build 

install:
	$(MAKE) pre-build build size
	$(MAKE) post-build
	$(MAKE) flash

pre-build: ;

post-build:
	python ../scripts/misc/copy_file.py bootloader.bin ../

build: $(RESULT).elf $(RESULT).lst $(RESULT).bin $(RESULT).hex
	
$(RESULT).elf: $(ASM_OBJECTS) $(C_OBJECTS) $(HEADERS) $(LINKER_SCRIPT) $(THIS_MAKEFILE)
	$(CC) -Wl,-M=$(RESULT).map -Wl,-T$(LINKER_SCRIPT) $(CFLAGS) $(addprefix $(OBJ_DIR)/, $(notdir $(ASM_OBJECTS))) $(addprefix $(OBJ_DIR)/, $(notdir $(C_OBJECTS))) -o $@

$(ASM_OBJECTS): | $(OBJ_DIR)

$(C_OBJECTS): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir -p $@

%.o: %.c $(HEADERS) $(THIS_MAKEFILE)
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(OBJ_DIR)/, $(notdir $@))

%.o: %.S $(HEADERS) $(THIS_MAKEFILE)
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(OBJ_DIR)/, $(notdir $@))

%.lst: %.elf
	$(OBJDUMP) -x -S $(RESULT).elf > $@

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

size: $(RESULT).elf
	$(SIZE) $(RESULT).elf

flash:
	openocd -f board/microchip_same54_xplained_pro.cfg -c "init; reset halt; flash write_image erase $(RESULT).bin 0x00000000; reset run; shutdown"

clean:
	rm -f $(RESULT).elf
	rm -f $(RESULT).bin
	rm -f $(RESULT).map
	rm -f $(RESULT).hex
	rm -f $(RESULT).lst
	rm -f $(OBJ_DIR)/*.o
