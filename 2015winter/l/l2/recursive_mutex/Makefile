SRC := mutex_recursive.c
PROG := mutexRecursive
CFLAGS :=
LDFLAG := -lpthread

ifdef DEBUG
	CFLAGS += -g
	LDFLAGS += -g
endif

all:
	clang -Wall $(LDFLAG) -o $(PROG) $(SRC)

clean:
	rm -f $(PROG)

run: all
	./$(PROG)

again: clean all run
