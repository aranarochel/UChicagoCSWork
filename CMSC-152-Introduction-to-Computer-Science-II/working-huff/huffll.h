#ifndef _HUFFLL_H
#define _HUFFLL_H
#include "huff.h"

typedef struct huffll huffll;

struct huffll {
  huff *h;
  huffll *next;
};

huffll *huffll_cons(huff *h, huffll *hs);

/* append two lists destructively, that is, */
/* link the end of the first directly to the beginning of the second */
huffll *huffll_append(huffll *t0, huffll *t1);

void huffll_show(huffll *hs);

/* insert by ascending order by weight */
huffll *huffll_insert(huff *h, huffll *hs);

/* insertion sort, ascending order by weight */
huffll *huffll_isort(huffll *hs);

/* coalesce many Huffman trees into one */
/* by repeatedly merging first two */
huff *huffll_coalesce(huffll *hs);

#endif /* _HUFFLL_H */
