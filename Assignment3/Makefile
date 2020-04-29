CC=clang
CPP=clang++
CFLAGS=-ldeepstate
CFLAGS_DS=-ldeepstate_LF -fsanitize=fuzzer,address   #-lutf8
CPPFLAGS=$(CFLAGS_DS) -g #-lutf8 -g
CPPFLAGS_DS=-ldeepstate -fprofile-instr-generate -fcoverage-mapping -g #-lutf8 -g
LINKER_OPTS=

all: fuzzingwins symexwins
.PHONY: all

fuzzingwins: fuzzingwins.cpp fuzzgoat.o
	clang++ $(LINKER_OPTS) $^  $(CPPFLAGS) -o $@

fuzzgoat.o: fuzzgoat.c
	clang $^ -c -g -o $@

symexwins: symexwins.cpp
	clang++ $^ $(CFLAGS) -o $@ -g

clean:
	rm fuzzingwins symexwins *.o
