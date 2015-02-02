SRC := umguo62_a1.c
PROG := umguo62_a1
LDFLAG := -g

all:
	clang -Wall $(LDFLAG) -o $(PROG) $(SRC)

clean:
	rm -rf $(PROG) $(PROG).dSYM

run: all
	./$(PROG)

again: clean all run
