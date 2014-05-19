/**********************************************************************
  Module: protocol.c
  Author: Jim Young
  Date:   2013 Jan 3

  Purpose: implements the message reading / writing protocol.

  Changes: 
    2013 Jan 3 [ Jim Young ]
      - initial version

**********************************************************************/
#include <stdlib.h>
#include <stdio.h>

#include "globals.h"
#include "protocol.h"

/* shared message buffer. CAREFUL: only one message at a time for each process!!!!!!!*/
char messageBuffer[MAX_MESSAGE_SIZE+1]; /* 1 for null */

/* valid messages. Order / type MUST match enum in header */
char const * const MESSAGE_CHARS = "RPQ"; 


/* set a data packet to the initial valid state */
void initProtocol(ProtocolData *state) {
	state->currState = START_TAG;
	state->message = UNKNOWN;
	state->rawDataIndex = 0;
	state->complete = false;
} 

/* convert a character message letter into the enumerated type */
Message parseMessageType(char byte) {
	int i = 0;
	Message m = UNKNOWN;
	while (MESSAGE_CHARS[i] != '\0' && m==UNKNOWN) {
		if (MESSAGE_CHARS[i] == byte) 
			m = i;
		i++;
	}
	return m;
}

/* process the protocol: given a state (packet) and the next byte, determine
   the next state */
void processProtocol(char byte, ProtocolData *state) {
	switch (state->currState) {
	
	// 1 skip data until start boundary (should never happen in this case)
	case START_TAG:
		if (byte == START_BOUNDARY)
			state->currState = ID;
	break;

	// 2 grab ID
	case ID:
		if (byte == SERVER_ID_C)
			state->sourceID = SERVER_ID;
		else
			state->sourceID = byte-'0';
		state->currState = MESSAGE;
	break;

	// 3 seek to end of message and store characters
	case MESSAGE:
		state->message = parseMessageType(byte);
		state->currState = END_TAG;
	break;

	// 4 seek to end tag, saving everything passed over as data
	case END_TAG:
		if (byte != END_BOUNDARY) {
			if (state->rawDataIndex < RAW_DATA_SIZE-1)
				state->rawData[state->rawDataIndex++] = byte;
		} else { /* end boundary found, move on */
			state->complete = true;
			state->rawData[state->rawDataIndex] = '\0';
			state->data = atoi(state->rawData);
		}
	break;
	}
}

/* create a message string from given type, integer data, and source ID.
   creates it in a local buffer and returns pointer to that buffer. */
char const *createMessage(Message m, int data, int sourceID) {
	int index = 0;

	messageBuffer[index++] = START_BOUNDARY;

	if (sourceID == SERVER_ID)
		messageBuffer[index++] = SERVER_ID_C;
	else
		messageBuffer[index++] = sourceID+'0';

	messageBuffer[index++] = MESSAGE_CHARS[m]; /* get character for M enumerated type. */

	index += sprintf(&messageBuffer[index], "%d", data); /* should really be using snprintf */
	messageBuffer[index++] = END_BOUNDARY;
	messageBuffer[index] = '\0';
	return messageBuffer;
}

