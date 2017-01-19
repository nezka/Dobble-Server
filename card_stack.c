#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "card_stack.h"
#include "constants.h"

/*
void printCard(s_card *card, int crd) {
    int i;
    printf("karta %d[%p]: \n", crd, card);
    for (i = 0; i < SYMBOLS_PER_CARD; i++) {
        printf("%d, ", card->symbols[i]);

    }
    printf("\n");
}*/

int init_stack(card_stack *stack) {
    int i;
    stack->middle = CARD_COUNT - 1;
    stack->player1 = (CARD_COUNT - 1) / 2;
    stack->player2 = 0;
    fill_symbols(stack->card_symbols, CARD_COUNT);


    for (i = 0; i < CARD_COUNT; i++) {
        stack->cards[i] = i;
    }
    shuffle(stack->cards, CARD_COUNT);
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

void fill_card(s_card *card, int a, int b, int c, int d, int e) {
    card->symbols[0] = a;
    card->symbols[1] = b;
    card->symbols[2] = c;
    card->symbols[3] = d;
    card->symbols[4] = e;
}

void fill_symbols(s_card *card_symbols, int card_count) {
    fill_card(card_symbols + 0, 0, 1, 2, 3, 4);
    fill_card(card_symbols + 1, 4, 5, 6, 7, 8);
    fill_card(card_symbols + 2, 1, 9, 10, 5, 11);
    fill_card(card_symbols + 3, 12, 7, 2, 14, 9);
    fill_card(card_symbols + 4, 12, 10, 8, 0, 13);

}


int is_it_right_symbol(int player_card, int middle, int symbol, card_stack *stack) {
    int i, right = 0;
    s_card *card;
    card = stack->card_symbols + player_card;
    for (i = 0; i < SYMBOLS_PER_CARD; i++) {

        if (card->symbols[i] == symbol) {
            right = 1;
            break;
        }
    }
    card = stack->card_symbols + middle;
    for (i = 0; i < SYMBOLS_PER_CARD; i++) {
        if (card->symbols[i] == symbol && right) {
            return 1;
        }
    }
    return 0;

}