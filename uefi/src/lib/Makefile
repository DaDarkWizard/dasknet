
all: stdlib.o stdio.o

stdlib.o: stdlib.c
	$(GCC) $(CFLAGS) -o stdlib.o stdlib.c

stdio.o: stdio.c
	$(GCC) $(CFLAGS) -o stdio.o stdio.c

test: test.c all
	gcc -O0 test.c stdlib.c stdarg.c -o test