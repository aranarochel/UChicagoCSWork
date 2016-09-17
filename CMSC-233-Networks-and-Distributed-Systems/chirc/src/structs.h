#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdio.h>


//structs.h
// A file to hold all the structs that will be used throughout all our code
struct user_info;
struct clist;

char* global_password;
/*		For handlers.c 		*/

// This will hold a command and a string, for example "USER rory"
// or "PRVMSG rory :sup babe luv amy"
// or "MOTD"
// It will be used to pass in information to the handlers from the main
typedef struct _cmdValueStruct
{
  char *cmd;
  char *value;
  char *message;
} cmdValueStruct;


// This will hold a message response being passed from the handlers to the
// sending functions. For example, "RPL_WELCOME" and "Welcome to irc...", or
// "QUIT" and "See ya suckers", etc 
typedef struct _messageInfoStruct
{
	char *messageType;
	char *messageContents;
} messageInfoStruct;


/*    For main.c    */
// struct to hold user information
struct user
{
  char *nick;           //nickname
  char *username;       //username
  char *fullName;       //user's full name
  int welcomed;         //whether or not they have been sent RPL_WELCOME

  int clientSocket;     //The socket they are connected to
  char *clientIP;       //the ip of the client computer
  char *hostname;       //The hostname they are connected to - the server ip

  struct clist *userChannels;  //The list of channels the user is connected to

  char mode;
  char *away_message;
  char *password;
};
typedef struct user user;

/*             For list.c                */
struct node {
  user *user;
  struct node *next;
};
typedef struct node node;

struct list {
  node *head;
};
typedef struct list list;


/*              For channels.c            */
struct channel
{
  char *cname;    // channel name
  list *userList;     // list of connected users
  char *topic;
  char umode[128];   //user mode
  char cmode[128];   //channel mode
};
typedef struct channel channel;

struct cnode {
  channel *channel;
  struct cnode *next;
};
typedef struct cnode cnode;

struct clist {
  cnode *head;
};
typedef struct clist clist;




#endif
