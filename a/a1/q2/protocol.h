#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

#define SERVER_FIFO "primeserver"

typedef enum { false, true } bool;

struct PACKET
{
	char header[3];
	char data[7];
};

void checkError(int ret, const char *errorInfo);

void cleanUp(char *fifo);

void recvPacket(struct PACKET *pPack, char *fifo);

void sendPacket(struct PACKET *pPack, char *fifo);

#endif
