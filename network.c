#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "network.h"
#include "constants.h"
#include "message.h"


int bind_it(int port) {
    int server_socket, return_value;
    struct sockaddr_in my_addr;
    int reuse = 1;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&my_addr, 0, sizeof (struct sockaddr_in));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    return_value = bind(server_socket, (struct sockaddr *) &my_addr, \
		sizeof (struct sockaddr_in));
    printf("Bind %d ...", port);
    if (return_value == 0) {
        printf("OK\n");
        return server_socket;
    } else {
        printf("ERR\n");
        return -1;
    }
}

int listen_to_the_beat(int server_socket) {
    int return_value;

    return_value = listen(server_socket, MAX_QUEUED_CONNECTIONS);
    if (return_value == 0) {
        printf("Listen - OK\n");
    } else {
        printf("Listen - ER\n");
    }
    return return_value;

}

int server_init(int port) {
    int server_socket;
    server_socket = bind_it(port);

    if (server_socket == -1) {
        printf("server_socket = -1");
        return -1;
    }

    if (listen_to_the_beat(server_socket) != 0) {
        printf("listen messed up");
        return -1;
    }
    return server_socket;

}

fd_set fd_init(int server_socket) {
    fd_set client_socks;
    FD_ZERO(&client_socks);
    FD_SET(server_socket, &client_socks);
    return client_socks;
}


int accept_new_connection(int server_socket, struct sockaddr_in *peer_addr, int *len_addr) {
    int client_socket;
    
    client_socket = accept(server_socket, (struct sockaddr *) peer_addr, len_addr);
    return client_socket;
}


void send_it(int fd, message *message) {
    char sendBuff[BUFF_SIZE];
    memset(sendBuff, 0, BUFF_SIZE);
    sendBuff[0] = message->type;
    sendBuff[1] = message->subtype;
    memcpy((sendBuff+2), message->str, message->len);
    if (sendBuff[message->len + 1] != '\n') {
        sendBuff[message->len + 2] = '\n';
    }
    
    write(fd, sendBuff, message->len + 3);
    //printf("FD: %d odeslana zprava: %s", fd, sendBuff);
}