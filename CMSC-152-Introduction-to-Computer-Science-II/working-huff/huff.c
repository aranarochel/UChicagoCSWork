#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "huff.h"

void bad_tag(char *f)
{
  fprintf(stderr,"error in %s: unrecognized tag\n",f);
}

int huff_weight(huff *h)
{
  switch (h->tag) {
  case LEAF:
    return h->h.leaf.n;
  case NODE:
    return h->h.node.n;
  }
  bad_tag("huff_weight");
  exit(1);
}

huff *huff_singleton(char c, int n)
{
  huff *new = malloc(sizeof(huff));
  new->tag = LEAF;
  new->h.leaf.c = c;
  new->h.leaf.n = n;
  return new;
}

huff *huff_node(huff *h0, huff *h1)
{
  huff *new = malloc(sizeof(huff));
  new->tag = NODE;
  new->h.node.n = huff_weight(h0) + huff_weight(h1);
  new->h.node.lsub = h0;
  new->h.node.rsub = h1;
  return new;
}

/* helper function for display of huff trees */
void hsl(int level, huff *h)
{
  int i;
  for (i=0; i<level; i++)
    putchar('+');
  switch (h->tag) {
  case LEAF:
    printf("LEAF(%c,%i)\n",h->h.leaf.c,h->h.leaf.n);
    return;
  case NODE:
    printf("NODE(%i)\n",h->h.node.n);
    hsl(level+1,h->h.node.lsub);
    hsl(level+1,h->h.node.rsub);
    return;
  }
  bad_tag("huff_show");
  exit(1);
}

void huff_show(huff *h)
{
  hsl(0,h);
}

huff *huff_merge(huff *h1, huff *h2)
{
  int w = huff_weight(h1) + huff_weight(h2);
  huff *new = malloc(sizeof(huff));
  new->tag = NODE;
  new->h.node.n = w;
  new->h.node.lsub = h1;
  new->h.node.rsub = h2;
  return new;
}
