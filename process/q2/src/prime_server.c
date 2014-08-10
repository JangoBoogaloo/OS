#include "protocol.h"

static char client_fifo[13];
static int server_fd;
static int maxPrime = -1;
static int checkNumber = 0;

void handleSignal(int sig)
{
	if (SIGPIPE == sig)
		printf("SIGPIPE received!\n");

	cleanUp(server_fd, SERVER_FIFO);
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

	int ret = -1;

	signal(SIGINT, handleSignal);
	signal(SIGTERM, handleSignal);
	signal(SIGPIPE, handleSignal);

	ret = mkfifo(SERVER_FIFO, 0666);
	checkError(ret, "make FIFO");

	server_fd = open(SERVER_FIFO, O_RDWR);
	checkError(ret, "open FIFO");

	struct PACKET packet;
	while(checkNumber <5000000)
	{
		sleep(1);
		recvPacket(&packet, server_fd);
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
	
	cleanUp(server_fd, SERVER_FIFO);
	return 0;
}
