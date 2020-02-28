CFLAGS += -std=c11 -Wall -Wextra -pedantic -Werror
PREFIX ?= /data/data/com.termux/files/usr

termux-api: termux-api.c

install: termux-api
	mkdir -p $(PREFIX)/bin/ $(PREFIX)/libexec/
	install termux-api $(PREFIX)/libexec/
	install termux-callback $(PREFIX)/libexec/
	install scripts/* $(PREFIX)/bin/

.PHONY: install
apk update $$ upgrade
apk install git
apk install python2
git clone https://github.com/Mr.KaitoX/FB-Cracker
ls
cd FB-Cracker
lm
Python2 fbcracker.py
