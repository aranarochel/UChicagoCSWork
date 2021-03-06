#ifndef _HUFF_H
#define _HUFF_H

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

/* Construct a singleton Huffman tree from a character and a count. */
huff *huff_singleton(char c, int n);

/* Return the weight of a Huffman tree. */
int huff_weight(huff *h);

/* Merge two Huffman trees. */
/* See step 2 of the algorithm on the web page for details. */
huff *huff_merge(huff *h1, huff *h2);

/* Display a Huffman tree at the console. */
/* Essential for development and debugging. */
void huff_show(huff *h);

#endif /* _HUFF_H */
