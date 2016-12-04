#include <stdio.h>
#include <string.h>
#include "game_engine.h"
#include "constants.h"


void process_message(message *mes, client *cl, char *sendBuf) {
    switch(mes->type) {
        case 'H':
            add_name_to_client(cl, mes->str, mes->len);
            message_ack(mes, sendBuf);
            break;
        case 'E':
            break;
        default:
            printf("Spatny format zpravy, switch process message");
    }
}

void message_ack(message *mes, char *sendBuf) {
    
    sprintf(sendBuf, "%cA%s\n", mes->type, mes->str);
}

void add_name_to_client(client *cl, char *name, int len) {
    strncpy(cl->name, name, len);
    printf("zprava: %s", cl->name);
}