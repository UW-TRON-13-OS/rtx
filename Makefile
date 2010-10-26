##################################################################################
# vim: noet
#
#

APP_SUBDIRS := kern

.PHONY: clean all $(APP_SUBDIRS)

all: $(APP_SUBDIRS)

$(APP_SUBDIRS):
	@echo Make module $@
	@make --no-print-directory -C $@

clean:
	@for dir in $(APP_SUBDIRS); do \
		make --no-print-directory -C $$dir clean; \
	done
	@rm -rf bin lib test
