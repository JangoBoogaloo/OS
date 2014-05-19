/**********************************************************************
  Module: protocol.h
  Author: Jim Young
  Date:   2013 Jan 3

  Purpose: provides interface to the message reading / writing protocol.

  Changes: 
    2013 Jan 3 [ Jim Young ]
      - initial version

**********************************************************************/

#ifndef PROTOCOL_H
#define PROTOCOL_H

/* define protocol tokens here */
#define START_BOUNDARY '!'
#define END_BOUNDARY '?'

/* character IDs are just 0-9. server is exception */
#define SERVER_ID_C 'S'
#define SERVER_ID -1

#define MAX_CLIENTS 10

/* string characters */
#define MAX_MESSAGE_SIZE 32 

/* protocol overhead */
/* -start, -end, -cmd, - src*/
#define PROTOCOL_SIZE 4 

/*useable data in packet*/
#define RAW_DATA_SIZE (MAX_MESSAGE_SIZE - PROTOCOL_SIZE)


/* states for message parsing state machine*/
enum STATES {
	START_TAG,
	ID,
	MESSAGE,
	END_TAG
};
typedef enum STATES ProtocolState;


/* different possible messages. (changes must be reflected in 
 character mapping in the .c file */
	
enum MESSAGE {
	REQUEST,
	PRIME_FOUND,
	QUERY,
	UNKNOWN
};
typedef enum MESSAGE Message;


/* A message packet
   protocol: "!#C____________?", 
		! is start char. # is source, C is single-char cmd, ____ is data for some cmds. ? is end */
struct PROTOCOL_DATA {
	ProtocolState currState; /* state for parsing incoming msg */
	Message message;
	char rawData[RAW_DATA_SIZE]; /* used for grabbing data as text before conversion to int */
	int rawDataIndex; /* used for parsing placeholder only */
	int data;
	int sourceID; /* who sent this packet */
	boolean complete; /* is this packet complete? */
};

typedef struct PROTOCOL_DATA ProtocolData;

/********************** FUNCTIONS ********************/

/* set a data packet to the initial valid state */
void initProtocol(ProtocolData *state);

/* convert a character message letter into the enumerated type */
void processProtocol(char byte, ProtocolData *state);

/* create a message string from given type, integer data, and source ID.
   creates it in a local buffer and returns pointer to that buffer. */
char const *createMessage(Message m, int data, int sourceID);

#endif
