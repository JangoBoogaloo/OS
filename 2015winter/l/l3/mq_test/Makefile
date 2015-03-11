SRC :=mq_test.c
PROG :=mq_test 
CFLAGS :=-g
LDFLAG :=-g -lrt#e.g. -lpthread

all:
	clang -Wall $(LDFLAG) -o $(PROG) $(SRC)

clean:
	rm -f $(PROG)

run: all
	./$(PROG)

again: clean all run
