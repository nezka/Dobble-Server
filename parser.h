
#ifndef PARSER_H
#define PARSER_H
#include "message.h"

void prepare_message_reverse(char *dst, const char *src, int len);
void prepare_message_broadcast(char *dst);
message *parse_message(char *rcvBuf, int len);

#endif /* PARSER_H */

