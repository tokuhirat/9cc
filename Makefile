CFLAGS=-std=c11 -g -static

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

TEST_SRCS=$(wildcard test/*.c)
TESTS=$(TEST_SRCS:.c=.exe)

9cc: $(OBJS)
		$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJS): 9cc.h

test/%.exe: 9cc test/%.c
		$(CC) -o- -E -P -C test/$*.c | ./9cc -o test/$*.s -
		$(CC) -o $@ test/$*.s -xc test/common

test: $(TESTS)
		for i in $^; do echo $$i; ./$$i || exit 1; echo; done
		test/driver.sh

clean:
		rm -rf chibicc tmp* $(TESTS) test/*.s test/*.exe
		find * -type f '(' -name '*~' -o -name '*.o' ')' -exec rm {} ';'

.PHONY: test clean
