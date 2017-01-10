#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "message.h"
#include "constants.h"




void message_ack(message *mes) {
    mes->subtype = 'A';   
}

void message_wait(message *mes) {
    mes->subtype = 'W';   
}

void message_game_start(message *mes) {
    sprintf(mes->str, "");
    
    make_message(mes, 'G', 'S', "", 4); 
    
}

void message_to_string(message *mes, char *sendBuf) {
    sprintf(sendBuf, "%cA%s\n", mes->type, mes->str);
}

message *parse_message(char *rcvBuf, int len) {
    message *mes; int len_str; char typ, subtyp;
    char *str;
    
    mes = malloc(sizeof(message));
    if (mes == NULL) {
        printf("Malloc failure!\n");
        return NULL;
    }
    typ = rcvBuf[0];
    subtyp = rcvBuf[1];
    len_str = len - 3;
    str = malloc(len_str);
    if (str == NULL) {
        printf("Malloc failure!\n");
        return NULL;
    }
    memcpy(str, rcvBuf + 2, len_str);
    make_message(mes, typ, subtyp, str, len_str);
    free(str);
    printf("Zprava obdrzena: %s\n", mes->str);
    //printf("Type: %c, subtype: %c, len: %d\n", mes->type, mes->subtype, mes->len);
    return mes;
    
}

void make_message(message *mes, char typ, char subtyp, char* str, int len_str) {
    memset(mes, 0, sizeof(message));
    
    mes->type = typ;
    mes->subtype = subtyp;
    mes->len = len_str;
    //memset(mes->str, 0, MESSAGE_LEN);
    memcpy(mes->str, str, len_str);
    
    
}