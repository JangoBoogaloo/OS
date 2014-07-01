#include "protocol.h"

static char client_fifo[13];
static int maxPrime = -1;
static int checkNumber = 0;
static sig_atomic_t isRunning = false;

void handleSigTerm(int sig)
{
	printf("terminate program\n");
	if(SIGINT == sig)
		isRunning = false;
}

void sendQuery(struct PACKET *pPack)
{
	int client_id;
	checkNumber++;
	sscanf(pPack->header, "!%dR", &client_id);
	sprintf(client_fifo, "primeclient%d", client_id);

	struct PACKET packet;
	sprintf(packet.header, "!#Q");
	sprintf(packet.data, "%d?", checkNumber);
	sendPacket(&packet, client_fifo);
}

void recordPrime(struct PACKET *pPack)
{
	int newPrime =-1;
	sscanf(pPack->data, "%d?", &newPrime);
	printf("new Prime %d\n", newPrime);
	if(newPrime >maxPrime)
	{
		maxPrime = newPrime;
		printf("Current max prime: %d\n", maxPrime);
	}
}

int main(int argc, char **argv)
{

	isRunning = true;
	int ret = -1;

	signal(SIGINT, handleSigTerm);

	ret = mkfifo(SERVER_FIFO, 0666);
	checkError(ret, "FIFO");
	struct PACKET packet;
	while(isRunning && checkNumber <5000000)
	{
		sleep(1);
		recvPacket(&packet, SERVER_FIFO);
		switch(packet.header[2])
		{
			case 'R':
				sendQuery(&packet);	
				break;
			case 'P':
				recordPrime(&packet);			
				break;
			default:
				printf("invalid packet: %.3s%.7s\n", 
						packet.header, packet.data);
		}
	}
	
	cleanUp(SERVER_FIFO);

	return 0;
}

