CC=clang
CPP=clang++
CFLAGS=-ldeepstate_LF -fsanitize=fuzzer,address
CFLAGS_DS=-ldeepstate
CPPFLAGS=$(CFLAGS)
CPPFLAGS_DS=-ldeepstate
c_printf.o: c_printf/sources/c_printf.c
	$(CC) -c $^ $(CFLAGS) -o $@

c_printf.oa: c_printf/sources/c_printf.c
	$(CC) -c $^ $(CFLAGS_DS) -o $@

main: Test.cpp c_printf.o
	$(CPP) $^ $(CPPFLAGS) -o $@

main_alt: Test.cpp c_printf.oa
	$(CPP) $^ $(CPPFLAGS_DS) -o $@
clean:
	rm *.o main main2
