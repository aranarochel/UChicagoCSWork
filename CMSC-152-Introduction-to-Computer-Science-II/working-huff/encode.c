#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "encode.h"

#define ALPHABET_SIZE 26

int *letter_freq(char *m)
{
  int mlen = strlen(m);
  int i;
  int *f = malloc(ALPHABET_SIZE*sizeof(int));
  bzero(f,ALPHABET_SIZE*sizeof(int));
  for (i=0; i<mlen; i++) {
    char c = m[i];
    if (c>='A' && c<='Z')
      f[c-'A']++;
    else if (c>='a' && c<='z')
      f[c-'a']++;
  }
  return f;
}

huffll *leaves(int *freq)
{
  int i;
  huffll *list = NULL;
  for (i=ALPHABET_SIZE-1; i>=0; i--)
    if (freq[i]>0)
      list = huffll_cons(huff_singleton('A'+i,freq[i]),list);
  return list;
}

huff *huff_from_msg(char *m)
{
  return huffll_coalesce(huffll_isort(leaves(letter_freq(m))));
}
