# Makefile
# David Grant, 2004
# Irene Huang, 2010/05/03 

TOP_DIR := ..

# Add source file name (without .c) here
OBJ_NAMES=null_process

OBJS=$(addprefix bin/, $(addsuffix .o, $(OBJ_NAMES)))
COMMON_INC:=rtx.h dbug.h

all: $(OBJS)

bin/null_process.o: $(COMON_INC) null_process.h

# Add an entry above ^^^ where its the form of:
# bin/<filename>.o: <header files the .h depends on>

include $(TOP_DIR)/make/coldfire.mk
