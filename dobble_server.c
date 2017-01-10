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
#include "dobble_server.h"

void *run_game(void *param) {
    int port, i, a2read, fd, return_value, client_socket, len_addr, server_socket, index;
    char rcvBuf[BUFF_SIZE];
    char sendBuf[BUFF_SIZE];
    fd_set tests, client_socks;
    struct sockaddr_in my_addr, peer_addr;
    client curr_client, client_arr[CLIENT_COUNT];
    game game_arr[GAME_COUNT];
    message *mes;
    
    
    port = *(int *)param;
    server_socket = server_init(port);
    client_socks = fd_init(server_socket);
    
    memset(client_arr, 0, CLIENT_COUNT*sizeof(client));
    memset(game_arr, 0, GAME_COUNT*sizeof(game));
    
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
                    //peer_addr.sin_addr.
                    
                    add_new_client(client_socket, client_arr, CLIENT_COUNT);
                    FD_SET(client_socket, &client_socks);
                    printf("Pripojen novy klient a pridan do sady socketu\n");
                    
                } else {

                    ioctl(fd, FIONREAD, &a2read);
                    if (a2read > 0) {
                        memset(rcvBuf, 0, BUFF_SIZE);
                        read(fd, rcvBuf, a2read);
                        if (a2read < 3 && a2read > 255) {
                            client_arr[i].bad_messages++;                       
                        } else {
                            mes = parse_message(rcvBuf, a2read);                       
                            i = find_client_by_fd(fd, client_arr, CLIENT_COUNT);        
                            process_message(mes, client_arr, i, game_arr);
                            message_to_string(mes, sendBuf);
                            send_it(fd, sendBuf, a2read);
                            //free(mes->str);
                            free(mes);
                            memset(sendBuf, 0, BUFF_SIZE);
                        }
                        
                        



                    }
                    else {
                        index = find_client_by_fd(fd, client_arr, CLIENT_COUNT);
                        index = client_arr[index].game;
                        close(fd);
                        FD_CLR(fd, &client_socks);
                        game_arr[index].player_count--;
                        if (game_arr[index].player_count == 0) {
                            remove_game(index, game_arr, GAME_COUNT);
                        }
                        remove_client(fd, client_arr, CLIENT_COUNT);
                        printf("Klient se odpojil a byl odebran ze sady socketu\n");
                    }
                }
            }
        }

    }

}

void process_message(message *mes, client *clients, int cl, game *games) {
    client *rival; int rival_id; int game_id;
    client *cur_cl = &clients[cl];
    switch(mes->type) {
        case 'H':
            message_ack(mes);
            break;
        case 'N':
            add_name_to_client(cur_cl, mes->str, mes->len);
            //printf("%s", cur_cl->name);
            message_ack(mes);
            rival_id = find_rival_to_client(cur_cl->client_fd, clients, CLIENT_COUNT);
            if (rival_id < 0) {
                message_wait(mes);
                break;
            }
            rival = &clients[rival_id];
            game_id = add_new_game(cur_cl, rival, games, GAME_COUNT);
            //printf("game id = %d", game_id);
             message_game_start(mes);
            //start_game(games[game_id]);
            //printf("name 1: %s    name 2: %s", games[0].player1->name, games[0].player2->name);
            //return 0;
            break;
        case 'G':
            
            break;
            //return 0;
        case 'R':
            //return 0;
        default:
            cur_cl->bad_messages++;
            //return 1;
    }
}

