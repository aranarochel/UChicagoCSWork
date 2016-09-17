/* Jaime Arana-Rochel, aranarochel */
/* CS152, WIN 2013' */
/* Lab 7 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sll.h"

sll *sll_cons(char *s, sll *ss)
{
  sll *new = (sll*)malloc(sizeof(sll));
  char *dup = strdup(s);
  new->s = dup;
  new->next = ss;
  return new;
}

int sll_length(sll *ss)
{
  int total = 0;
  if(ss == NULL)
    return total;

  while (ss != NULL) {
    total++;
    ss = ss->next;
  }
  return total;
}

int sll_member(char *s, sll *ss)
{
  int x = 0;
  for(; ss != NULL; ss = ss->next) {
    if(strcmp(s,ss->s) == 0) {
      x = 1;
      break;
    } else {
      x = 0;
    }
  }
  return x;
}
  
void sll_show(sll *ss)
{
  while(ss) {
    printf("%s",ss->s);
    if(ss->next != NULL)
      putchar(',');
    ss = ss->next;
  }
  putchar('\n');
}

void sll_free(sll *ss)
{
  while(ss) {
    free(ss->s);
    ss = ss->next;
  }
  free(ss);
}









