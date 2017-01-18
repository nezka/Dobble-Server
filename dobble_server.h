

#ifndef GAME_ACTIONS_H
#define GAME_ACTIONS_H
#include "client.h"
#include "game.h"
#include "message.h"

void *run_game(void *param);

void process_message(message *mes, client *clients, int cl, game *games, int *game_message);
//void process_service_message(message *mes);

void card_clicked(message *mes, client *cl, game *games);
void process_game_message(message *mes, client *clients, int cl, game *games, int *game_message);
void new_connected(message *mes, client *clients, client *cur_cl, game *games);
void bye_opponent(client *cur_cl, game *games);


#endif /* GAME_MAIN_H */

