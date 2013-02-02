COMPILE=g++ -std=c++11 -Wall -Werror -Wextra -licuuc -licui18n

all: phdl.o run_test

phdl.o: $(wildcard *.?++)
	$(COMPILE) $(wildcard *.c++) -c -o $@

run_test:
	for test in test/*.c++; do export TARGET=test/suite-$$(basename $$test .c++); echo "Test suite $$TARGET"; $(COMPILE) $$test phdl.o -o $$TARGET && ./$$TARGET > $$TARGET.log; done
