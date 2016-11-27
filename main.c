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
#include "network_stuff.h"
#include "tests.h"
#include "console_stuff.h"

int main(int argc, char** argv) {
    int run_test = 0;
    int *port;
    //run_test = 1;
    /*if(run_test){
        tests_run();
        return 0;
    }*/
    pthread_t console_thread;
    pthread_t network_thread;

    if (pthread_create(&console_thread, NULL, console_listening, NULL)) {
        printf("Error creating  console thread\n");
        return 1;
    }

    if (argc == 2) {
        *port = atoi(argv[1]);
        if (pthread_create(&network_thread, NULL, network_listening, (void *)port)) {
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

    //run(10007);
    return (EXIT_SUCCESS);
}

