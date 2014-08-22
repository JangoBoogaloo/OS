SRC := main.c
PROG := merge_sort
CFLAGS :=
LDFLAG :=  #e.g. -lpthread

ifdef DEBUG
	CFLAGS += -g
	LDFLAGS += -g
endif
all:
	clang -Wall $(CFLAGS) $(LDFLAG) -o $(PROG) $(SRC)

clean:
	rm -f $(PROG)

run: all
	./$(PROG)

again: clean all run
