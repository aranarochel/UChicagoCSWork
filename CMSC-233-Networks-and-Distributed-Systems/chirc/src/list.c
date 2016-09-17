#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "structs.h"
#include "list.h"



/* ====== Functions ===== */

// Construct a new user node
node *new_node(user* user)
{
  node * new = (node*)malloc(sizeof(node));
  new->user = user;
  new->next = NULL;
  return new;
}


// Create a new list
list *new_list(void)
{
  list *new = (list*)malloc(sizeof(list));
  new->head = NULL;
  return new;
}

// Add a user to a list
void add_to_list(list *l, user* user)
{
  node *new = new_node(user);
  new->next = l->head;
  l->head = new;
}


// removes the first user in a list
void remove_first(list *l)
{
  if (l->head == NULL) {
    return;
  }

  node *original_head = l->head;
  l->head = original_head->next;
  free(original_head);
}


// removes the specified user from the list
void remove_from_list(list *l, user* user)
{
  node *iter = l->head;
  node *being_deleted;

  // if empty list
  if (!iter)
    return;

  // user nick matches the first user's nick in list
  if (iter->user->nick == user->nick) {
    remove_first(l);
    return;
  }

  // iterate through list to find the user location
  while (iter->next && iter->next->user->nick != user->nick) {
    iter = iter->next;
  }

  // user not in list
  if (iter->next == NULL)
    return;

  // found user, and remove from list
  being_deleted = iter->next;
  iter->next = iter->next->next;

  free(being_deleted);
}


// return list size
int list_size(list *l)
{
  if (l == NULL) 
    return 0;

  node *temp = l->head;

  int count = 0;

  while (temp != NULL) {
    count++;
    temp = temp->next;
  }

  return count;
}


// return 1 if empty, 0 otherwise
int is_empty(list *l)
{
  if (l->head == NULL)
    return 1;
  return 0;
}


// loops through list and checks whether a specific user is present
// checks this by looking up nicknames in global_user_list
int is_nick_present(list *l, char *nick)
{
  node *iter = l->head;

  // if empty list
  if (!iter)
    return 0;

  // nick matches the first user's nick in list
  if (strcmp(iter->user->nick,nick) == 0) 
    return 1;
 

  // iterate through list to find the user location
  while (iter->next && strcmp(iter->next->user->nick,nick) != 0) {
    iter = iter->next;
  }

  // nick not in list
  if (iter->next == NULL)
    return 0;

  // found user, so return 1
  return 1;
}


// same as "is_nick_present" but checks for usernames in list
int is_username_present(list *l, char *username)
{
  node *iter = l->head;

  // if empty list
  if (!iter)
    return 0;

  // nick matches the first user's nick in list
  if (strcmp(iter->user->username,username) == 0)
    return 1;


  // iterate through list to find the user location
  while (iter->next && strcmp(iter->next->user->username,username) != 0) {
    iter = iter->next;
  }

  // nick not in list
  if (iter->next == NULL)
    return 0;

  // found user, so return 1
  return 1;
}

int is_socket_connected(list *l, int socket)
{
  node *iter = l->head;

  // if empty list
  if (!iter)
    return 0;

  // socket matches the first user's socket in list
  if (iter->user->clientSocket == socket)
    return 1;


  // iterate through list to find the user location
  while (iter->next && (iter->next->user->clientSocket != socket)) {
    iter = iter->next;
  }

  // socket not in list
  if (iter->next == NULL)
    return 0;

  // found socket, so return 1
  return 1;
}



