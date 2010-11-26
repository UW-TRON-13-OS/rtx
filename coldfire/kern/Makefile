# Makefile
# David Grant, 2004
# Irene Huang, 2010/05/03 

TOP_DIR := ..

# Add source file name (without .c) here
OBJ_NAMES=primitives dbug k_init k_primitives k_scheduler k_process \
		  k_malloc

OBJS=$(addprefix bin/, $(addsuffix .o, $(OBJ_NAMES)))
COMMON_INC:=rtx.h k_globals.h k_structs.h dbug.h

all: $(OBJS)

bin/primitives.o: $(COMMON_INC) trap_table.h 
bin/dbug.o: coldfire.h dbug.h 
bin/k_init.o: $(COMMON_INC)
bin/k_primitives.o: $(COMMON_INC)
bin/k_scheduler.o: $(COMMON_INC)
bin/k_process.o: $(COMMON_INC)
bin/k_malloc.o: memory_layout.h k_globals.h

# Add an entry above ^^^ where its the form of:
# bin/<filename>.o: <header files the .h depends on>

include $(TOP_DIR)/make/coldfire.mk
