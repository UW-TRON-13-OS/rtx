# Makefile
# David Grant, 2004
# Irene Huang, 2010/05/03 

TOP_DIR := ..

# Add source file name (without .c) here
OBJ_NAMES=null_process
OBJ_NAMES=a
OBJ_NAMES=b
OBJ_NAMES=c

OBJS=$(addprefix bin/, $(addsuffix .o, $(OBJ_NAMES)))
COMMON_INC:=rtx.h dbug.h

all: $(OBJS)

bin/null_process.o: $(COMON_INC) null_process.h
bin/a.o: $(COMON_INC) abc.h
bin/b.o: $(COMON_INC) abc.h
bin/c.o: $(COMON_INC) abc.h

# Add an entry above ^^^ where its the form of:
# bin/<filename>.o: <header files the .h depends on>

include $(TOP_DIR)/make/coldfire.mk
