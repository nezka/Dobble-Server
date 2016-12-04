

#ifndef CLIENT_H
#define CLIENT_H

typedef struct client {
    char name[255];
    int client_fd;
    int secret;
    int bad_messages;
    int taken;
} client;


void remove_client(int fd, client *array, int len);
client *make_client(int fd);
int find_first_empty(client *array, int len);

#endif /* CLIENT_H */

