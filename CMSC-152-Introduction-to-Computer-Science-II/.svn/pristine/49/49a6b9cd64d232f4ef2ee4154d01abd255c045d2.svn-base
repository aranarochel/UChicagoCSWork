/* Jaime Arana-Rochel, aranarochel */
/* CS152, Winter 2013 */
/* Lab 8 */
#include <stdlib.h>
#include <stdio.h>

#include "vcard.h"

void vcard_todo(char *s)
{
  fprintf(stderr, "TODO[vcard]: %s\n", s);
  exit(1);
}

vcard *vcard_new(char *cnet, char *email, char *fname, char *lname, char *tel)
{
  vcard *new_card = (vcard*)malloc(sizeof(vcard));
  char *new_cnet = strdup(cnet);
  char *new_email = strdup(email);
  char *new_fname = strdup(fname);
  char *new_lname = strdup(lname);
  char *new_tel = strdup(tel);

  new_card->cnet = new_cnet;
  new_card->email = new_email;
  new_card->fname = new_fname;
  new_card->lname = new_lname;
  new_card->tel = new_tel;
  return new_card;
}

void vcard_free(vcard *c)
{
  free(c->cnet);
  free(c->email);
  free(c->fname);
  free(c->lname);
  free(c->tel);
  free(c);
  return;
}

void vcard_show(vcard *c)
{
  printf("cnet: %s\n",c->cnet);
  printf("email: %s\n",c->email);
  printf("name: %s %s\n",c->fname,c->lname);
  printf("telephone: %s\n",c->tel);
  return;
}
