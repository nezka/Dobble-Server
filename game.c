/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "game.h"
#include "card_stack.h"


game *make_game(client *first, client *sec) {
    game *g;
    g = malloc(sizeof(game));
    if (g == NULL) {
        return NULL;
    }
    memset(g, 0, sizeof(game));
    g->player1 = first;
    g->player2 = sec;
    g->player_count = 2;
    g->round = 1;
    g->taken = 1;
    first->score = 0;
    sec->score = 0;
    init_stack(&(g->stack));
    return g;
}

int add_new_game(client *first, client *sec, game* array, int len) {
    int i; game *g;
    g = make_game(first, sec);
    i = find_first_empty_game(array, len);
    if (i < 0) {
        printf("spatne add new game");
        free(g);
        return -1;
    }
    first->game = i;
    sec->game = i;
    printf("New game on index: %d\n", i);
    memcpy((array+i), g, sizeof(game));
    free(g);
    return i;
}

int find_first_empty_game(game *array, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (!array[i].taken) {
            return i;
        }
    }
    return -1;
}

void remove_game(int i, game *array, int len) {
   /* int i;*/
    /*i = find_client_by_fd(fd, array, len);*/
    if (i >= 0) {
        //printf("lalala\n");
        //free((array+i)->cards);
        memset((array+i), 0, sizeof(game));
        printf("Removed game: %d\n", i);
    }
    
}

int find_game_by_client(game *array, client *cl, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if(array[i].player_count > 0) {
            if (array[i].player1 == cl || array[i].player2 == cl) {
                return i;
            }
        }
    }
    return -1;
}

