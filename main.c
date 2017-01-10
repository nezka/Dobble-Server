/* 
 * File:   main.c
 * Author: anvy
 *
 * Created on 1. listopadu 2016, 13:52
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "main.h"
#include "dobble_server.h"
#include "console.h"


/* if (broad) {
                            for (i = 3; i < FD_SETSIZE; i++) {
                                if (FD_ISSET(i, &client_socks) && i != server_socket) {
                                    send_it(i, sendBuf, 10);

                                }
                            }
                        } else {*/

int main(int argc, char** argv) {
   
    int port;
   
    pthread_t console_thread;
    pthread_t network_thread;

    if (pthread_create(&console_thread, NULL, console_listening, NULL)) {
        printf("Error creating  console thread\n");
        return 1;
    }

    if (argc == 2) {
        port = 10002;//atoi(argv[1]);
        if (pthread_create(&network_thread, NULL, run_game, (void *)&port)) {
            printf("Error creating network thread\n");
            return 1;
          }   
    } else {
        printf("Malo argv.");
    }
    
    if (pthread_join(console_thread, NULL)) {
        printf("Error joining console thread\n");
        return 2;
    }
    
    if (pthread_join(network_thread, NULL)) {
        printf("Error joining network thread\n");
        return 2;
    }

    return (EXIT_SUCCESS);
}

