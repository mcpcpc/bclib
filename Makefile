.POSIX:
ALL_WARNING = -Wall -Wextra -pedantic
ALL_LDFLAGS = $(LDFLAGS)
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS) -std=c99 $(ALL_WARNING)
PREFIX = /usr/local
LDLIBS = -lm
BINDIR = $(PREFIX)/bin

all: bc
install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	cp -f bc $(DESTDIR)$(BINDIR)
	chmod 755 $(DESTDIR)$(BINDIR)/bc
bc: bc.o
	$(CC) $(ALL_LDFLAGS) -o bc bc.o $(LDLIBS)
bc.o: bc.c
	$(CC) $(ALL_CFLAGS) -c bc.c
clean:
	rm -f bc *.o
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/bc
.PHONY: all install uninstall clean
