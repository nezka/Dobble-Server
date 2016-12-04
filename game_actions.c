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
#include "game_actions.h"

void *run_game(void *param) {
    int port, i, a2read, fd, return_value, client_socket, len_addr, server_socket;
    char rcvBuf[BUFF_SIZE];
    char sendBuf[BUFF_SIZE];
    fd_set tests, client_socks;
    struct sockaddr_in my_addr, peer_addr;
    client curr_client, client_arr[CLIENT_COUNT];
    player player_arr[PLAYER_COUNT];
    room room_arr[GAME_COUNT];
    message *mes;
    
    
    port = *(int *)param;
    server_socket = server_init(port);
    client_socks = fd_init(server_socket);
    
    memset(client_arr, 0, CLIENT_COUNT*sizeof(client));
    memset(player_arr, 0, PLAYER_COUNT*sizeof(player));
    memset(room_arr, 0, GAME_COUNT*sizeof(room));
    
    for (;;) {

        tests = client_socks;
        FD_SET(0, &tests);
        return_value = select(FD_SETSIZE, &tests, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);

        if (return_value < 0) {
            printf("\nSelect - ERR\n");
            return NULL;
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
                        
                        mes = parse_message(rcvBuf, a2read);
                        i = find_client_by_fd(fd, client_arr, CLIENT_COUNT);
                        process_message(mes, &client_arr[i], sendBuf);
                       /* if (broad) {
                            for (i = 3; i < FD_SETSIZE; i++) {
                                if (FD_ISSET(i, &client_socks) && i != server_socket) {
                                    send_it(i, sendBuf, 10);

                                }
                            }
                        } else {*/
                        send_it(fd, sendBuf, a2read);
                        free(mes);
                        memset(sendBuf, 0, BUFF_SIZE);
                        



                    }
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

}

void process_message(message *mes, client *cl, char *sendBuf) {
    switch(mes->type) {
        case 'N':
            add_name_to_client(cl, mes->str, mes->len);
            message_ack(mes, sendBuf);
            //return 0;
            break;
        case 'E':
            break;
            //return 0;
        case 'R':
            //return 0;
        default:
            printf("Spatny format zpravy, switch process message");
            //return 1;
    }
}

