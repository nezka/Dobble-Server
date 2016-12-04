#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "constants.h"


void prepare_message_reverse(char *dst, const char *src, int len) {
    int i;
    for (i = 1; i < len; i++) {
        dst[i - 1] = src[len - i - 1];
    }
    dst[len - 1] = '\n';
}

void prepare_message_broadcast(char *dst) {
    char* str = "Broadcast\n";
    //strncpy(str, dst, sizeof(str));
    memcpy(dst, str, strlen(str) * sizeof(char));
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
    mes->str = malloc(BUFF_SIZE - 3);
    memset(mes->str, 0, BUFF_SIZE - 3);
    memcpy(mes->str, rcvBuf + 2, BUFF_SIZE - 3);
    printf("Zprava: %s\n", mes->str);
    printf("Type: %c, subtype: %c, len: %d\n", mes->type, mes->subtype, mes->len);
    switch(rcvBuf[0]) {
        case 'B':
            //prepare_message_broadcast(sendBuf);
            return mes;
            break;
        default:
            //prepare_message_reverse(sendBuf, rcvBuf, len);
            return mes;
            break;
    }
    
}