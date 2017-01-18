
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "console.h"

void *console_listening(void *param) {
    char rcv[CONSOLE_BUFF];
    char c;
    int should_run = 1;
    
    printf("For exiting the server write EXIT.\n");
    while (should_run) {
        c = '6';
        memset(rcv, 0, CONSOLE_BUFF);
        read(STDIN_FILENO, rcv, CONSOLE_BUFF);
//        if (rcv[CONSOLE_BUFF - 1] != '\n') {
//            while (c != '\n' && c != EOF) {
//                 c = getchar();
//            }
//        }
        if (!strcmp(rcv, "EXIT\n")) {
            should_run = 0;
           // stop_server_thread();
            exit(0);
        }
       
       printf("%s", rcv);
    }
    return NULL;
}

