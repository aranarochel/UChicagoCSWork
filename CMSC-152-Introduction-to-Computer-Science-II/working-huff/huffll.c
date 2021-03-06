#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "huff.h"
#include "huffll.h"

huffll *huffll_cons(huff *h, huffll *hs)
{
  huffll *new = malloc(sizeof(huffll));
  new->h = h;
  new->next = hs;
  return new;
}

/* destructive append (no allocations) */
huffll *huffll_append(huffll *hs0, huffll *hs1)
{
  huffll *p = hs0;
  if (hs0==NULL)
    return hs1;
  while (p->next)
    p=p->next;
  p->next = hs1;
  return hs0;
}

void huffll_show(huffll *t)
{
  int i=0;
  printf("BEGIN HUFFLL\n");
  while (t) {
    printf("item %i: ",i++);
    huff_show(t->h);
    t=t->next;
  }
  printf("END HUFFLL\n");
}

int lighter(huff *h, huff *i)
{
  return (huff_weight(h)<huff_weight(i));
}

huffll *huffll_insert(huff *h, huffll *hs)
{
  huffll *curr, *prev;
  if (hs==NULL || lighter(h,hs->h))
    return huffll_cons(h,hs);
  prev = hs;
  curr = hs->next;
  while (curr!=NULL) {
    if (lighter(h,curr->h)) {
      prev->next = huffll_cons(h,curr);
      return hs;
    }
    prev = prev->next;
    curr = curr->next;
  }
  prev->next = huffll_cons(h,NULL);
  return hs;
}

huffll *huffll_isort(huffll *hs)
{
  if (hs==NULL || hs->next==NULL)
    return hs;
  return huffll_insert(hs->h,huffll_isort(hs->next));
}

huff *huffll_coalesce(huffll *hs)
{
  if (hs==NULL) {
    fprintf(stderr,"error in huffll_merge: given NULL\n");
    exit(1);
  }
  if (hs->next==NULL)
    return hs->h;
  return 
    huffll_coalesce(huffll_insert(huff_merge(hs->h,hs->next->h),hs->next->next));
}
