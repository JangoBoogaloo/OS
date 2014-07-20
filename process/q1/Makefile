CC = clang
CFLAGS = -Wall -g
PROG = umguo62_q1
OBJS = umguo62_q1.o

$(PROG):$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG)

umguo62_q1.o: umguo62_q1.c
	$(CC) $(CFLAGS) -c umguo62_q1.c -o umguo62_q1.o

clean: 
	rm -f $(PROG) $(OBJS)
