

#ifndef CLIENT_H
#define CLIENT_H

typedef struct client {
    char name[10];
    int client_fd;
    int secret;
    int taken;
} client;


void remove_client(int fd, client *array, int len);
void add_name(client *cl, char *name, int len);
client *make_client(int fd);
void add_new_client(int fd, client* array, int len);
int find_first_empty(client *array, int len);

#endif /* CLIENT_H */

