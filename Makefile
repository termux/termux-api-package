PREFIX ?= /data/data/com.termux/files/usr
CFLAGS += -std=c11 -Wall -Wextra -pedantic -Werror -DPREFIX=\"$(PREFIX)\"

termux-api: termux-api.c

install: termux-api
	mkdir -p $(PREFIX)/bin/ $(PREFIX)/libexec/
	install termux-api $(PREFIX)/libexec/
	install termux-callback $(PREFIX)/libexec/
	cd scripts; for i in *; do \
	  sed -e "s|@TERMUX_PREFIX@|$(PREFIX)|g" $$i > $(PREFIX)/bin/$$i; \
	  chmod 700 $(PREFIX)/bin/$$i; \
	done

.PHONY: install
