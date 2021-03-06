#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <ctype.h>
#include "huff.h"
#include "bits.h"


char** serialize_helper(huff *h, bits *history, char** codes)
{
  switch (h->tag) {
  case LEAF:
    codes[(int)h->h.leaf.c] = strdup(bits_print(history));
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
  char **codes = malloc(128*sizeof(char*));

  int i;
  for(i = 0; i < 128; i++) {
    codes[i] = NULL;
  }
  bits *bs = bits_new(1024);
  return serialize_helper(h,bs,codes);
}

void word_to_bitstring(char *s, char** codes)
{
  int i;
  for(i = 0; i < strlen(s); i++) {
    printf("%s",codes[(int)s[i]]);
  }
}

void codes_show(char** codes)
{
  int i;
  for(i = 0; i < 128; i++) {
    if(codes[i] != NULL) {
      putchar((char)i);
      printf("%s\n",codes[i]);
    }
  }
}

int txt_size()
{
  long size;

  size = 0;
  while(getchar() != EOF) {
    ++size;
  }
  return size;
}

char *txt_to_string()
{
  int c;
  int i = 0;
  int txt_len = 1 + txt_size();
  char *string = (char*)malloc(txt_len*sizeof(char));
  rewind(stdin);

  while((c = getchar()) != EOF) {
    string[i] = c;
    i++;
  }
  string[i] = '\0';
  return string;
}

void huff_encode(char *txt)
{
  huff *h = word_to_huff(txt);
  char **codes = huff_serialize(h);
  printf("HF\n");
  codes_show(codes);
  printf("*\n");
  word_to_bitstring(txt,codes);
}


huff *insert_code(huff *node,char *code,int i,int c)
{
  // if char is last char in code...
  if(i == (strlen(code)-1)) {
    // create leaf and attach to node
    if(code[i] == '0') {
      huff *leaf = huff_singleton((char)c,0);
      node->h.node.lsub = leaf;
    } else if(code[i] == '1') {
      huff *leaf = huff_singleton((char)c,0);
      node->h.node.rsub = leaf;
    }
    // if char is not last char in code...
  } else if(i != (strlen(code)-1)) {
    // if char is '0'...
    if(code[i] == '0') {
      // if lsub of node is not NULL recursively travel through lsub
      if(node->h.node.lsub != NULL) {
        i = i + 1;
        insert_code(node->h.node.lsub,code,i,c);
        // if lsub is NULL create new lsub, attach to node, and recur
      } else if(node->h.node.lsub == NULL) {
        i = i + 1;
        huff *new = huff_single_node(0);
        node->h.node.lsub = new;
        insert_code(node->h.node.lsub,code,i,c);
      }
      // if char is '1'...
    } else if(code[i] == '1') {
      // if rsub is not NULL, recur through rsub
      if(node->h.node.rsub != NULL) {
        i = i + 1;
        insert_code(node->h.node.rsub,code,i,c);
        // if rsub is NULL, create new rsub, attach, and recur
      } else if(node->h.node.rsub == NULL) {
        i = i + 1;
        huff *new = huff_single_node(0);
        node->h.node.rsub = new;
        insert_code(node->h.node.rsub,code,i,c);
      }
    }
  }
  return node;
}

huff *huff_deserialize(char **codes)
{
  huff *start = huff_single_node(0);
  int i;
  int k = 0;

  for(i = 0; i < 128; i++) {
    if(codes[i] != NULL) {
      start = insert_code(start,codes[i],k,i);
    }
  }
  return start;
}


int main(int argc,char *argv[])
{
  char *txt = txt_to_string();
  huff_encode(txt);
  return 0;
}