// loop through list and check if a certain nick is in global list
// if yes, retreive and return the user struct which contains the user's info for sending messages
// if not, return NULL, signifying that no user was found with that nickname
user *fetch_user_by_nick(list *l, char *nick) 
{

  node *iter = l->head;
  user *fetched_user;

  // if empty list
  if (!iter)
    return NULL;

  // nick matches the first user's nick in list
  // return user's user struct
  if (strcmp(iter->user->nick,nick) == 0) {
    fetched_user = (user*)malloc(sizeof(user));
    fetched_user = iter->user;
    return fetched_user;
  }


  // iterate through list to find the user location
  while (iter->next && strcmp(iter->next->user->nick,nick) != 0) {
    iter = iter->next;
  }

  // nick not in list
  if (iter->next == NULL)
    return NULL;

  // found nick in list, return user's user struct
  fetched_user = (user*)malloc(sizeof(user));
  fetched_user = iter->next->user;
  return fetched_user;
}

user *fetch_user_by_socket(list *l, int socket) 
{

  node *iter = l->head;
  user *fetched_user;

  // if empty list
  if (!iter)
    return NULL;

  // nick matches the first user's nick in list
  // return user's user struct
  if (iter->user->clientSocket == socket) {
    fetched_user = (user*)malloc(sizeof(user));
    fetched_user = iter->user;
    return fetched_user;
  }


  // iterate through list to find the user location
  while (iter->next && (iter->next->user->clientSocket != socket)) {
    iter = iter->next;
  }

  // socket not in list
  if (iter->next == NULL)
    return NULL;

  // found socket in list, return user's user struct
  fetched_user = (user*)malloc(sizeof(user));
  fetched_user = iter->next->user;
  return fetched_user;
}

// returns a string of space separated users connected to given list
// used to get all nicks in a channel
char* fetch_users(list *l)
{
  node *copy = (node*)malloc(sizeof(node));
  copy = l->head;

  int total_chars = 0;
  while(copy != NULL) {
    total_chars = strlen(copy->user->nick) + total_chars;
    copy = copy->next;
  }

  // string to hold all nicks in user list
  char *nicks = (char*)malloc(total_chars*sizeof(char) + 1);
 
  node *iter = l->head;

  
  while(iter != NULL) {

    if(iter->user->mode == 'o') {
      nicks = strcat(nicks,"@");
      nicks = strcat(nicks, iter->user->nick);
    }
    else if (is_char_present(iter->user->userChannels->head->channel->umode, 'o')) {
      nicks = strcat(nicks, "@");
      nicks = strcat(nicks, iter->user->nick);
    }
    else if (is_char_present(iter->user->userChannels->head->channel->umode, 'v')) {
      nicks = strcat(nicks, "+");
      nicks = strcat(nicks, iter->user->nick);
    }
    else {
      nicks = strcat(nicks, iter->user->nick);
    }

    if(iter->next != NULL)
      nicks = strcat(nicks, " ");

    iter = iter->next;
  }
  free(copy);
  return nicks;
}

// returns a string of space separated users not joined in a channel
char *fetch_users_notJoined(list *l)
{
  node *copy = (node*)malloc(sizeof(node));
  copy = l->head;

  int total_chars = 0;
  while(copy != NULL) {
    if (copy->user->userChannels->head == NULL)
      total_chars = strlen(copy->user->nick) + total_chars;
    copy = copy->next;
  }

  // string to hold all nicks in user list
  char *nicks = (char*)malloc(total_chars*sizeof(char) + 1);

  node *iter = l->head;

  
  while(iter != NULL) {

    if(iter->user->userChannels->head == NULL) {
      if (iter->user->mode == 'o') {
	nicks = strcat(nicks, "@");
	nicks = strcat(nicks, iter->user->nick);
      }
      else if (is_char_present(iter->user->userChannels->head->channel->umode, 'o')) {
	nicks = strcat(nicks, "@");
	nicks = strcat(nicks, iter->user->nick);
      }
      else if (is_char_present(iter->user->userChannels->head->channel->umode, 'v')) {
	nicks = strcat(nicks, "+");
	nicks = strcat(nicks, iter->user->nick);
      }
      else {
	nicks = strcat(nicks, iter->user->nick);
      }
      
      if(iter->next != NULL)
	nicks = strcat(nicks, " ");
    }
    iter = iter->next;
  }
  free(copy);
  return nicks;
}
