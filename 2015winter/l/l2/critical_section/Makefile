SRC := threadeg4.c
PROG := threadeg4
CFLAGS :=
LDFLAG :=  -lpthread

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
