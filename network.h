#ifndef NETWORK_H
#define NETWORK_H

#include <sys/socket.h>

int bind_it(int port);
int listen_to_the_beat(int server_socket);
int server_init(int port);
fd_set fd_init(int server_socket);
void send_it(int fd, char *message, int len);
int accept_new_connection(int server_socket, struct sockaddr_in *peer_addr, int *len_addr);
#endif /* NETWORK_STUFF_H */

