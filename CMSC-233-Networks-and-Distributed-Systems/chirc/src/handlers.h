#ifndef HANDLERS_H
#define HANDLERS_H
#include "structs.h"

// *** 1a *** //
int handle_NICK(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);
int handle_USER(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);
int handle_internal_WELCOME(cmdValueStruct cmdValueInstance,user* userInfo,list* userList);

// *** 1b ***//
int handle_QUIT(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);

int handle_PRIVMSG(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);
int handle_NOTICE(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);

int handle_PING(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);	// incomplete
int handle_PONG(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);	// incomplete

int handle_MOTD(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);
int handle_LUSERS(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);
int handle_WHOIS(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);

int handle_UNKNOWN(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);

// *** 1c *** //
int handle_JOIN(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);
int handle_PART(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);
int handle_TOPIC(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);
int handle_OPER(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);
int handle_MODE(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);
int handle_internal_MODE_at_channel(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);
int handle_internal_MODE_at_user(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);
int handle_AWAY(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);
int handle_NAMES(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);
int handle_LIST(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);
int handle_WHO(cmdValueStruct cmdValueInstance, user* userInfo, list* userList);

#endif
