#include "protocol.h"

void checkError(int ret, const char *errorInfo)
{
	if(ret != -1)
		return;
	perror(errorInfo);
}

void cleanUp(int fd, char *fifo)
{
	int ret = -1;
	printf("closing fd\n");
	ret = close(fd);
	checkError(ret, "close FD");

	ret = unlink(fifo);
	checkError(ret, "Unlink FIFO");
	exit(EXIT_SUCCESS);
}

bool recvPacket(struct PACKET *pPack, int fd)
{
	int ret = -1;
	char buffer[sizeof(struct PACKET)];

	ret = read(fd, buffer, sizeof(buffer));
	checkError(ret, "Read PACKET");
	if(ret <0)
		return false;
	sprintf(pPack->header, "%.3s", buffer);
	sprintf(pPack->data, "%.7s", &buffer[3]);

	strncpy(pPack->header, buffer, 3);
	strncpy(pPack->data, &buffer[3], 7);

	printf("Recvd Packet: %.3s%.7s\n", pPack->header, pPack->data);
	return true;
}

bool sendPacket(struct PACKET *pPack, char *fifo)
{
	int ret = -1;
	char buffer[sizeof(struct PACKET)];

	strncpy(buffer, pPack->header, 3);
	strncat(buffer, pPack->data, 7);

	sprintf(buffer, "%.3s%.7s", pPack->header, pPack->data);

	printf("Send Packet: %s\n", buffer);

	int fd = open(fifo, O_WRONLY);
	checkError(fd, "Open Write FIFO");
	if(fd <0)
		return false;

	ret =	write(fd, buffer, sizeof(buffer));
	checkError(ret, "WRITE PACKET");

	ret = close(fd);
	checkError(ret, "Close Write FIFO");
	return true;
}
