ICU_LIBS=-licuuc -licui18n
LIBS=$(ICU_LIBS)

COMPILE_BASE=g++ -std=c++11 -Wall -Werror -Wextra
COMPILE=$(COMPILE_BASE) -c -MMD
LINK=$(COMPILE_BASE) $(LIBS)

MAKELIB=ar rcus

SOURCES=$(wildcard *.c++)
OBJECTS=$(patsubst %.c++,%.o,$(SOURCES))

TEST_SOURCES=$(wildcard test/*.c++)
TEST_TARGETS=$(patsubst test/%.c++,test/suite-%,$(TEST_SOURCES))

all: libphdl.a $(TEST_TARGETS)

include $(wildcard *.d test/*.d)

%.o %.d: %.c++
	$(COMPILE) $< -o $*.o

libphdl.a: $(OBJECTS)
	$(MAKELIB) $@ $?

test/suite-%: test/%.c++ libphdl.a
	$(LINK) $^ -o $@
	./$@ > ./$@.log

clean:
	rm -f *.o *.d *.a
	rm -f test/suite-*
	rm -f test/tmp-*
