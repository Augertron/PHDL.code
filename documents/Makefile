SOURCES=$(shell find text -maxdepth 2 -name "*.txt")
TARGETS=$(patsubst text/%.txt,build/%.html,$(SOURCES))

all: make_dirs $(TARGETS)

.PHONY: make_dirs
make_dirs:
	@mkdir -p build/PHDL_Language_Reference_Manual
	@ln -sf ../../highlight/source-highlight/lang.map  build/lang.map
	@ln -sf ../../highlight/source-highlight/phdl.lang build/phdl.lang

build/%.html: text/%.txt $(SOURCES)
	@echo "Asciidocing $*"
	(cd build; asciidoc -o $*.html ../$<)

.PHONY: clean
clean:
	@echo "Cleaning"
	@rm -rf build
