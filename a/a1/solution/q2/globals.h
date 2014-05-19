/**********************************************************************
  Module: globals.h
  Author: Jim Young
  Date:   2013 Jan 3

  Purpose: provides common constants between the server and client.

  Changes: 
    2013 Jan 3 [ Jim Young ]
      - initial version

**********************************************************************/

#ifndef GLOBALS_H
#define GLOBALS_H

#define SERVER_FIFO "primeserver"
#define CLIENT_FIFO "primeclient"
#define MAX_FILE_SIZE 32

// booleans
typedef enum
{
  false,
  true
} boolean;

#endif
