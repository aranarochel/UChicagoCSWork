#ifndef FUNCTIONS_H
#define FUNCTIONS_H
/* Functions used in our main.c code */
#include "structs.h"
#include <netinet/in.h>
#include "list.h"
#include "handlers.h"
#include "reply.h"

#define BUFFER_SIZE 512

user* create_user(char *nick, char *username, char *fullName, int clientSocket, int welcomed, char *clientIP, char *hostname, char *password);

void trim_CRLF(char* msg);

cmdValueStruct parse_message(char* msg);

int check_for_newline(char* str, int bytes);

char* receive_message(int clientSocket, char* partial_msg);

int reply_to_one(messageInfoStruct messageInfo, user* userInfo);

int reply_to_all(messageInfoStruct messageInfo, user* userInfo, clist* channels);

int reply_to_channel(messageInfoStruct messageInfo, user* userInfo, channel *channel);


#endif
