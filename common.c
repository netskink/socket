
/*
 * This code is based on:
 * http://www.gnu.org/software/libc/manual/html_node/Inet-Example.html#Inet-Example
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>


#include "common.h"

int make_socket (uint16_t port) {
	int sock;
	struct sockaddr_in name;
	int iRC;

	/* Create the socket. */
	sock = socket (PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror ("socket");
		return (EXIT_FAILURE);
	}

	/* Give the socket a name. */
	name.sin_family = AF_INET;
	name.sin_port = htons (port);
	name.sin_addr.s_addr = htonl (INADDR_ANY);
	iRC = bind (sock, (struct sockaddr *) &name, sizeof (name));
	if (iRC < 0) {
		perror ("bind");
		return (EXIT_FAILURE);
	}

	return sock;
}



void init_sockaddr (struct sockaddr_in *name, const char *hostname, uint16_t port) {
	struct hostent *hostinfo;

	name->sin_family = AF_INET;
	name->sin_port = htons (port);
	hostinfo = gethostbyname (hostname);
	if (hostinfo == NULL) {
		fprintf (stderr, "Unknown host %s.\n", hostname);
		exit (EXIT_FAILURE);
	}
	name->sin_addr = *(struct in_addr *) hostinfo->h_addr;
}



