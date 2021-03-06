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




int read_from_client (int filedes) {
	char buffer[coniMaxMsg];
	int nbytes;

	nbytes = read (filedes, buffer, coniMaxMsg);
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
	int sockfd; /* the socket we open for server */
	int newfd;	/* the socket we open for accept */
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

	test_one(argv[0]);

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

		/* 
		 Select monitors multiple file descriptos, waiting until one or
		 more of the file descriptos become "ready" for some class of
  		 I/O operation. A file descriptor is considered ready if it is 
 		 possible to perform the corresponding I/O operation (eg. read())
		 without blocking.

		 Three  independent  sets of file descriptors are watched.  
		 1. Those listed in readfds will be watched to see if characters 
		   become available for reading (more precisely,  to  see  if  a
           read will not block; in particular, a file descriptor is also 
		   ready on end-of-file). 
		 
		 2. Those in writefds will be watched to see if a write will not block. 
		  
		 3. Those in exceptfds will be watched for exceptions.  
		 
		 On exit, the sets are modified in place to indicate which file
       	 descriptors actually changed status.  
		 
		 Each of the three file descriptor sets may be specified as NULL if no 
		 file descriptors are to be watched for the corresponding class of events.

		 Four macros are provided to manipulate the sets.  
		  FD_ZERO() clears a  set.   
		  FD_SET()  and FD_CLR() respectively add and remove a given file 
		  	descriptor from a set.  
		  FD_ISSET() tests to see if a file descriptor is part of the set; 
		   this is useful after select() returns.

       	 nfds is the highest-numbered file descriptor in any of the three sets, plus 1.

         The timeout argument specifies the interval that select() should block 
		 waiting for a file descriptor to become ready.  This interval will be 
		 rounded up to the system clock granularity, and kernel scheduling delays 
		 mean that the blocking interval may overrun by a small amount.  If both 
		 fields of the timeval structure are zero, then select() returns immediately.  
		 (This is useful for polling.)  If timeout is NULL (no timeout), select() can
         block indefinitely.


		 int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
		 */

		//printf("blocking on following select call\n");

		/* Block until input arrives on one or more active sockets. */
		read_fd_set = active_fd_set;
		iRC = select (FD_SETSIZE, &read_fd_set, /*write*/NULL, /*exception*/NULL, /*timeout*/NULL);
		/* blocked above until the fd set can read */
		if (iRC < 0) {
			perror ("select");
			exit (EXIT_FAILURE);
		}

		/* Service all the sockets with input pending. */
		for (i = 0; i < FD_SETSIZE; ++i) {
			/* this code will iterate through all 1K file descriptors.
			 * considering we only have at most two, this seems a waste of effort.
			 */
			//printf("in for loop %d\n",i);
			if (FD_ISSET (i, &read_fd_set)) {
				if (i == sockfd) {
					char *pchBuffer;
			        uint16_t uhPort;


					/* Connection request on original socket. */
					/*
					   int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);

				       The  accept()  function shall extract the first connection on the queue of pending 
					   connections, create a new socket with the same socket type protocol and address
					   family as the specified socket, and allocate a new file descriptor for that socket.

       				   The accept() function takes the following arguments:

       				   socket - Specifies a socket that was created with socket(), has been bound to 
					   an address with bind(), and has issued a successful call to listen().

       				  address - Either a null pointer, or a pointer to a sockaddr structure where the  
					  address of the connecting socket shall be returned.

       				  address_len - Points to a socklen_t structure which on input specifies the length of 
					  the supplied sockaddr structure, and on output specifies the length of the stored address.

				      Upon successful completion, accept() shall return the non-negative file descriptor of the
       				  accepted socket. Otherwise, -1 shall be returned and errno set to indicate the error.
					 */
					size = sizeof (clientname);
					newfd = accept (sockfd, (struct sockaddr *) &clientname, &size);
					if (newfd < 0) {
						perror ("accept");
						exit (EXIT_FAILURE);
					}
					
					/*
					char *inet_ntoa(struct in_addr in);
					The inet_ntoa() function converts the Internet host address in, given in network byte
			        order, to a string in IPv4 dotted-decimal notation.  The string is returned in a 
					statically allocated buffer, which subsequent calls will overwrite.
	
			        uint16_t htons(uint16_t hostshort);
			        The htons() function converts the unsigned short integer hostshort from host byte 
				    order to network byte order.
					*/
					

					pchBuffer = inet_ntoa (clientname.sin_addr);
			        uhPort = ntohs (clientname.sin_port);

					fprintf (stderr, "Server: connect from host %s, port %hu.\n",pchBuffer,uhPort);
					// add the newfd to the set of file descriptors to poll.
					FD_SET (newfd, &active_fd_set);
					
					continue;
				} /* handle listen socket file descriptor */

				if (i == newfd) {
					/* Data arriving on an already-connected socket. */
					if (read_from_client (newfd) < 0) {
						close (newfd);
						FD_CLR (newfd, &active_fd_set);
					}
					continue;
				} /* handle accept socket file descriptor */

				// Added a continue to each of the if clauses above, so that
				// if we reach here, we have an error condition where
				// we have input from a fd we don't know about.

				perror("We have some fd set which is not what we are looking for?\n");
				iRC = EXIT_FAILURE;
				goto early_exit;
				
			}
		}

	}

	printf("Normal exit\n");
early_exit:
	exit(iRC);

}



