CC:=gcc
AR:=ar -r
RANLIB:=ranlib
RM:=rm -f
MKDIR=mkdir -p
CHMOD=chmod
RUN_TESTS=./$(TOP_DIR)/make/run_tests.sh

MODULE=$(shell basename $(shell pwd))

OBJ_DIR:=bin
SRC_DIR:=src
INC_DIR:=inc
TEST_SRC_DIR:=test

MAIN_FILE=main

BIN_DIR=$(TOP_DIR)/bin
LIB_DIR=$(TOP_DIR)/lib
TEST_DIR=$(TOP_DIR)/tests

LIB=$(LIB_DIR)/lib$(MODULE).a
LIBDEPS := $(addprefix $(LIB_DIR)/lib, $(addsuffix .a, $(APPDEPS)))

CFLAGS=-Wall -I$(TOP_DIR)/inc -I$(INC_DIR) $(LOCAL_CFLAGS)
LDFLAGS=$(LOCAL_LDFLAGS)

SRC_FILES := $(notdir $(basename $(wildcard $(SRC_DIR)/*.c)))

TEST_FILES := $(notdir $(basename $(wildcard $(TEST_SRC_DIR)/*.c)))
TESTS := $(addprefix $(TEST_DIR)/$(MODULE)/, $(TEST_FILES))

LIB_SRC= $(filter-out $(basename $(MAIN_FILE)), $(SRC_FILES))
LIB_OBJ := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(LIB_SRC)))
TEST_LIB:=$(LIB_DIR)/libutest.a

ifeq ($(findstring $(MAIN_FILE), $(SRC_FILES)), $(MAIN_FILE))
	APP=$(BIN_DIR)/$(MODULE)
	APP_SRC := $(SRC_FILES)
	APP_OBJ := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(APP_SRC)))
else
	APP=
endif

BIN_DIR := $(TOP_DIR)/bin

.PHONY: clean all test $(TEST_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "    Make $(notdir $<) -> $(notdir $@)"
	@$(MKDIR) $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c -o $@ $< 
	@$(RM) *.o

$(TEST_DIR)/$(MODULE)/%: $(TEST_SRC_DIR)/%.c $(LIB) $(TEST_LIB)
	@echo "    +Compiling Test $@"
	@$(MKDIR) $(TEST_DIR)/$(MODULE)
	@$(CC) $(CFLAGS) -o $@ $< -L$(LIB_DIR) $(LIB) $(addprefix -l, $(APPDEPS)) -lutest

$(LIB_DIR)/lib%.a : FORCE
	@echo "    Make required library $*"
	@$(MAKE) --no-print-directory -C $(TOP_DIR)/$*

all: $(APP) $(LIB) $(TESTS)

FORCE:

test: $(TESTS)
	@$(CHMOD) 755 $(RUN_TESTS)
	@$(RUN_TESTS) $(MODULE)

$(APP): $(APP_OBJ) $(REQ_LIBS) $(LIB)
	@echo "    Link application $(notdir $@)"
	@$(MKDIR) $(BIN_DIR)
	@$(CC) $(LDFLAGS) -o $(TOP_DIR)/$(OBJ_DIR)/$@ $(APP_OBJ) \
										-L$(LIB_DIR) $(addprefix -l, $(APPDEPS))

$(LIB): $(LIB_OBJ)
	@echo "    Building Library lib$(MODULE).a"
	@$(MKDIR) $(LIB_DIR)
	@$(AR) $(LIB) $(LIB_OBJ)
	@$(RANLIB) $(LIB)

clean:
	@echo Clean $(MODULE)
	@$(RM) -r $(OBJ_DIR)
	@$(RM) $(LIB_DIR)/lib$(MODULE).a
	@$(RM) $(BIN_DIR)/$(MODULE)
	@$(RM) -r $(TEST_DIR)/$(MODULE)
