SOURCE_FILES=$(find . -name "*.txt")

all: PHDL_Language_Reference_Manual/PHDL_Language_Reference_Manual.html

%.html: %.txt $(SOURCE_FILES)
	asciidoc $<
