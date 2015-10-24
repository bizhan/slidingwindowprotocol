/* File : receiver.c */
#include "header.h"

MESGB *rcvframe(int sockfd, QTYPE *queue);
MESGB *q_get(QTYPE *queue);

MESGB rxbuf[WINDOWSIZE];
QTYPE rcvq = { 0, 0, -1, WINDOWSIZE, rxbuf };
QTYPE *rxq = &rcvq;
int endFileReceived;

/* Socket */
int sockfd; // listen on sock_fd
struct sockaddr_in adhost;
struct sockaddr_in srcAddr;
int srcLen = sizeof(srcAddr);

int main(int argc, char *argv[])
{
 	pthread_t thread[1];

 	if (argc<2) {
 		// case if arguments are less than specified
 		printf("Please use the program with arguments: %s <port>\n", argv[0]);
 		return 0;
 	}

 	printf("Creating socket to self in Port %s...\n", argv[1]);
 	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
 		printf("ERROR: Create socket failed.\n");

 	bzero((char*) &adhost, sizeof(adhost));
 	adhost.sin_family = AF_INET;
 	adhost.sin_port = htons(atoi(argv[1]));
 	adhost.sin_addr.s_addr = INADDR_ANY;

 	if(bind(sockfd, (struct sockaddr*) &adhost, sizeof(adhost)) < 0)
 		error("ERROR: Binding failed.\n");

 	endFileReceived = 0;

 	memset(rxbuf, 0, sizeof(rxbuf));
 	
 	/* Create child process */
 	if(pthread_create(&thread[0], NULL, childRProcess, 0)) 
 		error("ERROR: Failed to create thread for child.\n");

	/* parent process: unlimited looping */
	MESGB *c;
	while(1) {
 		c = rcvframe(sockfd, rxq);
 		printf("this %d : %s\n", rxq->window[rxq->rear].msgno, rxq->window[rxq->rear].data);
 		if(c->data[0] == EOF){
 			endFileReceived = 1;
 			printf("File received.");
 		}
	}
	printf("Receiving EOF\n");
	return 0;
}

void error(const char *message) {
	perror(message);
	exit(1);
}

MESGB *rcvframe(int sockfd, QTYPE *queue) {
 	char string[128];
 	MESGB *pmsg =(MESGB *) malloc(sizeof(MESGB));
 	if(recvfrom(sockfd, string, sizeof(MESGB), 0, (struct sockaddr *) &srcAddr, &srcLen) < sizeof(MESGB))
 		error("ERROR: Failed to receive frame from socket\n");
 	memcpy(pmsg,string,sizeof(MESGB));

 	//receive MESGB

 	//adding frame to buffer and resync the buffer queue
 	if (queue->count < 8) {
 		printf("qrear : %d qcount : %d\n", queue->rear, queue->count);
 		//queue->rear = (queue->count > 0) ? (++queue->rear) % 8 : queue->rear;
 		queue->window[++queue->rear] = *pmsg;
 		if(queue->rear == WINDOWSIZE) queue->rear = 0;
 		queue->count++;
 	}
 	return pmsg;
}

void *childRProcess(void *threadid) {
 	MESGB *current;
 	while(1) {
 		current = q_get(rxq);
 		sleep(DELAY);
 	}
 	pthread_exit(NULL);
}

MESGB *q_get(QTYPE *queue) {
/* q_get returns a pointer to the buffer where data is read or NULL if buffer is empty. */
 	MESGB *current = (MESGB *) malloc(sizeof(MESGB));
 	if(queue->count > 0) {
 		char string[128];
 		RESPL rsp;
 		*current = queue->window[queue->front];
 		printf("ack current msgno %d data %s soh %d,%d,%d\n", current->msgno, current->data, current->soh, current->stx, current->etx);
 		if(current->soh==SOH && current->stx==STX && current->etx==ETX && sizeof(current->data)==BUFMAX
 			/*DISINI TAMBAHIN CHECKSUM*/ ) {
 			//RESPL rsp = {ACK, current->msgno, 0};
 			rsp.ack = ACK;
 			rsp.msgno = current->msgno;
 			rsp.checksum = 0;
 			printf("nih %d :: %d\n", rsp.msgno, current->msgno);
 			queue->front++;
 			if(queue->front == WINDOWSIZE) queue->front = 0;
 			queue->count--;		
		}
		else {
			//RESPL rsp = {NAK, current->msgno, 0};
			rsp.ack = NAK;
 			rsp.msgno = current->msgno;
 			rsp.checksum = 0;
		}		
 		memcpy(string,&rsp,sizeof(RESPL));
 		printf("nih %d\n", rsp.msgno);
		if(sendto(sockfd, string, sizeof(RESPL), 0, (struct sockaddr *) &srcAddr, srcLen) < sizeof(RESPL))
			error("ERROR: sendto() sent frame with size more than expected.\n");
 	}
 	return current;
}
