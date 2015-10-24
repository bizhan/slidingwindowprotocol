/* 
 * File : dcomm.h 
 */ 

#ifndef _RECEIVER_H_ 
#define _RECEIVER_H_ 

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

/* ASCII Const */ 
#define SOH 1 /* Start of Header Character */
#define STX 2 /* Start of Text Character */
#define ETX 3 /* End of Text Character */ 
#define ENQ 5 /* Enquiry Character */ 
#define ACK 6 /* Acknowledgement */ 
#define BEL 7 /* Message Error Warning */ 
#define CR 13 /* Carriage Return */
#define LF 10 /* Line Feed */ 
#define NAK 21 /* Negative Acknowledgement */
#define Endfile 26 /* End of file character */
#define ESC 27 /* ESC key */ 

/* Const */ 
#define BYTESIZE 256 /* The maximum value of a byte */
#define MAXLEN 1024 /* Maximum messages length */ 
#define DELAY 1

#define bzero(p, size) (void)memset((p), 0 , (size))

/* Define receive buffer size */
#define WINDOWSIZE 7
#define BUFMAX 1	/* Maximum size of buffer that can be sent */


typedef unsigned char Byte;

/* FRAMES AND WINDOW */

typedef struct MESGB {
 	unsigned int soh;
 	unsigned int stx;
 	unsigned int etx;
 	Byte checksum;
 	Byte msgno;
 	char data[1];
} MESGB;

typedef struct QTYPE {
 	unsigned int count, front, rear, maxsize;
 	MESGB *window;
} QTYPE;

//////////////////////

/* ACKNOWLEDGMENTS */

typedef struct RESPL {
	unsigned int ack;
	Byte msgno;
	Byte checksum;
} RESPL;

typedef struct QTemp {
// used for save sent frame msgno
	unsigned int count, front, rear, maxsize; 
	RESPL *tab; 
} QTemp;

/////////////////////

/* FUNCTIONS AND PROCEDURES */
void *childRProcess(void * threadid);
void error(const char* message);

#endif