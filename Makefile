# Makefile for nRF9160 bare metal programming                 PVH, October 2022

# Arm GNU toolchain can be found here (look for gcc-arm-none-eabi)
# https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain
# nrfx is available at https://github.com/NordicSemiconductor/nrfx
# CMSIS can be found at https://github.com/ARM-software/CMSIS_5

# Path to Toolchain
TOOLCHAINPATH ?= /usr

# Path to mergehex command
MERGEHEX ?= /usr/bin/mergehex

# Paths to 3rd party dependencies
NRFXPATH ?= ${PWD}/3rd-party/nrfx
CMSISPATH ?= ${PWD}/3rd-party/CMSIS_5/CMSIS

# Specify project name, object files, headers (DEPS) and linker script
PROJECT = riot-tee
S_LDSCRIPT = ld/common.ld
BUILDDIR ?= $(PWD)/build
NSC_LIB = ${BUILDDIR}/nsc_lib.o

# Startup and system code
S_SOURCES = \
	${NRFXPATH}/mdk/gcc_startup_nrf9160.S \
	${NRFXPATH}/mdk/system_nrf9160.c \
	src/main.c \
	src/non_secure_entry.c \

# Common flags for CC, AS and LD
FLAGS = -mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mabi=aapcs
FLAGS += -mfpu=fpv5-sp-d16 -DNRF9160_XXAA
FLAGS += -DCONFIG_GPIO_AS_PINRESET -DFLOAT_ABI_HARD

# Shortcuts for various tools
CC = ${TOOLCHAINPATH}/bin/arm-none-eabi-gcc
AS = ${TOOLCHAINPATH}/bin/arm-none-eabi-gcc
LD = ${TOOLCHAINPATH}/bin/arm-none-eabi-gcc
OBJCOPY = ${TOOLCHAINPATH}/bin/arm-none-eabi-objcopy
SIZETOOL = ${TOOLCHAINPATH}/bin/arm-none-eabi-size

# Compiler flags
CFLAGS = ${FLAGS} -std=c99 -Wall -Werror
CFLAGS += -I$(PWD)/target/nrf9160
CFLAGS += -I${CMSISPATH}/Core/Include
CFLAGS += -I${NRFXPATH}
CFLAGS += -I${NRFXPATH}/templates
CFLAGS += -I${NRFXPATH}/mdk
CFLAGS += -I${NRFXPATH}/hal
CFLAGS += -I${NRFXPATH}/drivers/include

# Common flags for secure code
SFLAGS = -mcmse

# Assembler flags
AFLAGS = ${FLAGS} -x assembler-with-cpp

# Linker flags
S_LDFLAGS = ${FLAGS} -T "$(S_LDSCRIPT)" -Xlinker
S_LDFLAGS += --cmse-implib -Xlinker --out-implib=$(NSC_LIB) -Xlinker
S_LDFLAGS += --gc-sections -Xlinker -Map="$(BUILDDIR)/$(PROJECT).map"
S_LDFLAGS += --specs=nano.specs
S_LDFLAGS += -L"${NRFXPATH}/mdk/"
S_LDFLAGS += -L"target/nrf9160/"

LIBS = -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group

# Check whether to optimize or build for debugging
DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CFLAGS += -O0 -g3 -gdwarf-2
	AFLAGS += -g3 -gdwarf-2
	LDFLAGS += -g3
else
	CFLAGS += -O3
endif

# Substitute the correct path for the object filenames
# _OBJ = $(patsubst %,$(BUILDDIR)/%,$(OBJ))
S_OBJECTS := $(addsuffix .o,$(S_SOURCES))

# Replace NRFX paths since objects here are going to be build in the build dir.
S_OBJECTS := $(subst $(NRFXPATH),nrfx,$(S_OBJECTS))

# Finally, add build directory prefix
S_OBJECTS := $(addprefix $(BUILDDIR)/,$(S_OBJECTS))

PREPROCESSED_LDSCRIPT := $(BUILDDIR)/ldscript.lds

# Build the project
all: $(BUILDDIR)/$(PROJECT).elf

$(BUILDDIR)/$(PROJECT).elf: $(S_OBJECTS)
	@echo "[LD] Linking boot image $@"
	@$(LD) $(S_LDFLAGS) -o $@ $(S_OBJECTS) $(LIBS)
	@echo "[OBJCOPY] $@ -> $(BUILDDIR)/$(PROJECT).hex"
	@objcopy -O ihex $@ $(BUILDDIR)/$(PROJECT).hex
	@echo "[OBJCOPY] $@ -> $(BUILDDIR)/$(PROJECT).bin"
	@objcopy -I ihex -O binary $(BUILDDIR)/$(PROJECT).hex $(BUILDDIR)/$(PROJECT).bin
	@$(SIZETOOL) $@

# Recipe for building C objects in the secure imaga
$(BUILDDIR)/src/%.c.o: src/%.c
	@echo "[CC] $< -> $@"
	@mkdir -p $(@D)
	$(CC) $(SFLAGS) $(CFLAGS) -c $< -o $@

# Recipe for assembling C objects in the secure imaga
$(BUILDDIR)/src/%.S.o: src/%.S
	@echo "[AS] $< -> $@"
	@mkdir -p $(@D)
	@$(AS) $(SFLAGS) $(S_INCLUDES) $(AFLAGS) -c $< -o $@

# Recipe for building C objects in $(NRFXPATH) for the secure image
$(BUILDDIR)/nrfx/%.c.o: $(NRFXPATH)/%.c
	@echo "[CC] $< -> $@"
	@mkdir -p $(@D)
	@$(CC) $(SFLAGS) $(CFLAGS) -c $< -o $@

# Recipe for assembling objects in $(NRFXPATH) for the image
$(BUILDDIR)/nrfx/%.S.o: $(NRFXPATH)/%.S
	@echo "[AS] $< -> $@"
	@mkdir -p $(@D)
	@$(AS) $(SFLAGS) $(AFLAGS) -c $< -o $@

# Remove build folder
clean:
	rm -dfr $(BUILDDIR)
