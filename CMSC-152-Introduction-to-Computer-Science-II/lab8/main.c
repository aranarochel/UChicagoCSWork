#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vcard.h"
#include "bst.h"

#define BUF_SZ 256

char *curr_file = NULL;

/* allocate a string on the heap and fill it with '\0' */
char *alloc_str(int sz)
{
  char *s = (char *)malloc(sz); /* sizeof(char) is 1, guaranteed */
  bzero(s,sz);                  /* zero out the character array; "man bzero" for info  */
  return s;
}

/* trim_newline replaces the newline at the end of the string (if there                                                    
 * is one) with '\0', and then calls strdup on the result,                                                                 
 * returning a pointer to the duplicated string.                                                                           
 * The string argument is left unaltered (but altered temporarily). */
char *trim_newline(char *s)
{
  char *t;
  int n = strlen(s);
  int must_change = (s[n-1]=='\n');
  if (must_change)
    s[n-1] = '\0';
  t = strdup(s);
  if (must_change)
    s[n-1] = '\n'; /* tidy up */
  return t;
}

char *extract_arg(char *cmd, int *parse_succeeded)
{
  int i;
  char cmdbuf[2];
  char argbuf[30];
  for (i=0; i<30; i++)
    argbuf[i] = '\0';
  sscanf(cmd,"%s %s",cmdbuf,argbuf);
  *parse_succeeded = strlen(argbuf)>0;
  return strdup(argbuf);
}

bst *addr_book_from_file(char *infile)
{
  char buf0[BUF_SZ], buf1[BUF_SZ], buf2[BUF_SZ], buf3[BUF_SZ], buf4[BUF_SZ];
  bst *address_book = NULL;
  FILE *f = fopen(infile,"r");
  if (f==NULL) {
    fprintf(stdout,"File not found: %s.\n", infile);
    return NULL;
  }
  while (!feof(f)) {
    vcard *c;
    fscanf(f,"%s\t%s\t%s\t%s\t%s\n",buf0,buf1,buf2,buf3,buf4);
    c = vcard_new(buf0,buf1,buf2,buf3,buf4);
    address_book = bst_insert(address_book, c);
  }
  fclose(f);
  return address_book;
}

void addr_book_stats(bst *abk)
{
  fprintf(stdout, "* entries in address book:      %d\n", 
	  bst_num_entries(abk));
  fprintf(stdout, "* height of binary search tree: %d\n", 
	  bst_height(abk));
  putchar('\n');
}

bst *admin_addr_book_from_file(char *infile)
{
  bst *address_book = addr_book_from_file(infile);
  if (address_book!=NULL) {
    printf("Read file %s.\n", infile);
    addr_book_stats(address_book);
    curr_file = strdup(infile);
  }
  return address_book;
}

int process_cmd(char *cmd, bst **address_book, int *quit)
{
  int parse_succeeded;
  char *cnet;
  char *infile;
  switch (cmd[0]) {
  case 'q':
    if (strcmp(cmd,"q")==0) {
      *quit = 1;
      return 1;
    }
    return 0;
  case 's':
    if (strcmp(cmd,"s")==0) {
      fprintf(stdout,"Current file: %s.\n",curr_file);
      addr_book_stats(*address_book);
      return 1;
    }
    return 0;
  case 'r':
    infile = extract_arg(cmd, &parse_succeeded);
    if (parse_succeeded) {
      bst *tmp = *address_book;
      *address_book = admin_addr_book_from_file(infile);
      if (*address_book==NULL) {
	/* restore address book */
	*address_book = tmp;
      } else {
	if (tmp!=NULL)
	  bst_free(tmp);
      }
    }
    free(infile);
    return parse_succeeded;
  case 'l':
    cnet = extract_arg(cmd, &parse_succeeded);
    if (parse_succeeded) {
      int n_comparisons = 0;
      vcard *c = bst_search(*address_book, cnet, &n_comparisons);
      if (c==NULL) 
	fprintf(stdout,"%s not found in current address book.\n", cnet);
      else
	vcard_show(c);
      fprintf(stdout,"(Performed %d comparisons in search.)\n", n_comparisons);
      putchar('\n');
    }
    free(cnet);
    return parse_succeeded;
  default:
    return 0;
  }
}

void prompt()
{
  char *p = 
    "Enter l [cnet] (lookup), r [file] (read file), s (stats), q (quit):\n> ";
  fprintf(stdout,"%s",p);
  fflush(stdout);
  return;
}

int main(int argc, char *argv[])
{
  int quit = 0;
  bst *address_book = NULL;
  fprintf(stdout,"Welcome to abk!\n\n");
  fflush(stdout);
  if (argc == 2) {
    curr_file = strdup(argv[1]);
    address_book = addr_book_from_file(curr_file);
    printf("Read file %s.\n", curr_file);
    putchar('\n');
    fflush(stdout);
  }
  char buf[BUF_SZ];
  /* interactive i/o loop */
  while (1) {
    char *cmd;
    prompt();
    cmd = fgets(buf, BUF_SZ, stdin);
    cmd = trim_newline(cmd);
    /* printf("cmd=%s\n", cmd); */
    putchar('\n');
    if (!process_cmd(cmd, &address_book, &quit))
      fprintf(stdout, "unknown command: \"%s\"\n", cmd);
    else
      if (quit)
	break;
  }
  fprintf(stdout,"Bye!\n");
  return 0;
}
