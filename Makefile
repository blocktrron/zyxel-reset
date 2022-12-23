CC:=gcc
CFLAGS:=
LDFLAGS:=

all: zyxel-reset

zyxel-reset:
	$(CC) $(CFLAGS) $(LDFLAGS) -o zyxel-reset zyxel-reset.c

clean:
	rm -rf zyxel-reset

.PHONY: clean all
