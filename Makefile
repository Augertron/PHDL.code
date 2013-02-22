all:
	$(MAKE) -C documents
	$(MAKE) -C source

clean:
	$(MAKE) -C documents clean
	$(MAKE) -C source    clean
