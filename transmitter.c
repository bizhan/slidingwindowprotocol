/* File 	: transmitter.c */
#include "header.h"

/* NETWORKS */
int sockfd, port;		// sock file descriptor and port number
struct hostent *server;
struct sockaddr_in receiverAddr;			// receiver host information
int receiverAddrLen = sizeof(receiverAddr);

/* FILE AND BUFFERS */
FILE *tFile;			// file descriptor
char *receiverIP;		// buffer for Host IP address
char buf[BUFMAX];		// buffer for character to send
char xbuf[BUFMAX+1];	// buffer for receiving XON/XOFF characters

/* FLAGS */
int isSocketOpen;	// flag to indicate if connection from socket is done

int main(int argc, char *argv[]) {
	pthread_t thread[1];

	if (argc < 4) {
		// case if arguments are less than specified
		printf("Please use the program with arguments: %s <target-ip> <port> <filename>\n", argv[0]);
		return 0;
	}

	if ((server = gethostbyname(argv[1])) == NULL)
		error("ERROR: Receiver Address is Unknown or Wrong.\n");

	// creating IPv4 data stream socket
	printf("Creating socket to %s Port %s...\n", argv[1], argv[2]);
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		error("ERROR: Create socket failed.\n");

	// flag set to 1 (connection is established)
	isSocketOpen = 1;

	// initializing the socket host information
	memset(&receiverAddr, 0, sizeof(receiverAddr));
	receiverAddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&receiverAddr.sin_addr.s_addr, server->h_length);
	receiverAddr.sin_port = htons(atoi(argv[2]));

	// open the text file
	tFile = fopen(argv[argc-1], "r");
	if (tFile == NULL) 
		error("ERROR: File text not Found.\n");

	if (pthread_create(&thread[0], NULL, childProcess, 0) != 0) 
		error("ERROR: Failed to create thread for child. Please free some space.\n");
	
	// this is the parent process
	// use as char transmitter from the text file
	// connect to receiver, and read the file per character
	int counter = 0;
	char string[128];
	while((buf[0] = fgetc(tFile)) != EOF) {
		MESGB msg = { .soh = SOH, .stx = STX, .etx = ETX, .checksum = 0, .msgno = counter};
		strcpy(msg.data, buf);
		printf("Sending byte no. %d: \'%c\'\n", counter++,msg.data[0]);
		memcpy(string,&msg,sizeof(MESGB));
		if(sendto(sockfd, string, sizeof(MESGB), 0, (const struct sockaddr *) &receiverAddr, receiverAddrLen) != sizeof(MESGB))
			error("ERROR: sendto() sent buffer with size more than expected.\n");
		sleep(DELAY);
	}

	// sending endfile to receiver, marking the end of data transfer
	MESGB msg = { .soh = SOH, .stx = STX, .etx = ETX, .checksum = 0, .msgno = counter};
	strcpy(msg.data, buf);
	memcpy(string,&msg,sizeof(MESGB));
	if(sendto(sockfd, string, sizeof(MESGB), 0, (const struct sockaddr *) &receiverAddr, receiverAddrLen) != sizeof(MESGB))
		error("ERROR: sendto() sent buffer with size more than expected.\n");
	printf("Sending EOF");
	fclose(tFile);
	
	printf("Frame sending done! Closing sockets...\n");
	close(sockfd);
	isSocketOpen = 0;
	printf("Socket Closed!\n");
	return 0;
}

void error(const char *message) {
	perror(message);
	exit(1);
}

void *childProcess(void *threadid) {
	// child process
	// read if there is XON/XOFF sent by receiver using recvfrom()
	struct sockaddr_in srcAddr;
	int srcLen = sizeof(srcAddr);
	char string[128];
	RESPL *rsp = (RESPL *) malloc(sizeof(RESPL));
	while (isSocketOpen) {
		if(recvfrom(sockfd, string, sizeof(RESPL), 0, (struct sockaddr *) &srcAddr, &srcLen) < sizeof(RESPL))
			error("ERROR: Failed to receive frame from socket.\n");
		memcpy(rsp,string,sizeof(RESPL));
	}
	pthread_exit(NULL);
}