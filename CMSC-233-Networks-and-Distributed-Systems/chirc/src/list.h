#ifndef LIST_H
#define LIST_H

#include "structs.h"

/*   Global user list   */
list *global_user_list;


/* ====== Functions ===== */

// Construct a new user node
node *new_node(user* user);

// Create a new list
list *new_list(void);

// Add a user to a list
void add_to_list(list *l, user* user);


// removes the first user in a list
void remove_first(list *l);

// removes the specified user from the list
void remove_from_list(list *l, user* user);

// return list size
int list_size(list *l);

// return 1 if empty, 0 otherwise
int is_empty(list *l);

// loops through list and checks whether a specific user is present
// checks this by looking up nicknames in global_user_list
int is_nick_present(list *l, char *nick);

// same as "is_nick_present" but checks for usernames in list
int is_username_present(list *l, char *username);

// same as "is_username_present" but checks for clent socket conection
int is_socket_connected(list *l, int socket);

// loop through list and check if a certain nick is in global list
// if yes, retreive and return the user struct which contains the user's info for sending messages
// if not, return NULL, signifying that no user was found with that nickname
user *fetch_user_by_nick(list *l, char *nick); 

user *fetch_user_by_socket(list *l, int socket); 

char* fetch_users(list *l);

char *fetch_users_notJoined(list *l);

#endif
