# Makefile
# Author: Chris Wu
#

TOP_DIR = ..
MAIN_FILE =

# Add source file name (without .c) here
OBJ_NAMES=utest utils

OBJS=$(addprefix bin/, $(addsuffix .o, $(OBJ_NAMES)))
TEST_FILES=

#all: $(OBJS)
include $(TOP_DIR)/shared/coldfire.mk

bin/utest.o: utest.h 
bin/utils.o: utils.h

# Add an entry above ^^^ where its the form of:
# bin/<filename>.o: <header files the .h depends on>

