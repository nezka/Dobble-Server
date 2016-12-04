

#ifndef GAME_ACTIONS_H
#define GAME_ACTIONS_H
#include "client.h"
#include "player.h"
#include "room.h"
#include "message.h"

void *run_game(void *param);
void process_message(message *mes, client *cl, char *sendBuf);

#endif /* GAME_MAIN_H */

