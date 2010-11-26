# Makefile
# David Grant, 2004
# Irene Huang, 2010/05/03 

TOP_DIR := ..

OBJ_NAMES=primitives dbug k_init k_primitives k_scheduler k_process
OBJS=$(addprefix bin/, $(addsuffix .o, $(OBJ_NAMES)))

all: $(OBJS)

bin/primitives.o: trap_table.h
bin/dbug.o: dbug.h
bin/k_init.o: k_globals.h
k_primitives.o: k_globals.h
k_scheduler.c: k_globals.h
k_process.c: k_globals.h

include $(TOP_DIR)/make/coldfire.mk
