#ifndef _HTBL_H
#define _HTBL_H

#include "sll.h"

/* hash table of strings, with linked list buckets */

struct hash_table {
  int n_buckets;
  sll **buckets; /* an array of pointers to string lists */
};

typedef struct hash_table htbl;

/* hash : char -> unsigned int */
/* compute hash code for given string */
/* see web page for algorithm */
unsigned long hash(char *s);

/* htbl_init : int -> htbl* */
/* allocate space for a new hash table of given size */
/* all buckets must initially contain the empty list */
htbl *htbl_init(int sz);

/* htbl_n_entries : htbl* -> unsigned int */
/* return the number of entries in all buckets */
unsigned int htbl_n_entries(htbl *t);

/* htbl_load_factor : htbl* -> float */
/* The load factor of a hash table is the mean number */
/*   of elements per bucket. */
float htbl_load_factor(htbl *t);

/* htbl_add : (char*, htbl*) -> int */ 
/* add string s to hast table t */
/* return the number of strings in s's bucket _after_ inserting it */
int htbl_add(char *s, htbl *t);

/* htbl_member : (char*, htbl*) -> int */
/* test membership of given string in given table */
/* the integer returned is a quasi-boolean */
int htbl_member(char *s, htbl *t);

/* htbl_show : htbl* -> <void> */
/* print a represntation of the hash table to stdout */
void htbl_show(htbl *t);

/* htbl_free : htbl* -> <void> */
void htbl_free(htbl *t);

#endif /* _HTBL_H */
