

#ifndef CLIENT_H
#define CLIENT_H

typedef struct client {
    char name[255];
    int client_fd;
    int secret;
    int bad_messages;
    int taken;
    int game;
    int score;
} client;


void remove_client(int fd, client *array, int len);
client *make_client(int fd);
int find_first_empty(client *array, int len);
int find_rival_to_client(int fd, client *array, int len);
void add_name_to_client(client *cl, char *name, int len);

#endif /* CLIENT_H */

