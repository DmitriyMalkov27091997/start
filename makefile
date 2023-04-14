###############################################################################
#
#                                 MAKEFILE
#
###############################################################################

#------------------------------------------------------------------------------
#    Project options
#------------------------------------------------------------------------------
  SW_NAME 			= am800
  SW_DATED			= $(shell perl tools\scripts\date.pl tools\datetime.exe)
  OPT 					= s

  SW_VER_MAJ		= 1
  SW_VER_MIN		= 6
  SW_VER_BUILD	= 112
  SW_SERIAL			= 0x00000001 
  SW_VERSION		= $(SW_VER_MAJ).$(SW_VER_MIN).$(SW_VER_BUILD)
  SW_DATET			= $(shell perl tools\scripts\date.pl tools\datetime.exe)

  MAIN_OUT 			=	$(SW_NAME).v$(SW_VERSION).$(SW_DATED)
  # list
  ASM_FILES = 

#------------------------------------------------------------------------------
#    PATH
#------------------------------------------------------------------------------
  
  # tools
  GCC_DIR           = D:/Projects/AM800_Firmware
  OUTPUT_OBJ				= ./obj
  OUTPUT_BIN				= ./bin
  
  KLEN_DIR          = $(GCC_DIR)/tools/arm-kgp-eabi-procyon/bin
  CC                = $(KLEN_DIR)/arm-kgp-eabi-gcc
  CPP               = $(KLEN_DIR)/arm-kgp-eabi-gcc
  LD                = $(KLEN_DIR)/arm-kgp-eabi-gcc
  AS                = $(KLEN_DIR)/arm-kgp-eabi-gcc
  OBJCP             = $(KLEN_DIR)/arm-kgp-eabi-objcopy
  OD                = $(KLEN_DIR)/arm-kgp-eabi-objdump
  AR                = $(KLEN_DIR)/arm-kgp-eabi-ar
  SZ                = $(KLEN_DIR)/arm-kgp-eabi-size
  DG                = $(GCC_DIR)/doc/doxygen/bin/doxygen.exe"
  PG                = "C:/Program Files (x86)/STMicroelectronics/STM32 ST-LINK Utility/ST-LINK Utility/ST-LINK_CLI.exe"
  VSPG							=	$(GCC_DIR)/tools/vsprog/vsprog.exe

  # source  

  INCLUDE_DIRS	    = -I $(GCC_DIR)/drivers/stm32/inc
  LIBRARY_DIRS	    = 

  COMPILE_OPTS      = -mcpu=cortex-m3 -mthumb -fpack-struct -Wpadded -g -O$(OPT) -DSW_DATE=\"$(SW_DATET)\" -DSW_NAME=\"$(MAIN_OUT)\" -DSW_VER_MAJ=$(SW_VER_MAJ) -DSW_VER_MIN=$(SW_VER_MIN) -DSW_VER_BUILD=$(SW_VER_BUILD)
  CFLAGS            = $(COMPILE_OPTS) $(INCLUDE_DIRS)
  ASFLAGS           = $(COMPILE_OPTS) -c
  LDFLAGS 					= -mcpu=cortex-m3 -mthumb -Wl,--gc-sections,-Map=$@.map,-cref,-u,Reset_Handler $(INCLUDE_DIRS) $(LIBRARY_DIRS) -T STM32L152RCT6.ld
  OBJCPFLAGS        = -O binary
  ARFLAGS           = cr

  MAIN_OUT_ELF      = $(OUTPUT_OBJ)/$(MAIN_OUT).elf
  MAIN_OUT_BIN      = $(OUTPUT_BIN)/$(MAIN_OUT).bin
  MAIN_OUT_SRC      = $(OUTPUT_OBJ)/$(MAIN_OUT).src
  MAIN_OUT_LST      = $(OUTPUT_OBJ)/$(MAIN_OUT).lst

###############################################################################
#
#                              OBJECT FILES
#
###############################################################################

