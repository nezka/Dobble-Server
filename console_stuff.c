
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "console_stuff.h"

void *console_listening(void *param) {
    char rcv[CONSOLE_BUFF];
    char c;
    
    printf("Start vlakna konzole\n");
    for (;;) {
        c = '6';
        memset(rcv, 0, CONSOLE_BUFF);
        read(STDIN_FILENO, rcv, CONSOLE_BUFF);
//        if (rcv[CONSOLE_BUFF - 1] != '\n') {
//            while (c != '\n' && c != EOF) {
//                 c = getchar();
//            }
//        }
        if (!strcmp(rcv, "EXIT\n")) {
            exit(0);
        }
       
       printf("%s", rcv);
    }
    return NULL;
}

