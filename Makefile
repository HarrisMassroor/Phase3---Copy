CC = gcc
CFLAGS = -std=gnu90 -Wall -Wextra -Wpedantic

UNAME := $(shell uname -s 2>/dev/null)

ifneq ($(OS),Windows_NT)
ifneq ($(findstring MINGW,$(UNAME)),MINGW)
ifneq ($(findstring MSYS,$(UNAME)),MSYS)
WINDOWS_BUILD = 0
else
WINDOWS_BUILD = 1
endif
else
WINDOWS_BUILD = 1
endif
else
WINDOWS_BUILD = 1
endif

PARTC_OBJECTS = list_pool.o list_adders.o list_movers.o \
                list_removers.o

.PHONY: all linux windows clean

all:
	@if [ "$(WINDOWS_BUILD)" = "1" ]; then \
		$(MAKE) windows; \
	else \
		$(MAKE) linux; \
	fi

linux: A2 A3 A4 liblist.a mytestlist-Linuxx86_64
	@if [ -f testlist.c ]; then $(MAKE) testlist-Linuxx86_64; fi

windows:
	@if [ "$(OS)" != "Windows_NT" ] && \
		case "$(UNAME)" in MINGW*|MSYS*|CYGWIN*) false;; *) true;; esac; then \
		echo "Use MSYS2/MinGW to build A1."; exit 1; \
	fi
	$(CC) $(CFLAGS) A1.c square.c -o A1.exe

A2: A2.c square.c
	$(CC) $(CFLAGS) -pthread A2.c square.c -o $@

A3: A3.c square.c
	$(CC) $(CFLAGS) -pthread A3.c square.c -o $@

A4: A4.c square.c
	$(CC) $(CFLAGS) A4.c square.c -o $@

liblist.a: $(PARTC_OBJECTS)
	ar rcs $@ $^

mytestlist-Linuxx86_64: mytestlist.c liblist.a
	$(CC) $(CFLAGS) mytestlist.c -L. -llist -o $@

testlist-Linuxx86_64: testlist.c liblist.a
	$(CC) $(CFLAGS) testlist.c -L. -llist -o $@

%.o: %.c list.h list_internal.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f A1.exe A2 A3 A4 *.o *.a \
		mytestlist-Linuxx86_64 testlist-Linuxx86_64