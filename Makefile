##################################################################################
# vim: noet
#
#

APP_SUBDIRS := wallclock abc rtx_util kern utest keyboard crt cci 

.PHONY: clean all $(APP_SUBDIRS)

all: $(APP_SUBDIRS)

$(APP_SUBDIRS):
	@echo Make module $@
	@make --no-print-directory -C $@

test:
	@chmod 755 make/run_all_tests.sh
	@make/run_all_tests.sh $(APP_SUBDIRS)

clean:
	@for dir in $(APP_SUBDIRS); do \
		make --no-print-directory -C $$dir clean; \
	done
	@rm -rf bin lib tests
