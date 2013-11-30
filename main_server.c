/*
 * This code was based on:
 * http://www.gnu.org/software/libc/manual/html_node/Server-Example.html
 */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "unit.h"
#include "common.h"

#define MAXMSG  512



int read_from_client (int filedes) {
	char buffer[MAXMSG];
	int nbytes;

	nbytes = read (filedes, buffer, MAXMSG);
	if (nbytes < 0) {
		/* Read error. */
		perror ("read");
		exit (EXIT_FAILURE);
	} else if (nbytes == 0) {
		/* End-of-file. */
		return -1;
	} else {
		/* Data read. */
		fprintf (stderr, "Server: got message: `%s'\n", buffer);
		return 0;
	}
}

int main(int argc, char *argv[]) {

	int iRC;
	int sockfd;
    fd_set active_fd_set;
    fd_set read_fd_set;
    int i;
    struct sockaddr_in clientname;
    socklen_t size;
     

	iRC=EXIT_SUCCESS;

	if (argc != 1) {
		fprintf(stderr, "Usage: %s \n", argv[0]);
		iRC=EXIT_FAILURE;
		exit(iRC);
	}

	test_one();

/*
	The sequence for a server socket is:
	socket - create socket
	bind - bind the socket
	listen - listen for connections
	accept - accept a connection
	in a loop 
		read - read from a socket
		write - write to a socket
	close - close the socket
 */
	
	/* Create the socket and set it up to accept connections. */
	sockfd = make_socket (coniPort);
	if (sockfd < 0) {
		perror ("make_socket");
		exit (EXIT_FAILURE);
	}
	iRC = listen (sockfd, 1);
	if (iRC < 0) {
		perror ("listen");
		exit (EXIT_FAILURE);
	}

	/* Initialize the set of active sockets. */
	FD_ZERO (&active_fd_set);
	FD_SET (sockfd, &active_fd_set);

	while (1) {
		/* Block until input arrives on one or more active sockets. */
		read_fd_set = active_fd_set;
		iRC = select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL);
		if (iRC < 0) {
			perror ("select");
			exit (EXIT_FAILURE);
		}

		/* Service all the sockets with input pending. */
		for (i = 0; i < FD_SETSIZE; ++i) {
			if (FD_ISSET (i, &read_fd_set)) {
				if (i == sockfd) {

					/* Connection request on original socket. */
					int new;
					size = sizeof (clientname);
					new = accept (sockfd, (struct sockaddr *) &clientname, &size);

					if (new < 0) {
						perror ("accept");
						exit (EXIT_FAILURE);
					}

					fprintf (stderr, "Server: connect from host %s, port %hd.\n",
					inet_ntoa (clientname.sin_addr),
					ntohs (clientname.sin_port));
					FD_SET (new, &active_fd_set);
				} else {
					/* Data arriving on an already-connected socket. */
					if (read_from_client (i) < 0) {
							close (i);
							FD_CLR (i, &active_fd_set);
					}
				}
			}
		}

	}

	printf("Normal exit\n");
	exit(iRC);

}



