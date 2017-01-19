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
#include "thread_param.h"


int port_control(char *argv) {   
    long val;
    char *next;
    
    val = strtol (argv, &next, 10);
    if ((next == argv) || (*next != '\0')) {
        return -1;
    } else {
        return val;
    }
}


int main(int argc, char** argv) {
   
    int port, should_run = 1;
    thread_param param;
    pthread_t console_thread;
    pthread_t network_thread;
    
    if (argc != 2) {
        printf("Incorrect number of arguments.\nStart program with one integer argument that is the port number.");
        return 1;
    }

    port = port_control(argv[1]);
    
    if (port < 0 || port > 65535) {
        printf("Incorrect port number.\nPort number must be in the range <0;65535>.");
        return 1;
    }
    
    param.port = port;
    param.should_run = should_run;
    if (pthread_create(&network_thread, NULL, run_game, (void *)&param)) {
        printf("Error creating network thread\n");
        return 1;
    }   
 
    if (pthread_create(&console_thread, NULL, console_listening, (void *)&(param.should_run))) {
        printf("Error creating  console thread\n");
        return 1;
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

