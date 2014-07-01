#include "protocol.h"

static int id = -1;
static char client_fifo[13];
static sig_atomic_t isRunning = false;

void handleSigTerm(int sig)
{

	printf("terminate program\n");
	if(SIGINT == sig)
		isRunning = false;
}

void requestPrime()
{
	struct PACKET packet;
	sprintf(packet.header, "!%dR", id);
	sprintf(packet.data, "?");
	sendPacket(&packet, SERVER_FIFO);
}

void sendPrime(int number)
{
	struct PACKET packet;
	printf("Send Prime\n");
	sprintf(packet.header, "!%dP", id);
	sprintf(packet.data, "%d?", number);
	
	sendPacket(&packet, SERVER_FIFO);
}

int recvQuery()
{
	struct PACKET packet;
	printf("Received Query\n");
	recvPacket(&packet, client_fifo);
	char sender;
	int number = -1;

	if(packet.header[2] != 'Q')
	{
		printf("Packet not Query: %s", packet.header);
		return -1;
	}
	sscanf(packet.header, "!%cQ", &sender);
	if(sender != '#')
	{
		printf("Wrong Packet Sender: %c", sender);
		return -1;
	}

	if(sscanf(packet.data, "%d?", &number) != 1)
	{
		printf("Fail to convert data: %s", packet.data);
		return -1;
	}

	return number;
}

bool isPrime(int number)
{
	int i;
	if(number <= 1)
		return false;

	for(i=2; i<number; i++)
	{
		if(number%i ==0)
			return false;
	}

	return true;
}

int main(int argc, char **argv)
{
	id = 0;
	int ret = -1;
	int number = -1;
	isRunning = true;
	
	signal(SIGINT, handleSigTerm);

	sprintf(client_fifo, "primeclient%d", id);
	ret = mkfifo(client_fifo, 0666);
	checkError(ret, "FIFO");
	
	printf("Client %d running\n", id);

	while(isRunning)
	{
		sleep(1);
		requestPrime();
		number = recvQuery();
		if(number != -1 && isPrime(number))
			sendPrime(number);
	}

	cleanUp(client_fifo);	
		
	return 0;
}

