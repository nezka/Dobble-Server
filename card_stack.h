/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   card_stack.h
 * Author: anvy
 *
 * Created on 8. ledna 2017, 16:37
 */

#ifndef CARD_STACK_H
#define CARD_STACK_H

#include "constants.h"

typedef struct s_card{
    int symbols[SYMBOLS_PER_CARD];
} s_card;

typedef struct card_stack {
    int cards[CARD_COUNT];
    int player1;
    int player2;
    int middle;
    s_card card_symbols[CARD_COUNT];
} card_stack;


int init_stack(card_stack *stack);
void shuffle(int *array, int n);
void fill_symbols(s_card *card_symbols, int card_count);
int is_it_right_symbol(int player_card, int middle, int symbol, card_stack *stack);
#endif /* CARD_STACK_H */

