/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdlib.h>
#include <stdio.h>
#include "card_stack.h"
#include "constants.h"


int init_stack(card_stack *stack) {
    int i;
    stack->middle = CARD_COUNT - 1;
    stack->player1 = (CARD_COUNT - 1) / 2;
    stack->player2 = 0;
    printf("middle: %d, p1: %d, p2: %d\n", stack->middle, stack->player1, stack->player2);
    
    
    for (i=0; i < CARD_COUNT; i++) {
        stack->cards[i] = i;
    }
    shuffle(stack->cards, CARD_COUNT);
   /* for (i=0; i < CARD_COUNT; i++) {
        printf(" %d ", cur_game->cards[i]);
    }
    printf(" \n ");*/
    return 0;
}


void shuffle(int *array, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        j = i + rand() / (RAND_MAX / (n - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}