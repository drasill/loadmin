.PHONY: install clean

all: loadmin

loadmin: loadmin.c
	gcc -o loadmin loadmin.c

prefix=/usr/local

install: loadmin
	install -m 0755 loadmin $(prefix)/bin

clean:
	rm loadmin


