QT_INCLUDE_DIR=/usr/include/qt4
QT_INCLUDES=-I$(QT_INCLUDE_DIR)
QT_LIBRARIES=-lQtCore

ICU_LIBRARIES=-licuuc -licui18n

INCLUDES=-Iinclude $(QT_INCLUDES)
LIBRARIES=$(ICU_LIBRARIES) $(QT_LIBRARIES)

COMPILE_BASE=g++ -std=c++11 -fwrapv -pipe -pthread -Wall -Werror -Wextra --optimize=2
COMPILE=$(COMPILE_BASE) -c -MMD -MP $(INCLUDES)
LINK=$(COMPILE_BASE) $(LIBRARIES) $(INCLUDES)

MAKELIB=ar rcus

SOURCES=$(shell find implement -name "*.c++")
OBJECTS=$(patsubst implement/%.c++,build/%.o,$(SOURCES))

TEST_SOURCES=$(shell find test -name "*.c++")
TEST_TARGETS=$(patsubst test/%.c++,build/test/%,$(TEST_SOURCES))

all: make_dirs build/libphdl.a $(TEST_TARGETS)

.PHONY: make_dirs
make_dirs:
	@for ns in parser unicode; do\
		mkdir -p build/phdl/$$ns;\
		mkdir -p build/test/phdl/$$ns;\
	done

build/%.o build/%.d: implement/%.c++
	@echo "Compiling $*"
	@$(COMPILE) $< -o build/$*.o
-include $(shell find build -name "*.d")

build/libphdl.a: $(OBJECTS)
	@$(MAKELIB) $@ $?

build/test/%: test/%.c++ build/libphdl.a
	@echo "Testing $*"
	@$(LINK) $^ -o $@
	@(cd build/test; ./$* > $*.log)

.PHONY: clean
clean:
	@echo "Cleaning"
	@rm -rf build
