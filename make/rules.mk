CC:=gcc
AR:=ar -r
RANLIB:=ranlib
RM:=rm -f
MKDIR=mkdir -p

OBJ_DIR:=bin
SRC_DIR:=src
INC_DIR:=inc
TEST_SRC:=test

BIN_DIR=$(TOP_DIR)/bin
LIB_DIR=$(TOP_DIR)/lib

LIB=$(LIB_DIR)/lib$(MODULE).a
APP=$(BIN_DIR)/$(MODULE)

CFLAGS=-Wall -I$(TOP_DIR)/inc -I$(INC_DIR) $(LOCAL_CFLAGS)
LDFLAGS=$(LOCAL_LDFLAGS)

SRC_FILES := $(notdir $(basename $(wildcard $(SRC_DIR)/*.c)))
APP_SRC := $(SRC_FILES)
APP_OBJ := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(APP_SRC)))
TEST_OBJ := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(TEST_SRC)))
LIB_SRC= $(filter-out $(basename $(MAIN_FILE)), $(SRC_FILES))
LIB_OBJ := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(LIB_SRC)))

BIN_DIR := $(TOP_DIR)/bin

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "    Make $(notdir $<) -> $(notdir $@)"
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c -o $@ $< 
	@rm -f *.o

all: $(APP) $(LIB)

$(APP): $(APP_OBJ) $(REQ_LIBS)
	@echo "    Link application $(notdir $@)"
	@$(MKDIR) $(BIN_DIR)
	@$(CC) $(LDFLAGS) -o $(TOP_DIR)/$(OBJ_DIR)/$@ $(APP_OBJ) 

$(LIB): $(LIB_OBJ)
	@echo "    Building Library lib$(MODULE).a"
	@$(MKDIR) $(LIB_DIR)
	@$(AR) $(LIB) $(LIB_OBJ)
	@$(RANLIB) $(LIB)

clean:
	@echo Clean $(APP)
	@rm -fr $(OBJ_DIR)
