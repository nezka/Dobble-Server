
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "console.h"

void *console_listening(void *param) {
    char rcv[CONSOLE_BUFF];
    char c;
    int *should_run_game = (int *)param;
    int should_run_console = 1;
    printf("For exiting the server write EXIT.\n");
    while (should_run_console) {
        c = '6';
        memset(rcv, 0, CONSOLE_BUFF);
        read(STDIN_FILENO, rcv, CONSOLE_BUFF);
        if (!strcmp(rcv, "EXIT\n")) {
            exit(0);
            //*should_run_game = 0;
            //should_run_console = 0;
        }
       
    }
    return NULL;
}

