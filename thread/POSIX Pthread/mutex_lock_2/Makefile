SRC := threadeg3b.c
PROG := threadeg3b
LDFLAG := -lpthread

all:
	clang -Wall $(LDFLAG) -o $(PROG) $(SRC)

clean:
	rm -f $(PROG)

run: all
	./$(PROG)

again: clean all run
