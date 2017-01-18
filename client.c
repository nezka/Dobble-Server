
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "client.h"


client *make_client(int fd) {
    client *cl;
    cl = malloc(sizeof(client));
    if (cl == NULL) {
        return NULL;
    }
    memset(cl, 0, sizeof(client));
    cl->client_fd = fd;
    cl->secret = 0;
    cl->taken = 1;
    cl->game = -2;
    cl->score = 0;
    cl->inactive = 0;
    return cl;
}

void reset_client(client *cl) {
    cl->game = -1;
    cl->inactive = 1;
    cl->score = 0;
   
}



void add_new_client(int fd, client* array, int len) {
    client *cl;
    int i;
    cl = make_client(fd);
    i = find_first_empty(array, len);
    if (i < 0) {
        printf("spatne add new client");
        free(cl);
        return;
    }
    
    printf("New client on index: %d\n", i);
    memcpy((array+i), cl, sizeof(client));
    free(cl);
}

int find_first_empty(client *array, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (!array[i].taken) {
            return i;
        }
    }
    return -1;
}

int find_client_by_fd(int fd, client *array, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (array[i].client_fd == fd && array[i].client_fd >= 0) {
            return i;
        }
    }
    return -1;
}

int find_rival_to_client(int fd, client *array, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (array[i].game == -1) {
            if (array[i].client_fd != fd && !array[i].inactive) {
                return i;
            }
        }
    }
    return -1;
}

void remove_client(client *cl) {
   /* int i;
    i = find_client_by_fd(fd, array, len);
    if (i >= 0) {*/
        memset(cl, 0, sizeof(client));
        printf("odpojil se klient.\n");
    
    
}

client *find_client_by_secret(client *array, int secret, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (array[i].secret == secret) {
            printf("client: %d\n", i);
            return &array[i];        
        }
    }
    return NULL;
}

int get_secret() {
    int secret;
    secret = rand() % (99999 + 1 - 10000) + 10000;
    return secret;

}
