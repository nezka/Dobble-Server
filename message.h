

#ifndef MESSAGE_H
#define MESSAGE_H

#include "client.h"
#include "constants.h"
#include "game.h"

typedef struct message {
    char type;
    char subtype;
    int len;
    char str[MESSAGE_LEN];
} message;

void message_ack(message *mes);
void message_wait(message *mes);
void message_secret(message *mes, client *player);
void message_victor(message *mes, game *game, int first);
void message_opponent(message *mes);
void message_finish(message *mes, game *g, int first);
void message_incorrect(message *mes, int symbol);
void make_message(message *mes, char typ, char subtyp, char* str, int len_str);
message parse_message(char *rcvBuf, int len);

#endif /* MESSAGE_H */

