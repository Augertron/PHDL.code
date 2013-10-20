QT_INCLUDE_DIR=/usr/include/qt5
QT_INCLUDES=-I$(QT_INCLUDE_DIR)
QT_LIBRARIES=-lQt5Core

ICU_LIBRARIES=-licuuc -licui18n

BOOST_LIBRARIES=-lboost_locale

INCLUDES=-Isource $(QT_INCLUDES)
LIBRARIES=$(ICU_LIBRARIES) $(QT_LIBRARIES) $(BOOST_LIBRARIES)

COMPILE_BASE=g++ -std=c++11 -fwrapv -fPIC -pipe -pthread -Wall -Werror -Wextra --optimize=2
COMPILE=$(COMPILE_BASE) -c -MMD -MP $(INCLUDES)
LINK=$(COMPILE_BASE) $(LIBRARIES) $(INCLUDES)

MAKELIB=ar rcs

SOURCES=$(shell find source -name "*.c++" | grep -v ".test.c++")
OBJECTS=$(patsubst source/%.c++,build/object/%.o,$(SOURCES))

TEST_SOURCES=$(shell find source -name "*.test.c++")
TEST_TARGETS=$(patsubst source/%.test.c++,build/test/%,$(TEST_SOURCES))

.PHONY: all
all: make_dirs build/libphdl.a $(TEST_TARGETS)

.PHONY: make_dirs
make_dirs:
	@for type in object test; do\
		for dir in error parser unicode; do\
			mkdir -p build/$$type/phdl/$$dir;\
		done;\
	done

build/object/%.o build/object/%.d: source/%.c++
	@echo "Compiling $*"
	@$(COMPILE) $< -o build/object/$*.o
-include $(shell find build -name "*.d" 2> /dev/null)

build/libphdl.a: $(OBJECTS)
	@echo "Constructing libphdl"
	@$(MAKELIB) $@ $^

build/test/%: source/%.test.c++ build/libphdl.a
	@echo "Testing $*"
	@$(LINK) $^ -o $@
	@(cd build/test; ./$* > $*.log || rm ./$*)

.PHONY: clean
clean:
	@echo "Cleaning"
	@rm -rf build
