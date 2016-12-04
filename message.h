

#ifndef MESSAGE_H
#define MESSAGE_H

#include "client.h"

typedef struct message {
    char type;
    char subtype;
    int len;
    char *str;
} message;

void add_name_to_client(client *cl, char *name, int len);
void message_ack(message *mes, char *sendBuf);
message *parse_message(char *rcvBuf, int len);

#endif /* MESSAGE_H */

