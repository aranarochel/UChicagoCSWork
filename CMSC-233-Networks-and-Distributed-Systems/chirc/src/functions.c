#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "reply.h"
#include "list.h"
#include "channels.h"
#include "structs.h"
#include "functions.h"
#include "handlers.h"
#include <time.h>
#include <pthread.h>

#define BUFFER_SIZE 512


// creates a user - note that we will have to create a user right when the user connects - right after we give them their own thread
user* create_user(char *nick, char *username, char *fullName, int clientSocket, int welcomed, char *clientIP, char *hostname, char *password)
{
  user *new_user = (user*)malloc(sizeof(user));
  new_user->nick = strdup(nick);
  new_user->username = strdup(username);
  new_user->fullName = strdup(fullName);
  new_user->welcomed = welcomed;
  new_user->clientSocket = clientSocket;
  new_user->clientIP = strdup(clientIP);
  new_user->hostname = strdup(hostname);
  new_user->userChannels = new_clist();
  new_user->password = strdup(password);

  return new_user;
}

// Gets rid of the '\r' and '\n' characters from client messages
// and turns them into strings
void trim_CRLF(char* msg)
{
  int len = strlen(msg);
  int i = len - 1;

  while (i >= 0 && (msg[i] == '\n' || msg[i] == ' ' || msg[i] == '\r')) 
  {
    --i;
  }
  
  msg[i+1] = '\0';
}


// parses the client message and stores the command and params in the cmdValueStruct
cmdValueStruct parse_message(char* msg)
{
  cmdValueStruct parsed = {NULL,NULL,NULL};

  trim_CRLF(msg);

  // First, separate
  char *saveptr;

  parsed.cmd = strtok_r(msg, " ", &saveptr);
  parsed.value = strtok_r(NULL, " ", &saveptr);
  parsed.message = strtok_r(NULL, "", &saveptr);

  return parsed;

}

// checks for the \n char in a message
// used by receive_message()
int check_for_newline(char* str, int bytes) 
{
  int i;
  int yes = -1;

  for (i = 0; i < bytes; i++) 
  {
    if (str[i] == '\n') 
    {
      yes = i;
      break;
    }
  }

  return yes;
}

/* returns a complete client message. The partial_msg
   parameter is the char array which will hold the contents of the 
   buffer from recv */
char* receive_message(int clientSocket, char* partial_msg)
{
  int bytes = -1;
  int has_newline = -1;
  char *new_msg;
  char *tok;
  char *rest;
  char buffer[BUFFER_SIZE];


  /* This first test is so that we won't call recv again if we already have
     a command in partial_msg, as this was causing Reply_Timeout_Error before */
  has_newline = check_for_newline(partial_msg, strlen(partial_msg));

  if(has_newline >0) 
  {
    // retreive the message from partial_msg and store in new_msg
    tok = strtok(partial_msg, "\r\n");
    new_msg = strdup(tok);

    /* make sure to store the rest partial_msg for later
       in case partial_msg contained more than one message */
    rest = strtok(NULL, "");
    char *str = strdup(rest+1);
    int len = strlen(str);

    partial_msg[0] = '\0';
    strncat(partial_msg, str, len);

    return new_msg;
  }
  else 
  {
    // receive message from client
    bytes = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    
    // concat buffer onto partial_msg 
    strncat(partial_msg, buffer, bytes);
    
    // check if partial_msg has a newline char, indicating a full message
    has_newline = -1;
    has_newline = check_for_newline(partial_msg, strlen(partial_msg));
  
    // only received a partial message, return NULL and wait for next receive_message call
    if(has_newline < 0) 
    {
      return NULL;
    }
    // partial_msg contains a full message (could be 1, 2, etc.)
    else
    {
      // retreive the message from partial_msg and store in new_msg
      tok = strtok(partial_msg, "\r\n");
      new_msg = strdup(tok);
      // make sure to store the rest of partial_msg for later
	    // in case partial_msg contained more than one message 
      rest = strtok(NULL, "");
      char *str = strdup(rest+1);
      int len = strlen(str);

      partial_msg[0] = '\0';
      strncat(partial_msg, str, len);

      return new_msg;
    }
  }
  
  return new_msg;
}


/*    Functions for handlers    */

// send a formatted reply to a sender specified by the connection information provided in userInfo
// we may be able to remove the user struct portion of this.
int reply_to_one(messageInfoStruct messageInfo, user* userInfo)
{
  // Make sure that a target is specified
  if(userInfo->clientSocket < 0)
  {
    printf("reply_to_one() failed: no target client specified\n");
    return -1;
  }

  char msg[1023];
  char hostname[128];

  char *messageType = messageInfo.messageType;
  char *messageContents = messageInfo.messageContents;
  int clientSocket = userInfo->clientSocket;
  strcpy(hostname,userInfo->hostname);
  char *nick = userInfo->nick;

  // FROM USERS TO USERS
  /* unlike other messages, PRIVMSGS look to users as if they're coming from other users (not from the server)
  // therefore, we have to make it appear that way.
  // Similarly, NICK (changing nicknames) does not seem to have the uniform prefix, which is stupid
  */
  // Note from 1c: now i feel dumb. we should have done something about different prefixes earlier on
  if (!strcmp(messageType,"PRIVMSG") || !strcmp(messageType,"NOTICE") || !strcmp(messageType, "NICK") ||!strcmp(messageType, "QUIT") || !strcmp(messageType,"JOIN") || !strcmp(messageType,"PART") || !strcmp(messageType, "TOPIC") || !strcmp(messageType, "MODE") || !strcmp(messageType, RPL_LIST))
  {
      snprintf(msg, 1023, "%s\r\n",messageContents);
  }
  // ERRORS TO USERS
  else if(!strcmp(messageType, ERR_NICKNAMEINUSE)) {
  snprintf(msg, 1023, ":%s %s * %s\r\n",hostname,messageType,messageContents);
  }
  // FROM SERVER TO USERS					
  else 
  {
      snprintf(msg, 1023, ":%s %s %s %s\r\n",hostname,messageType,(!strcmp(nick,"")? "*" : nick),messageContents);
  }
  // we'll see if messageType comes out the right way, converted into a number.
  // if not, we will probably have to remove the null terminator

  printf("reply_to_one(%s) sent: %s",nick,msg);
  send(clientSocket, msg, strlen(msg), 0);

  return 1;

}


int reply_to_channel(messageInfoStruct messageInfo, user* userInfo, channel *channel)
{
  node *iter = channel->userList->head;

  // iterate through list to find the user location
  while (iter != NULL) 
  {
    if (strcmp(iter->user->nick, userInfo->nick) || !strcmp(messageInfo.messageType, "PART") || !strcmp(messageInfo.messageType, "TOPIC") || !strcmp(messageInfo.messageType, "MODE") || !strcmp(messageInfo.messageType, "NICK")) {
      if (iter->user->welcomed == 1){
	      reply_to_one(messageInfo, iter->user);
      }

    }
    iter = iter->next;
  }

  return 1;
}



// send a formatted reply to everyone connected
// ****** 1c update: this needs to be changed to loop not through all users, but
// all users connected to the channels the user is connected to
int reply_to_all(messageInfoStruct messageInfo, user* userInfo, clist *channels)
{
  cnode *citer = channels->head;
  while(citer!=NULL)
  {
      reply_to_channel(messageInfo, userInfo, citer->channel);
      citer = citer->next;
  }
  return 1;
}
