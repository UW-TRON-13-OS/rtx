
all:
	@make -C user
	@make -C kern

clean:
	@rm -rf bin lib *.o *.lst *.map *.s19 *.bin
	@make -C user clean
	@make -C kern clean
