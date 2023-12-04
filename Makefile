GCC = gcc -std=c99 -g -Wall -Wshadow --pedantic -Wvla -Werror
VAL = valgrind --tool=memcheck --log-file=vallog --leak-check=full --verbose

SRCS = main.c sudoku.c
OBJS = $(SRCS:%.c=%.o)

sudoku: $(OBJS)
	$(GCC) $(OBJS) -o sudoku

.c.o: 
	$(GCC) -c $*.c 

testall: test1 test2 test3 test4 test5 

test1 test2 test3 test4 test5: sudoku
	./sudoku inputs/$@ > output$(subst test,,$@)
	diff output$(subst test,,$@) expected/expected$(subst test,,$@)

testmem: sudoku
	$(VAL) ./sudoku inputs/test1

clean: # remove all machine generated files
	rm -f sudoku *.o output* vallog *~