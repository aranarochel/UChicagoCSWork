#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "bits.h"

bits *bits_new(unsigned int sz)
{
  bits *new = (bits *)malloc(sizeof(bits));
  new->capacity = sz;
  new->next = 0;
  new->bits = (char *)malloc(sz);
  bzero(new->bits,sz);
  return new;
}

void bits_free(bits *a)
{
  if (a!=NULL) {
    free(a->bits);
    free(a);
  }
}

void bits_show(bits *a)
{
  int i;
  for (i=0; i<a->next; i++)
    putchar(a->bits[i]);
}

char *bits_print(bits *a)
{
  char *new = malloc(sizeof(char));
  int i;
  for (i=0; i<a->next; i++) {
    new[i] = a->bits[i];
  }
  return new;
}
    

void bits_putlast(char b, bits *a)
{
  if (a->next==a->capacity) {
    /* this is where one would double and copy...I'm going to omit that, */
    /* but will at least report a clear error and exit. */
    fprintf(stderr,"bits_putlast: array full.\n");
    exit(1);
  }
  if (b!='0' && b!='1') {
    fprintf(stderr,"bits_putlast: please put only '0' or '1'\n");
    exit(1);
  }
  a->bits[a->next] = b;
  (a->next)++;
  return;
}

void bits_remove_last(bits *a)
{
  if (a->next==0) {
    fprintf(stderr,"bits_remove_last: nothing to remove\n");
    exit(1);
  }
  (a->next)--;
  return;
}
