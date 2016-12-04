#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "message.h"
#include "constants.h"




void message_ack(message *mes, char *sendBuf) {
    
    sprintf(sendBuf, "%cA%s\n", mes->type, mes->str);
}

void add_name_to_client(client *cl, char *name, int len) {
    strncpy(cl->name, name, len);
    printf("zprava: %s", cl->name);
}

message *parse_message(char *rcvBuf, int len) {
    message *mes;
    
    mes = malloc(sizeof(message));
    if (mes == NULL) {
        printf("Malloc failure!\n");
        return NULL;
    }
    
    memset(mes, 0, sizeof(message));
    mes->type = rcvBuf[0];
    mes->subtype = rcvBuf[1];
    mes->len = len - 3; //type, subtype, enter
    mes->str = malloc(mes->len);
    memset(mes->str, 0, mes->len);
    memcpy(mes->str, rcvBuf + 2, mes->len);
    printf("Zprava: %s\n", mes->str);
    printf("Type: %c, subtype: %c, len: %d\n", mes->type, mes->subtype, mes->len);
    return mes;
    
}