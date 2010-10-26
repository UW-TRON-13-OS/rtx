CC:=gcc
AR:=ar -r
RANLIB:=ranlib
RM:=rm -f
MKDIR=mkdir -p

OBJ_DIR:=bin
SRC_DIR:=src
INC_DIR:=inc

MAIN_FILE=main

BIN_DIR=$(TOP_DIR)/bin
LIB_DIR=$(TOP_DIR)/lib
TEST_DIR=$(TOP_DIR)/test

LIB=$(LIB_DIR)/lib$(MODULE).a

CFLAGS=-Wall -I$(TOP_DIR)/inc -I$(INC_DIR) $(LOCAL_CFLAGS)
LDFLAGS=$(LOCAL_LDFLAGS)

SRC_FILES := $(notdir $(basename $(wildcard $(SRC_DIR)/*.c)))

TEST_SRC := $(filter test, $(wildcard *))
TEST_FILES := $(notdir $(basename $(wildcard $(TEST_SRC)/*.c)))
TESTS := $(addprefix $(TEST_DIR)/, $(TEST_FILES))

LIB_SRC= $(filter-out $(basename $(MAIN_FILE)), $(SRC_FILES))
LIB_OBJ := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(LIB_SRC)))


ifeq ($(findstring $(MAIN_FILE), $(SRC_FILES)), $(MAIN_FILE))
	APP=$(BIN_DIR)/$(MODULE)
	APP_SRC := $(SRC_FILES)
	APP_OBJ := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(APP_SRC)))
else
	APP=
endif

BIN_DIR := $(TOP_DIR)/bin

.PHONY: clean all $(TEST_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "    Make $(notdir $<) -> $(notdir $@)"
	@$(MKDIR) $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c -o $@ $< 
	@rm -f *.o

$(TEST_DIR)/%: $(TEST_SRC)/%.c $(LIB)
	@echo "Compiling Test $@"
	@$(MKDIR) $(TEST_DIR)
	@$(CC) $(CFLAGS) -o $@ $< $(LIB) 

all: $(APP) $(LIB) $(TESTS)

$(APP): $(APP_OBJ) $(REQ_LIBS) $(LIB)
	@echo "    Link application $(notdir $@)"
	@$(MKDIR) $(BIN_DIR)
	@$(CC) $(LDFLAGS) -o $(TOP_DIR)/$(OBJ_DIR)/$@ $(APP_OBJ)  -L$(LIB_DIR)

$(LIB): $(LIB_OBJ)
	@echo "    Building Library lib$(MODULE).a"
	@$(MKDIR) $(LIB_DIR)
	@$(AR) $(LIB) $(LIB_OBJ)
	@$(RANLIB) $(LIB)

#$(TESTS):
#	@echo "akjsdlfjsdf $@"
#	@echo "akjsdlfjsdf $(TEST_DIR)"
#	@echo "akjsdlfjsdf $(TEST_FILES)"

clean:
	@echo Clean $(MODULE)
	@rm -fr $(OBJ_DIR)
