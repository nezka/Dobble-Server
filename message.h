

#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct message {
    char type;
    char subtype;
    int len;
    char *str;
} message;


#endif /* MESSAGE_H */

