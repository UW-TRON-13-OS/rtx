CC=m68k-elf-gcc
CXX=m68k-elf-g++
LD=m68k-elf-gcc
AS=m68k-elf-as
AR=m68k-elf-ar
ARFLAGS=-r
OBJCPY=m68k-elf-objcopy
OBJDMP=m68k-elf-objdump
START_ASM=../shared/start.s
LDFLAGS=-T../shared/rtx.ld -Wl,-Map=hello.map
MKDIR=mkdir -p
CD =cd

MODULE=`basename \`pwd\``

SRC_DIR:=src
INC_DIR:=inc
BIN_DIR:=bin
SHARED_DIR:=$(TOP_DIR)/shared

CFLAGS=-Wall -m5307 -pipe -nostdlib -T$(SHARED_DIR)/mcf5307.ld -I$(INC_DIR) -I$(TOP_DIR)/inc

vpath %.c $(SRC_DIR)
vpath %.h $(INC_DIR)
vpath %.h $(TOP_DIR)/$(INC_DIR)

SRC_FILES:=$(wildcard $(SRC_DIR)/*.c)

DIS_ASM_FILES:=$(addprefix $(BIN_DIR)/,  \
					$(addsuffix .s, \
						$(notdir \
							$(basename \
								$(SRC_FILES) \
							) \
						) \
					) \
				)

#bin/objectfile based on a .c (automatically found using vpath in src)
$(BIN_DIR)/%.o: %.c
	@echo "Module $(MODULE) - $< -> $@"
	@$(MKDIR) $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/%.s: %.c
	@$(MKDIR) $(dir $@)
	@$(CC) $(CFLAGS) -S -o $@ $<

show: $(DIS_ASM_FILES)
	@echo "Showing assemblies in $(BIN_DIR)"

clean:
	@rm -rf $(BIN_DIR) *.s19 *.o *.bin *.lst *.map
