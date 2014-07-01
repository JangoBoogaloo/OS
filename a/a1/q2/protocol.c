#include "protocol.h"

void checkError(int ret, const char *errorInfo)
{
	if(ret != -1)
		return;
	perror(errorInfo);
}

void cleanUp(char *fifo)
{
	int ret = -1;
	ret = unlink(fifo);
	checkError(ret, "Unlink FIFO");
}

void recvPacket(struct PACKET *pPack, char *fifo)
{
	int ret = -1;
	char buffer[sizeof(struct PACKET)];
	
	int fd = open(fifo, O_RDONLY);
	checkError(fd, "Open Read FIFO");

	ret = read(fd, buffer, sizeof(buffer));
	checkError(ret, "Read PACKET");

	ret = close(fd);
	checkError(ret, "Close Read FIFO");
	sprintf(pPack->header, "%.3s", buffer);
	sprintf(pPack->data, "%.7s", &buffer[3]);

	strncpy(pPack->header, buffer, 3);
	strncpy(pPack->data, &buffer[3], 7);

	printf("Recvd Packet: %.3s%.7s\n", pPack->header, pPack->data);
}

void sendPacket(struct PACKET *pPack, char *fifo)
{
	int ret = -1;
	char buffer[sizeof(struct PACKET)];

	strncpy(buffer, pPack->header, 3);
	strncat(buffer, pPack->data, 7);

	sprintf(buffer, "%.3s%.7s", pPack->header, pPack->data);

	printf("Send Packet: %s\n", buffer);

	int fd = open(fifo, O_WRONLY);
	checkError(fd, "Open Write FIFO");

	ret =	write(fd, buffer, sizeof(buffer));
	checkError(ret, "WRITE PACKET");

	ret = close(fd);
	checkError(ret, "Close Write FIFO");
}
