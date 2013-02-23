all:
	$(MAKE) -C documents
	$(MAKE) -C libphdl

clean:
	$(MAKE) -C documents clean
	$(MAKE) -C libphdl   clean
