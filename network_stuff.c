#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "network_stuff.h"
#include "constants.h"
#include "parser.h"

int bind_it(int port) {
    int server_socket, return_value;
    struct sockaddr_in my_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&my_addr, 0, sizeof (struct sockaddr_in));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

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

void *network_listening(void *param) {
    int server_socket, port;
    fd_set client_socks;
    client clients[CLIENT_COUNT];
    port = *(int *)param;

    FD_ZERO(&client_socks);

    server_socket = server_init(port);
    client_socks = fd_init(server_socket);
    select_it(server_socket, client_socks, clients);
    return 0;

}

int accept_new_connection(int server_socket, struct sockaddr_in *peer_addr, int *len_addr) {
    int client_socket;
    
    client_socket = accept(server_socket, (struct sockaddr *) peer_addr, len_addr);
    return client_socket;
}

int select_it(int server_socket, fd_set client_socks, client *clients) {
    int i;
    int fd;
    int broad;
    int return_value;
    char rcvBuf[BUFF_SIZE];
    char sendBuf[BUFF_SIZE];
    int a2read;
    fd_set tests;
    int client_socket;
    int len_addr;
    struct sockaddr_in my_addr, peer_addr;
    client curr_client;
    client client_arr[CLIENT_COUNT];
    
    memset(client_arr, 0, CLIENT_COUNT*sizeof(client));

    for (;;) {

        tests = client_socks;
        FD_SET(0, &tests);
        return_value = select(FD_SETSIZE, &tests, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);

        if (return_value < 0) {
            printf("\nSelect - ERR\n");
            return -1;
        }
        
                // vynechavame stdin, stdout, stderr
        for (fd = 3; fd < FD_SETSIZE; fd++) {
            // je dany socket v sade fd ze kterych lze cist ?
            if (FD_ISSET(fd, &tests)) {
                if (fd == server_socket) {
                    client_socket = accept_new_connection(server_socket, &peer_addr, &len_addr);
                    add_new_client(client_socket, client_arr, CLIENT_COUNT);
                    FD_SET(client_socket, &client_socks);
                    printf("Pripojen novy klient a pridan do sady socketu\n");
                    
                } else {

                    ioctl(fd, FIONREAD, &a2read);


                    if (a2read > 0) {
                        memset(rcvBuf, 0, BUFF_SIZE);
                        read(fd, rcvBuf, a2read);

                        broad = process_message(rcvBuf, sendBuf, a2read);
                        if (broad) {
                            for (i = 3; i < FD_SETSIZE; i++) {
                                if (FD_ISSET(i, &client_socks) && i != server_socket) {
                                    send_it(i, sendBuf, 10);

                                }
                            }
                        } else {
                            send_it(fd, sendBuf, a2read);
                        }



                    }// na socketu se stalo neco spatneho
                    else {
                        close(fd);
                        FD_CLR(fd, &client_socks);
                        remove_client(fd, client_arr, CLIENT_COUNT);
                        printf("Klient se odpojil a byl odebran ze sady socketu\n");
                    }
                }
            }
        }

    }

    return 0;
}

void broadcast_it(client *clients, char *message, int len) {
    
}


void send_it(int fd, char *message, int len) {
    write(fd, message, len);
    printf("Odeslana zprava: %s", message);
}


