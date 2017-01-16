

#ifndef CLIENT_H
#define CLIENT_H


typedef struct client {
    int client_fd;
    int secret;
    int taken;
    int game;
    int score;
    int inactive;
} client;


void remove_client(client *cl);
client *make_client(int fd);
int find_first_empty(client *array, int len);
int find_rival_to_client(int fd, client *array, int len);
client *find_client_by_secret(client *array, int secret, int len);
int get_secret();

#endif /* CLIENT_H */

