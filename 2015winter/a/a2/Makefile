SRC := umguo62_a2.c
PROG := umguo62_a2
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
