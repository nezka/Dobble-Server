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

#include "dobble_server.h"

void *run_game(void *param) {
    int port, i, j, a2read, fd, return_value, client_socket, len_addr, server_socket, index;
    char rcvBuf[BUFF_SIZE];
   // char sendBuf[BUFF_SIZE];
    fd_set tests, client_socks;
    struct sockaddr_in my_addr, peer_addr;
    client curr_client, client_arr[CLIENT_COUNT];
    game game_arr[GAME_COUNT];
    message *mes;


    port = *(int *) param;
    server_socket = server_init(port);
    client_socks = fd_init(server_socket);

    memset(client_arr, 0, CLIENT_COUNT * sizeof (client));
    memset(game_arr, 0, GAME_COUNT * sizeof (game));

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
                        if (a2read < 3 && a2read > 255) {
                            printf("Wrong format of message!\n");                       
                        } else {
                            mes = parse_message(rcvBuf, a2read);
                            i = find_client_by_fd(fd, client_arr, CLIENT_COUNT);
                            process_message(mes, client_arr, i, game_arr);
                            free(mes);
                            
                        }

                    } else {
                        index = find_client_by_fd(fd, client_arr, CLIENT_COUNT);
                        client_arr[index].inactive = 1;
                        index = client_arr[index].game;
                        close(fd);
                        FD_CLR(fd, &client_socks);//dodelat, kdyd se odpojuje jen jeden
                        game_arr[index].player_count--;
                        if (game_arr[index].player_count == 0) {
                            if (game_arr[index].player1->taken) {
                                remove_client(game_arr[index].player1);
                            }
                            if (game_arr[index].player2->taken) {
                                remove_client(game_arr[index].player2);
                            }
                            remove_game(index, game_arr, GAME_COUNT);
                            printf("Removing all.\n");
                        }
                        
                        printf("Klient se odpojil a byl odebran ze sady socketu\n");
                    }
                }
            }
        }

    }

}

void process_message(message *mes, client *clients, int cl, game *games) {
 
    switch (mes->type) {
        case 'G':
            process_game_message(mes, clients, cl, games);
            break;
        case 'S':
           // process_service_message();
            break;

        default:
            printf("hahaa");
     
    }
}

void process_game_message(message *mes, client *clients, int cl, game *games) {
    client *cur_cl = &clients[cl];
    switch (mes->subtype) {
        case 'N':
            new_connected(mes, clients, cur_cl, games);
            break;
        case 'C':
            card_clicked(mes, cur_cl, games);
            break;
        case 'G':
            printf("chci hrat znovu");
            break;
        default:;


    }
}

void card_clicked(message *mes, client *cl, game *games) {
    char *cur; game *the_game;
    int first, right, symbol;
    the_game = &games[cl->game];
    first = (the_game->player1->client_fd == cl->client_fd ? 1 : 0);
    cur = strtok(mes->str, ";");
    if (atoi(cur) != the_game->round) {
        return;
    }
    cur = strtok(NULL, ";");
    symbol = atoi(cur);
    printf("%d %d \n",  the_game->stack.player1, the_game->stack.middle);
    if (first) {
        printf("zasrana karta: %d\n", the_game->stack.card_symbols->symbols[4]);
        
        right = is_it_right_symbol(the_game->stack.player1, the_game->stack.middle, symbol, &(the_game->stack));
        printf("first\n");
        if (right) {
            the_game->player1->score++;
            the_game->stack.middle = the_game->stack.player1;
            the_game->stack.player1++;
            
        } else {
            message_wrong(mes, symbol);
            send_it(the_game->player1->client_fd, mes);
            return;
        }
        
    } else {
        printf("first2\n");
        right = is_it_right_symbol(the_game->stack.player2, the_game->stack.middle, symbol, &(the_game->stack));
         printf("first\n");
        if (right) {
            the_game->player2->score++;
            the_game->stack.middle = the_game->stack.player2;
            the_game->stack.player2++;
        } else {
            message_wrong(mes, symbol);
            send_it(the_game->player2->client_fd, mes);
            return;
        }
    }
    if (the_game->stack.player1 == 5 || the_game->stack.player2 == 2) {
        message_finish(mes, the_game, 1);
        send_it(the_game->player1->client_fd, mes);
        message_finish(mes, the_game, 0);
        send_it(the_game->player2->client_fd, mes);
    } else {
        the_game->round++;
        message_victor(mes, the_game, 1);
        send_it(the_game->player1->client_fd, mes);
        message_victor(mes, the_game, 0);
        send_it(the_game->player2->client_fd, mes);
    }
    
    
    
}

void new_connected(message *mes, client *clients, client *cur_cl, game *games) {
    int rival_id, game_id, secret; client *retry_client;
    game *retry_game;
    client *rival;
    char str[SECRET_LEN+1];
 
   
    if (mes->len == SECRET_LEN) {
        
        memset(str, 0, (SECRET_LEN + 1)*sizeof(char));
        memcpy(str, mes->str, SECRET_LEN*sizeof(char));     
        retry_client = find_client_by_secret(clients, atoi(str), CLIENT_COUNT);
              
        if(retry_client != NULL) {        
            retry_client->client_fd = cur_cl->client_fd;
            retry_client->inactive = 0;
            remove_client(cur_cl);
            retry_game = &games[retry_client->game];
            retry_game->player_count++;
            message_victor(mes, retry_game, 1);//          
            send_it(retry_game->player1->client_fd, mes);
            message_victor(mes, retry_game, 0);           
            send_it(retry_game->player2->client_fd, mes);
            return;
        }
        
    }

    rival_id = find_rival_to_client(cur_cl->client_fd, clients, CLIENT_COUNT);
    if (rival_id < 0) {
        cur_cl->game = -1;
        message_wait(mes);
        send_it(cur_cl->client_fd, mes);
        return;
    }
    
    rival = &clients[rival_id];
    game_id = add_new_game(cur_cl, rival, games, GAME_COUNT);
    rival->secret = get_secret();
    cur_cl->secret = get_secret();

    printf("game id = %d\n", game_id);
 
    message_secret(mes, (games[game_id]).player1);
    send_it(games[game_id].player1->client_fd, mes);
    message_secret(mes, (games[game_id]).player2);
    send_it(games[game_id].player2->client_fd, mes);
    message_victor(mes, &(games[game_id]), 1);
    send_it(games[game_id].player1->client_fd, mes);
    message_victor(mes, &(games[game_id]), 0);
    send_it(games[game_id].player2->client_fd, mes);
    
}

void process_service_message(message *mes) {
    switch (mes->subtype) {
        case 'H':
            printf("nkjwg");
            break;
        case 'N':
            break;
        default:;


    }
}

