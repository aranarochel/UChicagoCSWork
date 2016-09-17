/* Jaime Arana-Rochel, aranarochel */
/* CS152, Winter 2013 */
/* Lab 7 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htbl.h"

unsigned long hash(char *s)
{
  int i;
  unsigned long int result = 17;
  for(i = 0; s[i] != '\0'; i++) {
    result = 37*result+(int)s[i];
  }
  return result;
}

htbl *htbl_init(int sz)
{
  int i;
  htbl *hashtable = (htbl*)malloc(sizeof(htbl));
  sll **table = (sll**)malloc(sizeof(sll*)*sz);
  for(i = 0; i < sz; i++) {
    table[i] = NULL;
  }
  hashtable->n_buckets = sz;
  hashtable->buckets = table;
  return hashtable;
}

unsigned int htbl_n_entries(htbl *t)
{
  int i;
  unsigned int tot = 0;
  if(t == NULL)
    return tot;

  for(i = 0; i < t->n_buckets; i++) {
    tot = sll_length(t->buckets[i]) + tot;
  }
  return tot;
}

float htbl_load_factor(htbl *t)
{
  float result;
  result = (float)htbl_n_entries(t)/t->n_buckets;
  return result;
}

int htbl_add(char *s, htbl *t)
{
  unsigned long hashcode = hash(s);
  int index = hashcode % t->n_buckets;

  sll *bucket = (sll*)malloc(sizeof(sll));
  bucket->s = (char*)malloc(strlen(s)*sizeof(char)+1);
  
  strcpy(bucket->s,s);
  bucket->next = t->buckets[index];
  t->buckets[index] = bucket;

  return sll_length(t->buckets[index]);
}

int htbl_member(char *s, htbl *t)
{
  int x = 0;
  unsigned long hashcode = hash(s);
  int index = hashcode % t->n_buckets;
  sll *bucket = t->buckets[index];

  while(bucket != NULL) {
    if(strcmp(s,bucket->s) == 0) {
      x = 1;
      break;
    } else {
      x = 0;
    }
    bucket = bucket->next;
  }
  return x;
}

void htbl_show(htbl *t)
{
  int i;
  for(i = 0; i < t->n_buckets; i++) {
    printf("bucket[%d]:",i);
    sll_show(t->buckets[i]);
  }
}

void htbl_free(htbl *t)
{
  int i;
  for(i = 0; i < t->n_buckets; i++) {
    sll_free(t->buckets[i]);
  }
  free(t->buckets);
  free(t);
}

  
  


