COMPILE=g++ -std=c++11 -Wall -Werror -Wextra -licuuc -licui18n

all: run_test

run_test:
	for test in test/*.c++; do export TARGET=test/suite-$$(basename $$test .c++); echo "Test suite $$TARGET"; $(COMPILE) $$test $(wildcard *.c++) -o $$TARGET && ./$$TARGET > $$TARGET.log; done
