#ifndef common_h
#define common_h


static const int coniPort = 5001;
static const int  coniMaxMsg = 512;
int make_socket (uint16_t port);
void init_sockaddr (struct sockaddr_in *name, const char *hostname, uint16_t port);

#endif