#------------------------------------------------------------------------------
#    Total O-files
#------------------------------------------------------------------------------
SW_OBJ_FILES = 
SW_OBJ_FILES += main.o
SW_OBJ_FILES += startup_STM32L152RCT6.o
SW_OBJ_FILES += command.o
SW_OBJ_FILES += define.o
SW_OBJ_FILES += init.o
SW_OBJ_FILES += isr.o
SW_OBJ_FILES += serial_port.o
SW_OBJ_FILES += exception.o
SW_OBJ_FILES += font.o
SW_OBJ_FILES += pcf8531.o
SW_OBJ_FILES += hart.o
SW_OBJ_FILES += picto.o
SW_OBJ_FILES += menu.o
SW_OBJ_FILES += menu_service.o
SW_OBJ_FILES += menu_helper.o
SW_OBJ_FILES += menu_navigate.o
SW_OBJ_FILES += menu_render.o
SW_OBJ_FILES += menu_v2.o
SW_OBJ_FILES += watchdog.o
SW_OBJ_FILES += crc32.o
SW_OBJ_FILES += spi_sensor.o

OBJ_FILES = $(patsubst %.o, $(OUTPUT_OBJ)/%.o, $(SW_OBJ_FILES))

###############################################################################
#
#                                   TARGETS
#
###############################################################################

#------------------------------------------------------------------------------
#    all
#------------------------------------------------------------------------------
all: $(MAIN_OUT_ELF) $(MAIN_OUT_BIN) $(MAIN_OUT_LST) $(ASM_FILES) $(MAIN_OUT_LST)
	$(SZ) -B $(MAIN_OUT_ELF) 

#------------------------------------------------------------------------------
#    clean
#------------------------------------------------------------------------------
clean:
	tools\msys\bin\rm.exe $(OBJ_FILES) $(MAIN_OUT_BIN) $(MAIN_OUT_SRC) $(MAIN_OUT_LST) $(MAIN_OUT_ELF) $(MAIN_OUT_ELF).map

#------------------------------------------------------------------------------
#    prog
#------------------------------------------------------------------------------
prog: $(MAIN_OUT_BIN)
	$(PG) -c SWD UR LPM -ME -P $(MAIN_OUT_BIN) 0x08000000 -Rst -Run 
# -w32 0x0800FFF0 $(SW_SERIAL)

reset:
	$(PG) -c SWD -Rst -Run

vsprog: $(MAIN_OUT_BIN)
	$(VSPG) -cstm32l1 -ms -W0 -I $(MAIN_OUT_BIN)@0,0x08000000 -oe -owf

#------------------------------------------------------------------------------
#    show
#------------------------------------------------------------------------------
show:
	@echo "show obj-files..."
	@echo "$(OBJ_FILES)"

###############################################################################
#
#                            COMPILE AND LINK
#
###############################################################################

#------------------------------------------------------------------------------
#    compile c
#------------------------------------------------------------------------------
.SUFFIXES: .o .c
$(OUTPUT_OBJ)/%.o: %.c
	@echo "compile " $@
	$(CC) $(CFLAGS) -c $< -o $@

#------------------------------------------------------------------------------
#    compile c -> asm
#------------------------------------------------------------------------------
.SUFFIXES: .asm .c .h
%.asm: %.c %.h
	$(CC) $(CFLAGS) -S -c $< -o $@

#------------------------------------------------------------------------------
#    compile s
#------------------------------------------------------------------------------
.SUFFIXES: .o .s
$(OUTPUT_OBJ)/%.o: %.s
	@echo "compile " $@
	$(AS) $(CFLAGS) -c $< -o $@

#------------------------------------------------------------------------------
#    link all
#------------------------------------------------------------------------------
$(MAIN_OUT_ELF): $(OBJ_FILES)
	@echo "linker.."
	$(LD) $(LDFLAGS) $(OBJ_FILES) --output $@

#------------------------------------------------------------------------------
#    create bin
#------------------------------------------------------------------------------
$(MAIN_OUT_SRC): $(MAIN_OUT_ELF)
	@echo "hex2bin..."
	$(OBJCP) $(OBJCPFLAGS) $< $@
	$(SZ) -A $(MAIN_OUT_ELF) 

$(MAIN_OUT_BIN): $(MAIN_OUT_SRC)
	tools\ppm_create.exe app_size=131072 infile=$(MAIN_OUT_SRC) outfile=$(MAIN_OUT_BIN) 

#------------------------------------------------------------------------------
#		listing
#------------------------------------------------------------------------------
.SUFFIXES: .lst .elf
%.lst: %.elf
	$(OD) -dStC $< >> $@

###############################################################################
#
#                                    END
#
###############################################################################