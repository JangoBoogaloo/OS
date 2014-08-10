#include "protocol.h"

static int id = -1;
static char client_fifo[13];
static int client_fd = -1;

void handleSignal(int sig)
{
	if (SIGPIPE == sig)
		printf("SIGPIPE received!\n");
	
	printf("received Signal\n");
	cleanUp(client_fd, client_fifo);
}

void requestPrime()
{
	struct PACKET packet;
	sprintf(packet.header, "!%dR", id);
	sprintf(packet.data, "?");
	bool isSent = sendPacket(&packet, SERVER_FIFO);
	if(!isSent)
		cleanUp(client_fd, client_fifo);
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
	bool isRcvd= false;
	printf("Received Query\n");

	client_fd = open(client_fifo, O_RDONLY);
	isRcvd = recvPacket(&packet, client_fd);
	close(client_fd);

	if(!isRcvd)
		return -1;

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
	int opt = -1;
	
	signal(SIGINT, handleSignal);
	signal(SIGTERM, handleSignal);
	signal(SIGPIPE, handleSignal);

	if ((opt = getopt(argc, argv, "c:")) == -1 || opt == '?')		
	{
		fprintf(stderr, "Usage: %s [-c clientID]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	id = atoi(optarg);

	sprintf(client_fifo, "primeclient%d", id);
	ret = mkfifo(client_fifo, 0666);
	checkError(ret, "FIFO");

	checkError(client_fd, "OPEN FIFO");
	printf("Client %d running\n", id);

	while(true)
	{
		sleep(1);
		requestPrime();
		number = recvQuery();
		if(number != -1 && isPrime(number))
			sendPrime(number);
	}

	cleanUp(client_fd, client_fifo);	
		
	return 0;
}

