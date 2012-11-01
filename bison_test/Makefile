all: rpcalc.exe

rpcalc.exe: rpcalc_lex.c rpcalc.tab.c
	cc -lm -o rpcalc rpcalc_lex.c rpcalc.tab.c

%.tab.c: %.y
	bison -d $<

clean: 
	rm *.tab.*; rm *.exe

