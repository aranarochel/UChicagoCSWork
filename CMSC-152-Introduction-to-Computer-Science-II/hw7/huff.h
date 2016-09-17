/* Jaime Arana-Rochel, aranarochel */
/* CS152, Winter 2013 */
/* Homework 5 */
#ifndef HUFF_H
#define HUFF_H

typedef struct leaf leaf;
typedef struct node node;
typedef struct huff huff;

enum huff_tag { LEAF, NODE };

struct leaf {
  char c;
  int n;
};

struct node {
  int n;
  huff *lsub;
  huff *rsub;
};

union huff_union {
  leaf leaf;
  node node;
};

struct huff {
  enum huff_tag tag;
  union huff_union h;
};

struct hll {
  huff *huff;
  struct hll *next;
};
typedef struct hll hll;

/* Construct a singleton Huffman tree from a character and a count. */
huff *huff_singleton(char c, int n);

/* Constructs a huff node with NULL subtrees */
huff *huff_single_node(int n);

/* Return the weight of a Huffman tree. */
int huff_weight(huff *h);

/* Merge two Huffman trees. */
/* See step 2 of the algorithm on the web page for details. */
huff *huff_merge(huff *h1, huff *h2);

/* Display a Huffman tree at the console. */
/* Essential for development and debugging. */
void huff_show(huff *h);

/* Builds a huff linked list */
hll *hll_cons(huff *h,hll *hs);

/* returns the length of a huff list */
int hll_length(hll *hs);

/* inserts a huff into a huff list */
hll *hll_insert(huff *h,hll *hs);

/* insertion sort of a huff linked list */
hll *hll_sort(hll *hs);

/* count occurences of a char in a string */
int count(char c,char *s);

/* converts a string into a sorted huff linked list */
hll *word_to_hll(char *s);

/* removes given huff from huff linked list */
hll *hll_rem(huff *h,hll *hs);

/* converts a string into a huffman tree */
huff *word_to_huff(char *s);

  
#endif /* HUFF_H */

