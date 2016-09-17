#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <ctype.h>
#include "huff.h"
#include "huffll.h"
#include "encode.h"
#include "bits.h"

/* helper for show (see below) */
void show_helper(huff *h, bits *history)
{
  switch (h->tag) {
  case LEAF:
    printf("%c ", h->h.leaf.c);
    bits_show(history);
    putchar('\n');
    return;
  case NODE:
    /* traverse left subtree */
    bits_putlast('0',history);
    show_helper(h->h.node.lsub,history);
    bits_remove_last(history);
    /* traverse right subtree */
    bits_putlast('1',history);
    show_helper(h->h.node.rsub,history);
    bits_remove_last(history);
    return;
  default:
    fprintf(stderr,"main.show_helper: bad tag\n");
    exit(1);
  }
}

/* A function to display the Huffman codes in the tree. */
/* (close cousin to serialization) */
void show(huff *h)
{
  bits *bs = bits_new(1024); 
  /* This allocation allows the program to handle a Huffman tree with 1024 levels. */
  /* That should be *plenty*. */
  show_helper(h,bs);
}

char** serialize_helper(huff *h, bits *history, char** codes)
{
  switch (h->tag) {
  case LEAF:
    codes[(int)h->h.leaf.c] = strdup(history->bits);
    break;
  case NODE:
    /* traverse left subtree */
    bits_putlast('0',history);
    serialize_helper(h->h.node.lsub,history,codes);
    bits_remove_last(history);
    /* traverse right subtree */
    bits_putlast('1',history);
    serialize_helper(h->h.node.rsub,history,codes);
    bits_remove_last(history);
    break;
  default:
    fprintf(stderr,"main.show_helper: bad tag\n");
    exit(1);
  }
  return codes;
}

char** huff_serialize(huff *h)
{
  char **codes = malloc(128*sizeof(char**));
  int i;
  for(i = 0; i < 128; i++) {
    codes[i] = NULL;
  }
  bits *bs = bits_new(1024);
  return serialize_helper(h,bs,codes);
}

int all_one_char(char *s)
{
  int i;
  int slen = strlen(s);
  char c = tolower(s[0]);
  for (i=1; i<slen; i++)
    if (tolower(s[i])!=c)
      return 0;
  return 1;
}

int main(int argc, char *argv[])
{
  char *txt;
  if (argc==1)  {
    fprintf(stderr,"%s: No text to encode; halting.\n",*argv);
    return 0;
  }
  if (argc!=2) {
    fprintf(stderr,
	    "%s: Please type exactly one whitespace-free string for encoding.\n",
	    *argv);
    exit(1);
  }
  txt = argv[1];
  if (all_one_char(txt)) {
    /* This is a bizarre special case that OCD compels me to handle. */
    fprintf(stderr,
	    "%s: Asked to encode message with only one character. That's weird. Exiting.\n",
	    *argv);
    exit(1);
  }
  huff *h = huff_from_msg(txt);
  show(h);
  char** ser = huff_serialize(h);
  printf("%s %s %s\n",ser[65],ser[66],ser[67]);
  return 0;
}
