#ifndef _VCARD_H
#define _VCARD_H

struct vcard {
  char *cnet;
  char *email;
  char *fname;
  char *lname;
  char *tel;
};

typedef struct vcard vcard;

/* Construct a new vcard */
/* Copy each string with strdup during construction. */
vcard *vcard_new(char *cnet, char *email, char *fname, char *lname, char *tel);

/* Display vcard to stdout. */
/* The display of a vcard need not match any exact specification, */
/* but it must include all five of the vcard's component strings. */
void vcard_show(vcard *c);

void vcard_free(vcard *c);

#endif /* _VCARD_H */
