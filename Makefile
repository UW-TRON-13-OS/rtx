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

test:
	@for dir in $(APP_SUBDIRS); do \
		make --no-print-directory -C $$dir test; \
	done

clean:
	@for dir in $(APP_SUBDIRS); do \
		make --no-print-directory -C $$dir clean; \
	done
	@rm -rf bin lib tests
