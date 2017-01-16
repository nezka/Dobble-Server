#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "message.h"
#include "constants.h"
#include "game.h"




void message_ack(message *mes) {
    mes->subtype = 'A';   
}


void message_wait(message *mes) {
    
    make_message(mes, 'G', 'W', "", 0); 
    
}

void message_secret(message *mes, client *player) {
    char str[5];
    sprintf(str, "%d", player->secret);
    
    make_message(mes, 'G', 'G', str, 5); 
    
}

void message_victor(message *mes, game *game, int first) {
    char str[BUFF_SIZE];
    
    memset(str, 0, BUFF_SIZE);
    str[1] = ';';
    str[3] = ';';
    str[5] = ';';
    str[7] = ';';
  
    if (first) {
        printf("client nalezena4111\n");
        str[0] = game->player1->score + '0';
         printf("client nalezena3\n");
        str[2] = game->player2->score + '0';
         printf("client nalezena4\n");
        str[6] = game->stack.player1 + '0';
        
    } else {
        str[0] = game->player2->score + '0';
         printf("client nalezena5\n");
        str[2] = game->player1->score + '0';
        str[6] = game->stack.player2 + '0';
        
    }
    str[4] = game->round + '0';
    str[8] = game->stack.middle + '0';
    str[9] = '\n';
    printf("\n%zd\n", strlen(str));
    make_message(mes, 'G', 'V', str, strlen(str)); 
    
}

void message_finish(message *mes, game *g, int first) {
    char str[BUFF_SIZE];
    
    memset(str, 0, BUFF_SIZE);
    if (first) {
        sprintf(str, "%d;%d", g->player1->score, g->player2->score);
    } else {
        sprintf(str, "%d;%d", g->player2->score, g->player1->score);
    }
    
    
    make_message(mes, 'G', 'F', str, strlen(str)); 
    
}

void message_wrong(message *mes, int symbol) {

    char str[BUFF_SIZE];
    
    memset(str, 0, BUFF_SIZE);
    sprintf(str,"%d", symbol);
    make_message(mes, 'G', 'W', str, strlen(str)); 
    
}

void message_to_string(message *mes, char *sendBuf) {
    sprintf(sendBuf, "%c%c%s\n", mes->type, mes->subtype, mes->str);
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