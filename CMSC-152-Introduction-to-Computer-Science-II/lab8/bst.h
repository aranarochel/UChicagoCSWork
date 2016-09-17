#ifndef _BST_H
#define _BST_H

#include "vcard.h"

typedef struct bst bst;

struct bst {
  vcard *c;
  bst *lsub;
  bst *rsub;
};

bst *bst_singleton(vcard *c);

/* Insert a vcard into a BST and return a pointer to the modified tree. */
/* Use cnet alphabetical order. */
bst *bst_insert(bst *t, vcard *c);

/* Compute the total number of vcards in the tree. */
unsigned int bst_num_entries(bst *t);

/* The empty bst has height 0. */
/* A singleton tree has height 1. */
/* Et cetera. */
unsigned int bst_height(bst *t);

/* Return NULL is nothing is found. */
/* n_comparisons is an out parameter to count the number of */
/* comparisons made during search. */
vcard *bst_search(bst *t, char *cnet, int *n_comparisons);

void bst_free(bst *t);

#endif /* _BST_H */
