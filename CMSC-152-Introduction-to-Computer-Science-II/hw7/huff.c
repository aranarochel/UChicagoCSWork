/* Jaime Arana-Rochel, aranarochel */
/* CS152, Winter 2013 */
/* Homework 5 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "huff.h"

void bad_tag(char *f)
{
  fprintf(stderr,"error in %s: unrecognized tag\n",f);
}


huff *huff_singleton(char c, int n)
{
  huff *new = (huff*)malloc(sizeof(huff));
  new->tag = LEAF;
  new->h.leaf.c = c;
  new->h.leaf.n = n;
  return new;
}

huff *huff_single_node(int n)
{
  huff *new = (huff*)malloc(sizeof(huff));
  new->tag = NODE;
  new->h.node.n = n;
  new->h.node.lsub = NULL;
  new->h.node.rsub = NULL;
  return new;
}


int huff_weight(huff *h)
{
  if(h->tag == LEAF) { 
    return h->h.leaf.n;
  } else {
    return h->h.node.n;
  }
}


huff *huff_merge(huff *h1, huff *h2)
{
  huff *new = (huff*)malloc(sizeof(huff));

  if(h2 == NULL) {
    return h1;
  } else {
    new->tag = NODE;
    new->h.node.n = huff_weight(h1) + huff_weight(h2);
    new->h.node.lsub = h1;
    new->h.node.rsub = h2;
    return new;
  }
}

void huff_show_helper(int level, huff *h)
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
    huff_show_helper(level+1,h->h.node.lsub);
    huff_show_helper(level+1,h->h.node.rsub);
    return;
  }
  bad_tag("huff_show");
  exit(1);
}

 
void huff_show(huff *h)
{
  huff_show_helper(0,h);
}


hll *hll_cons(huff *h,hll *hs)
{
  hll *new = (hll*)malloc(sizeof(hll));
  new->huff = h;
  new->next = hs;
  return new;
}


int hll_length(hll *hs)
{
  int tot = 0;
  while(hs != NULL) {
    tot++;
    hs = hs->next;
  }
  return tot;
}

int lighter(huff *h, huff *i)
{
  return (huff_weight(h)<huff_weight(i));
}


hll *hll_insert(huff *h,hll*hs)
{
  hll *curr, *prev;
  if (hs==NULL || lighter(h,hs->huff))
    return hll_cons(h,hs);
  prev = hs;
  curr = hs->next;
  while (curr!=NULL) {
    if (lighter(h,curr->huff)) {
      prev->next = hll_cons(h,curr);
      return hs;
    }
    prev = prev->next;
    curr = curr->next;
  }
  prev->next = hll_cons(h,NULL);
  return hs;
}


hll *hll_sort(hll *hs)
{
  if(hs == NULL) {
    return NULL;
  } else {
    return hll_insert(hs->huff,hll_sort(hs->next));
  }
}


int count(char c,char *s)
{
  int tot = 0;
  int i;
  for(i = 0; i < strlen(s); i++)
    if(s[i] == c)
      tot++;
  return tot;
}



hll *word_to_hll(char *s)
{
 
  char *new = strdup(s);
  hll *first = NULL;
  
  int i;
  int j;
  int k;
  
  // create array of ASCII char set
  char ascii[128];
  for(i = 0; i < 128; i++) {
    ascii[i] = (char)i;
  }

  // creates an array of weights
  int a[128];
  for(k = 0; k < 128; k++) {
    a[k] = count(ascii[k],new);
  }

  // creates a huff linked list out of string
  for(j = 127; j >= 0; j--) {
    if(a[j] != 0) {
      first = hll_cons(huff_singleton(ascii[j],a[j]),first);
    }
  }
  return hll_sort(first);
}

hll *hll_rem(huff *h,hll* hs)
{
  hll *prev, *curr;
  if(h == NULL)
    return NULL;
  if(hs->huff == h) {
    hll *t = hs->next;
    free(hs);
    return t;
  }
  
  prev = hs;
  curr = hs->next;
  while(curr != NULL) {
    if(curr->huff == h) {
      prev->next = curr->next;
      free(curr);
      return hs;
    }
    curr = curr->next;
    prev = prev ->next;
  }
  return hs;
}


hll *hll_compress(hll *list)
{
  if(list == NULL)
    return NULL;
  huff *new;
  
  while(hll_length(list) != 1) {
    new = huff_merge(list->huff,list->next->huff);
    list = hll_rem(list->huff,list);
    list = hll_rem(list->huff,list);
    list = hll_cons(new,list);
    list = hll_sort(list);
  }
  return list;
}

huff *word_to_huff(char *s)
{
  hll *h_list; 
  h_list = word_to_hll(s);
  h_list = hll_compress(h_list);
  return h_list->huff;
}



  
  
