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
        str[0] = game->player1->score + '0';
        str[2] = game->player2->score + '0';
        str[6] = game->stack.player1 + '0';
        
    } else {
        str[0] = game->player2->score + '0';
        str[2] = game->player1->score + '0';
        str[6] = game->stack.player2 + '0';
        
    }
    str[4] = game->round + '0';
    str[8] = game->stack.middle + '0';
    str[9] = '\n';
    make_message(mes, 'G', 'V', str, strlen(str)); 
    
}

void message_opponent(message *mes) {
    
    make_message(mes, 'S', 'O', "", 0); 
  
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

void message_incorrect(message *mes, int symbol) {

    char str[BUFF_SIZE];
    
    memset(str, 0, BUFF_SIZE);
    sprintf(str,"%d", symbol);
    make_message(mes, 'G', 'I', str, strlen(str)); 
    
}

void message_to_string(message *mes, char *sendBuf) {
    sprintf(sendBuf, "%c%c%s\n", mes->type, mes->subtype, mes->str);
}

message parse_message(char *rcvBuf, int len) {
    message mes; int len_str; char typ, subtyp;
    char *str;
    typ = rcvBuf[0];
    subtyp = rcvBuf[1];
    len_str = len - 3;
    str = malloc(len_str);
    if (str == NULL) {
        printf("Malloc failure!\n");
       // return NULL;
    }
    memcpy(str, rcvBuf + 2, len_str);
    make_message(&mes, typ, subtyp, str, len_str);
    free(str);
    return mes;
    
}

void make_message(message *mes, char typ, char subtyp, char* str, int len_str) {
    memset(mes, 0, sizeof(message));
    
    mes->type = typ;
    mes->subtype = subtyp;
    mes->len = len_str;
    memcpy(mes->str, str, len_str);
   
}