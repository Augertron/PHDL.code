INCLUDES=-Iinclude

ICU_LIBS=-licuuc -licui18n
LIBS=$(ICU_LIBS)

COMPILE_BASE=g++ -std=c++11 -fwrapv -pipe -Wall -Werror -Wextra --optimize=2
COMPILE=$(COMPILE_BASE) -c -MMD $(INCLUDES)
LINK=$(COMPILE_BASE) $(LIBS) $(INCLUDES)

MAKELIB=ar rcus

SOURCES=$(shell find implement -name "*.c++")
OBJECTS=$(patsubst implement/%.c++,build/%.o,$(SOURCES))

TEST_SOURCES=$(shell find test -name "*.c++")
TEST_TARGETS=$(patsubst test/%.c++,build/test/%,$(TEST_SOURCES))

all: make_dirs build/libphdl.a $(TEST_TARGETS)

.PHONY: make_dirs
make_dirs:
	@mkdir -p build/phdl/parser
	@mkdir -p build/test/phdl/parser

build/%.o build/%.d: implement/%.c++
	@echo "Compiling $*"
	@$(COMPILE) $< -o build/$*.o
include $(wildcard build/*.d build/test/*.d)

build/libphdl.a: $(OBJECTS)
	@$(MAKELIB) $@ $?

build/test/%: test/%.c++ build/libphdl.a
	@echo "Testing $*"
	@$(LINK) $^ -o $@
	@(cd build/test; ./$* > $*.log)

clean:
	@echo "Cleaning"
	@rm -rf build
