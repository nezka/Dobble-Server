
#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
#include "client.h"
#include "message.h"

void process_message(message *mes, client *cl, char *sendBuf);
void add_name_to_client(client *cl, char *name, int len);
void message_ack(message *mes, char *sendBuf);

#endif /* GAME_ENGINE_H */

