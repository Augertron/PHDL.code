SOURCES=$(shell find text -maxdepth 1 -name "*.txt")
TARGETS=$(patsubst text/%.txt,build/%.html,$(SOURCES))
SUBDOCS=$(shell find text -name "*.txt")

all: make_dirs $(TARGETS)

.PHONY: make_dirs
make_dirs:
	@mkdir -p build/PHDL_Language_Reference_Manual
	@(cd build; ln -sf ../../highlight/source-highlight/* .)

build/%.html: text/%.txt $(SOURCES) $(SUBDOCS)
	@echo "Formatting $*"
	@(cd build; asciidoc -o $*.html ../$<)

.PHONY: clean
clean:
	@echo "Cleaning"
	@rm -rf build
