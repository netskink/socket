#ifndef common_h
#define common_h


static const int coniPort = 5001;
int make_socket (uint16_t port);
void init_sockaddr (struct sockaddr_in *name, const char *hostname, uint16_t port);

#endif
