##################################################################################
# vim: noet
#
#

APP_SUBDIRS := kern utest keyboard crt

.PHONY: clean all $(APP_SUBDIRS)

all: $(APP_SUBDIRS)

$(APP_SUBDIRS):
	@echo Make module $@
	@make --no-print-directory -C $@



run: bin/kern
	@echo 
	@bin/kern

bin/kern:
	@make --no-print-directory -C kern

test:
	@chmod 755 make/run_all_tests.sh
	@make/run_all_tests.sh $(APP_SUBDIRS)

clean:
	@for dir in $(APP_SUBDIRS); do \
		make --no-print-directory -C $$dir clean; \
	done
	@rm -rf bin lib tests
