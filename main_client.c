#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
     
#include "common.h"
#include "unit.h"
#define MESSAGE         "Yow!!! Are we having fun yet?!?"
//#define SERVERHOST      "www.gnu.org"
#define SERVERHOST      "localhost"
     
void write_to_server (int filedes) {
	int nbytes;

	nbytes = write (filedes, MESSAGE, strlen (MESSAGE) + 1);
	if (nbytes < 0) {
		perror ("write");
	exit (EXIT_FAILURE);
	}
}
     
 


int main(int argc, char *argv[]) {

	int iRC;
    int sockfd;
    struct sockaddr_in servername;

	iRC=EXIT_SUCCESS;

	if (argc != 1) {
		fprintf(stderr, "Usage: %s \n", argv[0]);
		iRC=EXIT_FAILURE;
		exit(iRC);
	}

	test_one();

/*
	socket
	connect
	write
	read
	close
 */


	/* Create the socket. */
	sockfd = socket (PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror ("socket (client)");
		exit (EXIT_FAILURE);
	}

	/* Connect to the server. */
	init_sockaddr (&servername, SERVERHOST, coniPort);
	iRC = connect (sockfd, (struct sockaddr *) &servername, sizeof (servername));
	if (iRC < 0 ) {
		perror ("connect (client)");
		exit (EXIT_FAILURE);
	}

	/* Send data to the server. */
	write_to_server (sockfd);
	close (sockfd);


 
	printf("Normal exit\n");    
	exit(iRC);

}



