#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "structs.h"
#include "channels.h"
#include "list.h"



/* ====== Functions ===== */

// Construct a new channel cnode
cnode *new_cnode(channel* channel)
{
  cnode * new = (cnode*)malloc(sizeof(cnode));
  new->channel = channel;
  new->next = NULL;
  return new;
}

// Create a new clist
clist *new_clist(void)
{
  clist *new = (clist*)malloc(sizeof(clist));
  new->head = NULL;
  return new;
}

// Add a channel to a clist
void add_to_clist(clist *l, channel* channel)
{
  cnode *new = new_cnode(channel);
  new->next = l->head;
  l->head = new;
}

channel *add_new_channel(clist *l, char* cname)
{
  channel *newChannel = (channel*)malloc(sizeof(channel));
  newChannel->cname = strdup(cname);
  newChannel->userList = new_list();
  newChannel->topic = strdup("");
  strcpy(newChannel->umode,"");
  strcpy(newChannel->cmode,"");
  add_to_clist(l, newChannel);
  return newChannel;
}

// removes the first channel in a clist
void remove_first_channel(clist *l)
{
  if (l->head == NULL) {
    return;
  }

  cnode *original_head = l->head;
  l->head = original_head->next;
  free(original_head);
}


// removes the specified channel from the clist
char* remove_from_clist(clist *l, channel* channel)
{
  cnode *iter = l->head;
  cnode *being_deleted;

  // if empty clist
  if (!iter)
    return "no iter";

  // channel name matches the first channel's name in clist
  if (!strcmp(iter->channel->cname, channel->cname)) {
    remove_first_channel(l);
    return "removed first";
  }

  // iterate through clist to find the channel location
  while (iter->next && iter->next->channel->cname != channel->cname) {
    iter = iter->next;
  }

  // channel not in clist
  if (iter->next == NULL)
    return "not in clist";

  // found channel, and remove from clist
  char* cname = strdup(iter->next->channel->cname);
  iter->next = iter->next->next;
  free(channel);
  return cname;
}


// return clist size
int clist_size(clist *l)
{
  if (l == NULL) 
    return 0;

  cnode *temp = l->head;

  int count = 0;

  while (temp != NULL) {
    count++;
    temp = temp->next;
  }

  return count;
}


// return 1 if empty, 0 otherwise
int is_clist_empty(clist *l)
{
  if (l->head == NULL)
    return 1;
  return 0;
}


// loops through clist and checks whether a specific channel is present
// checks this by looking up cnames in global_channel_clist
int is_cname_present(clist *l, char *cname)
{
  cnode *iter = l->head;

  // if empty clist
  if (!iter)
    return 0;

  // cname matches the first channel's cname in clist
  if (strcmp(iter->channel->cname,cname) == 0) 
    return 1;
 

  // iterate through clist to find the channel location
  while (iter->next && strcmp(iter->next->channel->cname,cname) != 0) {
    iter = iter->next;
  }

  // cname not in clist
  if (iter->next == NULL)
    return 0;

  // found channel, so return 1
  return 1;
}

channel *fetch_channel_by_cname(clist *l, char *cname) 
{

  cnode *iter = l->head;
  channel *fetched_channel;

  // if empty clist
  if (!iter)
    return NULL;

  // cname matches the first channel's cname in clist
  // return channel's channel struct
  if (strcmp(iter->channel->cname,cname) == 0) {
    fetched_channel = (channel*)malloc(sizeof(channel));
    fetched_channel = iter->channel;
    return fetched_channel;
  }


  // iterate through clist to find the channel location
  while (iter->next && strcmp(iter->next->channel->cname,cname) != 0) {
    iter = iter->next;
  }

  // cname not in clist
  if (iter->next == NULL)
    return NULL;

  // found cname in clist, return channel's channel struct
  fetched_channel = (channel*)malloc(sizeof(channel));
  fetched_channel = iter->next->channel;
  return fetched_channel;
}

void leave_all_channels(clist *l,user *userInfo)
{

  cnode *citer = l-> head;
  // loop through all channels
  while(citer !=NULL)
  {
    list *userList = citer->channel->userList;

    remove_from_list(userList,userInfo);

    citer = citer->next;
  }
}

char* fetch_channels(clist *l)
{
  cnode *copy = (cnode*)malloc(sizeof(cnode));
  copy = l->head;

  int total_chars = 0;
  while(copy != NULL) {
    total_chars = strlen(copy->channel->cname) + total_chars;
    copy = copy->next;
  }

  // string to hold all nicks in user list
  char *channels = (char*)malloc(total_chars*sizeof(char) + 1);
 
  cnode *iter = l->head;

  
  while(iter != NULL) {
    channels = strcat(channels, iter->channel->cname);
    
    if(iter->next != NULL)
      channels = strcat(channels, " ");

    iter = iter->next;
  }
  free(copy);
  return channels;
}

void remove_mode(char* s, char c)
{
  int len = strlen(s);
  int found = 0;
  for (int i=0; i<len; i++)
  {
    if(s[i]==c)
      found = 1;
    if(found)
    {
      s[i]=s[i+1];
    }
  }
  if (found)
    s[len-1] = '\0';
}
void add_mode(char* s, char c)
{
  int len = strlen(s);
  s[len] = c;
  s[len+1] = '\0';
}
int is_char_present(char* string, char c)
{
  int i = 0;
  while(string[i] != '\0')
  {
    if(string[i] == c)
      return 1;
    i++;
  }
  return 0;
}
