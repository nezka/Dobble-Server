#ifndef ROOM_H
#define ROOM_H

#include "client.h"
#include "card_stack.h"
#include "constants.h"

typedef struct game {
    int player_count;
    int round;
    client *player1;
    client *player2;
    int taken;
    card_stack stack;
    int secret;
} game;

game *make_game(client *first, client *sec);
int add_new_game(client *first, client *sec, game* array, int len);
int find_first_empty_game(game *array, int len);
void remove_game(int i, game *array, int len);

#endif /* ROOM_H */

