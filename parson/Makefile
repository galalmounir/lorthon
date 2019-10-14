CC = gcc
CFLAGS = -O0 -g -Wall -Wextra -std=c89 -pedantic-errors

CPPC = g++
CPPFLAGS = -O0 -g -Wall -Wextra

all: test testcpp libparson.a

.PHONY: test testcpp
test: tests.c parson.c
	$(CC) $(CFLAGS) -o $@ tests.c parson.c
	./$@

testcpp: tests.c parson.c
	$(CPPC) $(CPPFLAGS) -o $@ tests.c parson.c
	./$@

libparson.a: parson.o
	ar rcs $@ $^
	rm parson.o
	mkdir -p ../lib
	mv libparson.a ../lib/libparson.a
parson.o:
	$(CC) -c parson.c
clean:
	rm -f test *.o

