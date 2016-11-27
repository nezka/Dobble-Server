
#ifndef PARSER_H
#define PARSER_H
#include "message.h"

void prepare_message_reverse(char *dst, const char *src, int len);
void prepare_message_broadcast(char *dst);
message *process_message(char *rcvBuf, char *sendBuf, int len);

#endif /* PARSER_H */

