#include <string.h>
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

message *process_message(char *rcvBuf, char *sendBuf, int len) {
    message *mes;
    
    mes = malloc(sizeof(message));
    if (mes == NULL) {
        printf("Malloc failure!\n");
        return NULL;
    }
    memset(sendBuf, 0, BUFF_SIZE);
    memset(mes, 0, sizeof(message));
    mes->type = rcvBuf[0];
    mes->subtype = rcvBuf[1];
    mes->len = rcvBuf[2];
    mes->str = malloc(BUFF_SIZE - 4);
    memset(mes->str, 0, BUFF_SIZE - 4);
    memcpy(mes->str, rcvBuf + 3, BUFF_SIZE - 4);
    printf("Zprava: %s\n", mes->str);
    printf("Type: %c, subtype: %c, len: %d\n", mes->type, mes->subtype, mes->len);
    switch(rcvBuf[0]) {
        case 'B':
            prepare_message_broadcast(sendBuf);
            return 1;
            break;
        default:
            prepare_message_reverse(sendBuf, rcvBuf, len);
            return 0;
            break;
    }

}