#ifndef _SLL_H
#define _SLL_H

/* linked lists of strings */

typedef struct sll sll;

struct sll {
  char *s;
  sll *next;
};

/* By convention, the empty list is NULL. */

/* sll_cons : (char*, sll*) -> sll* */
/* build new list with given string at the head */
/* note: copy the given string to the list (deep cons) */
sll *sll_cons(char *s, sll *ss);

/* sll_length : sll* -> int */
/* return the number of strings in the given list */
int sll_length(sll *ss);

/* sll_member : (char*, sll*) -> int */
/* test membership of given string in given list */
/* use strcmp to compare strings */
int sll_member(char *s, sll *ss);

/* sll_show : sll* -> <void> */
/* print a representation of the linked list to stdout */
void sll_show(sll *ss);

/* sll_free : sll* -> <void> */
void sll_free(sll *ss);

#endif /* _SLL_H */
