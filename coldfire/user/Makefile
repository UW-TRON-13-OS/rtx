# Makefile
# Author: Chris Wu
#

TOP_DIR := ..

# Add source file name (without .c) here
OBJ_NAMES=null_process a b  c

OBJS=$(addprefix bin/, $(addsuffix .o, $(OBJ_NAMES)))
COMMON_INC:=rtx.h dbug.h
LOCAL_CFLAGS=

include $(TOP_DIR)/shared/coldfire.mk

bin/null_process.o: $(COMON_INC) null_process.h
bin/a.o: $(COMON_INC) abc.h
bin/b.o: $(COMON_INC) abc.h
bin/c.o: $(COMON_INC) abc.h

# Add an entry above ^^^ where its the form of:
# bin/<filename>.o: <header files the .h depends on>

