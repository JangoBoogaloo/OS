#ifndef PROTOCOL_H
#define PROTOCOL_H

#define BUFSIZE 81
#define START_SYMBOL '!'
#define END_SYMBOL '?'

#define REQUEST_SYMBOL 'R'
#define POST_SYMBOL 'P'
#define QUERY_SYMBOL 'Q'
#define SERVER_ID 'S'
#define SERVER_FIFO "prime_server"
#define CLIENT_FIFO "prime_client"

typedef enum BOOL bool;
enum BOOL
{
	false,
	true
};

typedef enum STATE State;
enum STATE
{
	START,
	ID,
	MSG,
	END,	
};

typedef enum MESSAGE Message;
enum MESSAGE
{
	REQUEST, 
	POST,
	QUERY,
	UNKNOWN	
};

typedef struct PROTOCOL Protocol;
struct PROTOCOL
{
	State current_state;
	int id;	
	char *data_buffer[28];
	int buffer_index;
	Message msg;
	int data;
	char end;
	bool ready;
};

void create_msg(Message msg, int data, int id);
void init_protocol(Protocol *ptc_data);
Message parse_message(char byte);
void load_protocol(char byte, Protocol *ptc_data);
void create_msg(Message msg, int data, int id);



#endif
