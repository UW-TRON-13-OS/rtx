
LIB_SRC := $(wildcard *.ld)
LIB_DST := $(addprefix ../lib/, $(LIB_SRC))

all: $(LIB_DST)

../lib/%.ld: %.ld
	@mkdir -p ../lib
	@cp -f $< $@

clean:
	@rm -f $(LIB_DST)
