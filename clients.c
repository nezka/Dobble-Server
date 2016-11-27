
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
    cl->client_fd = fd;
    cl->secret = 5;
    cl->taken = 1;
    return cl;
}

void add_name(client *cl, char *name, int len) {
    strncpy(cl->name, name, len);
}

void add_new_client(int fd, client* array, int len) {
    client *cl;
    int i;
    cl = make_client(fd);
    i = find_first_empty(array, len);
    if (i < 0) {
        printf("kuk");
        return;
    }
    printf("index v poli: %d\n", i);
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
        if (array[i].client_fd == fd) {
            return i;
        }
    }
    return -1;
}

void remove_client(int fd, client *array, int len) {
    int i;
    i = find_client_by_fd(fd, array, len);
    if (i >= 0) {
        memset((array+i), 0, sizeof(client));
        printf("odpojil se: %d\n", i);
    }
    
}
