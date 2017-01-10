

#ifndef MESSAGE_H
#define MESSAGE_H

#include "client.h"
#include "constants.h"

typedef struct message {
    char type;
    char subtype;
    int len;
    char str[MESSAGE_LEN];
} message;

void message_ack(message *mes);
void message_wait(message *mes);
void message_game_start(message *mes);
void make_message(message *mes, char typ, char subtyp, char* str, int len_str);
message *parse_message(char *rcvBuf, int len);

#endif /* MESSAGE_H */

