.POSIX:
ALL_WARNING = -Wall -Wextra -pedantic
ALL_LDFLAGS = $(LDFLAGS)
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS) -std=c99 $(ALL_WARNING)
PREFIX = /usr/local
LDLIBS = -lm
BINDIR = $(PREFIX)/bin

all: bc_test
install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	cp -f bc $(DESTDIR)$(BINDIR)
	cp -f bc_test $(DESTDIR)$(BINDIR)
	chmod 755 $(DESTDIR)$(BINDIR)/bc
	chmod 755 $(DESTDIR)$(BINDIR)/bc_test
bc_test: bc_test.o bc.o
	$(CC) $(ALL_LDFLAGS) -o bc_test bc_test.o bc.o $(LDLIBS)
bc.o: bc.c bc.h
	$(CC) $(ALL_CFLAGS) -c bc.c
bc_test.o: bc_test.c bc.h
	$(CC) $(ALL_CFLAGS) -c bc_test.c
clean:
	rm -f bc bc_test *.o
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/bc
	rm -f $(DESTDIR)$(BINDIR)/bc_test
.PHONY: all install uninstall clean
