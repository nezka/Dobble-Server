

#ifndef GAME_ACTIONS_H
#define GAME_ACTIONS_H
#include "client.h"
#include "game.h"
#include "message.h"

void *run_game(void *param);
void process_message(message *mes, client *clients, int cl, game *games);

#endif /* GAME_MAIN_H */

