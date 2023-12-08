GCC = gcc -std=c99 -g -Wall -Wshadow --pedantic -Wvla -Werror
VAL = valgrind --tool=memcheck --log-file=vallog --leak-check=full --verbose

SRCS = main.c compression.c
OBJS = $(SRCS:%.c=%.o)

compression: $(OBJS)
	$(GCC) $(OBJS) -o compression

.c.o: 
	$(GCC) -c $*.c 

testall: test1 test2 test3 test4 test5 

test1 test2 test3 test4 test5: compression
	/.compression inputs/test1

testmem: compression
	$(VAL) ./compression inputs/test1

clean: # remove all machine generated files
	rm -f compression compressed* decompressed* *.o output* vallog *~