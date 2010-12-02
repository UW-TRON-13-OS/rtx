CC=m68k-elf-gcc
CXX=m68k-elf-g++
LD=m68k-elf-gcc
AS=m68k-elf-as
AR=m68k-elf-ar
RANLIB=m68k-elf-ranlib
ARFLAGS=-r
OBJCPY=m68k-elf-objcopy --output-format=srec 
OBJDMP=m68k-elf-objdump -xdC 
START_ASM=$(TOP_DIR)/shared/start.s
LDFLAGS=-T$(TOP_DIR)/shared/rtx.ld -Wl,-Map=hello.map
MKDIR=mkdir -p
CD =cd

ASM=$(wildcard $(SRC_DIR)/*.s)

MODULE=$(shell basename $(shell pwd))

SRC_DIR:=src
INC_DIR:=inc
BIN_DIR:=bin
TEST_SRC_DIR:=test
SHARED_DIR:=$(TOP_DIR)/shared
LIB_DIR:=$(TOP_DIR)/lib
TEST_DIR:=$(TOP_DIR)/test

INCLUDE:=-I$(INC_DIR) -I$(TOP_DIR)/inc -I$(TOP_DIR)/user/inc -I$(TOP_DIR)/util/inc
CFLAGS=-Wall -m5307 -pipe -nostdlib  $(INCLUDE) $(LOCAL_CFLAGS)

vpath %.c $(SRC_DIR)
vpath %.h $(INC_DIR)
vpath %.h $(TOP_DIR)/$(INC_DIR)
vpath %.h $(TOP_DIR)/util/inc
vpath %.h $(TOP_DIR)/user/inc

SRC_FILES:=$(wildcard $(SRC_DIR)/*.c)
LIB_SRC:=$(filter-out $(basename $(MAIN_FILE)), $(OBJ_NAMES))
LIB_OBJ:=$(addprefix $(BIN_DIR)/, $(addsuffix .o, $(LIB_SRC)))
TEST_FILES:=$(wildcard $(TEST_SRC_DIR)/*.c)

DIS_ASM_FILES:=$(addprefix $(BIN_DIR)/,  \
					$(addsuffix .s, \
						$(notdir \
							$(basename \
								$(SRC_FILES) \
							) \
						) \
					) \
				)

LIB:=$(LIB_DIR)/lib$(MODULE).a
APP=
TESTS:=$(addprefix $(TEST_DIR)/, $(notdir $(basename $(TEST_FILES))))
ifeq ($(MODULE),kern)
	APP=$(TOP_DIR)/rtx
	APPDEPS=$(addprefix $(LIB_DIR)/lib, $(addsuffix .a, $(DEPS)))
endif

# TARGETS
all: $(APP) $(LIB) $(TESTS)
	@echo "Building $(APP) $(MODULE)"
	
show: $(DIS_ASM_FILES)
	@echo "Showing dis-assemblies in $(BIN_DIR)"

clean:
	@rm -rf $(BIN_DIR) *.s19 *.o *.bin *.lst *.map $(LIB)

# RULES
$(TEST_DIR)/%: $(TEST_SRC_DIR)/%.c $(LIB) $(START_ASM) $(ASM) $(APPDEPS)
	@echo "	   Compiling Test $@"
	@$(MKDIR) $(TEST_DIR)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(TEST_FLAGS) -o $@.bin $(START_ASM) $(ASM) $< \
		$(LIB) $(APPDEPS)
	@$(OBJCPY) $@.bin $@.s19
	@$(OBJDMP) $@.bin > $@.lst
	@chmod u+x $@.s19

$(APP): $(LIB) $(MAIN_FILE) $(START_ASM) $(ASM) $(APPDEPS)
	@echo "Making app $(APP)"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(APP).bin $(START_ASM) $(ASM) $(MAIN_FILE) \
		$(LIB) $(APPDEPS)
	@$(OBJCPY) $(APP).bin $(APP).s19
	@$(OBJDMP) $(APP).bin > $(APP).lst
	@chmod u+x $(APP).s19

$(LIB): $(LIB_OBJ)
	@echo "    Building Library lib$(MODULE).a"
	@$(MKDIR) $(LIB_DIR)
	@$(AR) $(ARFLAGS) $(LIB) $(LIB_OBJ) > /dev/null
	@$(RANLIB) $(LIB)

$(BIN_DIR)/%.o: %.c
	@echo "Module $(MODULE) - $< -> $@"
	@$(MKDIR) $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/%.s: %.c
	@$(MKDIR) $(dir $@)
	@$(CC) $(CFLAGS) -S -o $@ $<

$(LIB_DIR)/lib%.a: .FORCE
	@echo "Building Dependency $(notdir $@)"
	@make -C $(TOP_DIR)/$* --no-print-directory

.FORCE:
