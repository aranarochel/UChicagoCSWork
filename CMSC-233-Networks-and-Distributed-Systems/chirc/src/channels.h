#ifndef CHANNELS_H
#define CHANNELS_H

#include "structs.h"

/*   Global channel clist   */
clist *global_channel_clist;


/* ====== Functions ===== */

// Construct a new channel cnode
cnode *new_cnode(channel* channel);

// Create a new clist
clist *new_clist(void);

// creates a new channel with cname from param and an empty user list
// then adds it to the given list
channel *add_new_channel(clist *l, char* cname);

// Add a channel to a clist
void add_to_clist(clist *l, channel* channel);


// removes the first channel in a clist
void remove_first_channel(clist *l);

// removes the specified channel from the clist
char* remove_from_clist(clist *l, channel* channel);

// return clist size
int clist_size(clist *l);

// return 1 if empty, 0 otherwise
int is_clist_empty(clist *l);

// loops through clist and checks whether a specific channel is present
// checks this by looking up cname in global_channel_clist
int is_cname_present(clist *l, char *cname);

channel *fetch_channel_by_cname(clist *l, char *cname); 

void leave_all_channels(clist *l,user *userInfo);

char* fetch_channels(clist *l);

void remove_mode(char* modestring, char c);

void add_mode(char* s, char c);

int is_char_present(char* string, char c);

#endif
