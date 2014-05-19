#include <stdio.h>
#include <stdlib.h>
#include "protocol.h"

char message_buffer[33];

void init_protocol(Protocol *ptc_data)
{
	ptc_data -> current_state = START;
	ptc_data -> msg = UNKNOWN;
	ptc_data -> buffer_index = 0;
	ptc_data -> ready = false;
}

Message parse_message(char byte)
{
	switch(byte)
	{
		case REQUEST_SYMBOL:
			return REQUEST;
		case POST_SYMBOL:
			return POST;
		case QUERY_SYMBOL:
			return QUERY;
	}
	printf("Can not convert message");
	return NULL;
}

void load_protocol(char byte, Protocol *ptc_data)
{
	switch(ptc_data->current_state)
	{	
		case START:
			if(byte == START_SYMBOL)
				ptc_data -> current_state = ID;
			break;
		case ID:
			if(byte == SERVER_ID)
				ptc_data -> id = SERVER_ID; 
			else
				ptc_data -> id = byte - '0';
			ptc_data -> current_state = MSG;
			break;
		case MSG:
			ptc_data -> msg = parse_message(byte);
			ptc_data -> current_state = END;
			break;
		case END:
			if(byte != END_SYMBOL)
				if(ptc_data -> buffer_index < 27)
					ptc_data ->data_buffer[ptc_data->buffer_index++] = byte;
			else
			{
				ptc_data -> ready = true;
				ptc_data -> data_buffer[ ptc_data -> buffer_index] = '\0';
				ptc_data -> data = atoi(ptc_data -> databuffer);
			}
			break;
	]
}

void create_msg(Message msg, int data, int id)
{
	message_buffer[0] = START_SYMBOL;
	if(id == SERVER_ID)
		message_buffer[1] = SERVER_ID;
	else
		message_buffer[1] = id + '0';
	switch(msg)
	{
		case REQUEST:
			message_buffer[2] = REQUEST_SYMBOL;
			break;
		case QUERY:
			message_buffer[2] = QUERY_SYMBOL;
			break;
		case POST:
			message_buffer[2] = POST_SYMBOL;
			break;
		default:
			printf("Received invalid message\n");
			break;
	}

	int index = 3+sprintf(&message_buffer[3], "%d", data);
	message_buffer[index++] = END_SYMBOL;
	message_buffer[index] = '\0';
	return message_buffer;
}
