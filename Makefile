DIRECTORIES=documents programs

.PHONY: all
all:
	@for dir in $(DIRECTORIES); do\
		$(MAKE) -C $$dir;\
	done

.PHONY: clean
clean:
	@for dir in $(DIRECTORIES); do\
		$(MAKE) -C $$dir clean;\
	done

