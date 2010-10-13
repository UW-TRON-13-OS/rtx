CC:=gcc
OBJ_DIR:=bin
SRC_DIR:=src
INC_DIR:=inc

CFLAGS=-Wall -I$(TOP_DIR)/inc -I$(INC_DIR) $(LOCAL_CFLAGS)
LDFLAGS=$(LOCAL_LDFLAGS)

APP_SRC := $(notdir $(basename $(wildcard $(SRC_DIR)/*.c)))
APP_OBJ := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(APP_SRC)))

BIN_DIR := $(TOP_DIR)/bin

MKDIR=mkdir -p

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "    Make $(notdir $<) -> $(notdir $@)"
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c -o $@ $< 
	@rm -f *.o

all: $(APP) $(LIB)

$(APP): $(APP_OBJ) $(REQ_LIBS) $(LIB)
	@echo "    Link application $(notdir $@)"
	@$(MKDIR) $(BIN_DIR)
	@$(CC) $(LDFLAGS) -o $(TOP_DIR)/$(OBJ_DIR)/$@ $(APP_OBJ) 

clean:
	@echo Clean $(APP)
	@rm -fr $(OBJ_DIR)
