SRC :=handler.c
PROG :=handler
CFLAGS :=-g
LDFLAG :=-g  

all:
	clang -Wall $(LDFLAG) -o $(PROG) $(SRC)

clean:
	rm -f $(PROG)

run: all
	./$(PROG)

again: clean all run
