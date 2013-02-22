ICU_LIBS=-licuuc -licui18n
LIBS=$(ICU_LIBS)

COMPILE_BASE=g++ -std=c++11 -Wall -Werror -Wextra
COMPILE=$(COMPILE_BASE) -c -MMD
LINK=$(COMPILE_BASE) $(LIBS)

MAKELIB=ar rcus

SOURCES=$(wildcard *.c++)
OBJECTS=$(patsubst %.c++,build/%.o,$(SOURCES))

TEST_SOURCES=$(wildcard test/*.c++)
TEST_TARGETS=$(patsubst test/%.c++,build/test/suite-%,$(TEST_SOURCES))

all: make_dirs build/libphdl.a $(TEST_TARGETS)

.PHONY: make_dirs
make_dirs:
	@mkdir -p build/test

build/%.o build/%.d: %.c++
	@echo "Compiling $*"
	@$(COMPILE) $< -o build/$*.o
include $(wildcard build/*.d build/test/*.d)

build/libphdl.a: $(OBJECTS)
	@$(MAKELIB) $@ $?

build/test/suite-%: test/%.c++ build/libphdl.a
	@echo "Testing $*"
	@$(LINK) $^ -o $@
	@./$@ > ./$@.log

clean:
	@echo "Cleaning"
	@rm -rf build
	@rm -rf build
