/* Jaime Arana-Rochel, aranarochel */
/* CS152, Win 13 */
/* Homework 4 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct string_linked_list {
  char *s;
  struct string_linked_list *next;
};

typedef struct string_linked_list SLL;

/* SLL_free : SLL -> <void> */
/* free the string list and all the strings it points to as well */
void SLL_free(SLL *list)
{
  while(list != NULL) {
    free(list->s);
    list = list->next;
  }
  free(list);
}

/* SLL_is_empty : SLL -> int */
/* return a quasi-boolean, indicating whether list is empty */
int SLL_is_empty(SLL *list)
{
  if(list == NULL) {
    return 1;
  } else {
    return 0;
  }
}

/* SLL_cons : string SLL -> SLL */
/* build a new list from s and list */
/* for the head of the list, copy the string to the heap (use strdup) */
SLL *SLL_cons(char *s, SLL *list)
{
  SLL *new = (SLL*)malloc(sizeof(SLL));
  char *dup = strdup(s);
  new->s = dup;
  new->next = list;
  return new;
  SLL_free(new);
}

// Like SLL_cons but modifies the given list instead of creating a new one
void insert(char*s, SLL *list)
{
  SLL *new = (SLL*)malloc(sizeof(SLL));
  char *dup = strdup(s);
  new->s = dup;
  new->next = list;
  list = new;
}

/* SLL_singleton : string -> SLL */
/* build a list of length 1, containing the given string */
/* copy the string to the heap (strdup) */
SLL *SLL_singleton(char *s)
{
  SLL *new = (SLL*)malloc(sizeof(SLL));
  char *dup = strdup(s);
  new->s = dup;
  new->next = NULL;
  return new;
  SLL_free(new);
}

/* SLL-length : SLL -> int */
/* compute the length of the given list */
int SLL_length(SLL *list)
{
  int total = 0;
  while (list != NULL) {
    total++;
    list = list->next;
  }
  return total;
}

/* SLL_shallow_copy : SLL -> SLL */
/* build a shallow copy of the given list */
/* i.e., do not duplicate the strings, just point to them */
SLL *SLL_shallow_copy(SLL *list)
{
  SLL *scopy = (SLL*)malloc(sizeof(SLL));
  scopy->s = list->s;
  scopy->next = list->next;
  return scopy;
}


/* SLL_deep_copy : SLL -> SLL */
/* build a deep copy of the given list */
SLL *SLL_deep_copy(SLL *list)
{
  SLL *dcopy = (SLL*)malloc(sizeof(SLL));
  while(list != NULL) {
    char *dup = strdup(list->s);
    dcopy->s = dup;
    dcopy = dcopy->next;
    list = list->next;
  }
  return dcopy;
}

/* SLL_tos : string string char SLL -> string */
/* build a string representation of the given list  */
/* the arguments are the left and right delimiters, */
/* a character to separate the strings in the list, */
/* and the list itself */
char *SLL_tos(char *ldelim, char *rdelim, char sep, SLL *list)
{
  char a[] = {sep,'\0'};
  SLL *copy = (SLL*)malloc(sizeof(SLL));
  copy->s = list->s;
  copy->next = list->next;

  int total_chars = 0;
  while(copy != NULL) {
    total_chars = strlen(copy->s) + total_chars;
    copy = copy->next;
  }
  char *str = (char*)malloc(total_chars*sizeof(char)+1);
  str = strcat(str,ldelim);
  while(list != NULL) {
    str = strcat(str,list->s);
    if(list->next != NULL)
      str = strcat(str,a);
    list = list->next;
  }
  str = strcat(str,rdelim);
  SLL_free(copy);
  return str;
  free(str);
}

/* SLL_same : SLL SLL -> int */
/* test whether the two lists are logically identical */
/* (they may or may not be physically identical) */
int SLL_same(SLL *list1, SLL *list2)
{
  int total = 1;
  int len1 = SLL_length(list1);
  int len2 = SLL_length(list2);

  if(len1 != len2) {
    return 0;
  } else {
    while(list1 != NULL && list2 != NULL) {
      total = strcmp(list1->s,list2->s) + total;
      list1 = list1->next;
      list2 = list2->next;
    }
    if(total == 1) {
      return total;
    } else {
      return 0;
    }
  }
}


/* Evidence Tests for String Toolkit */
    
void evidence_SLL_is_empty()
{
  printf("*** testing SLL_is_empty\n");
  printf("- expecting 1: result %d\n",SLL_is_empty(NULL));
  printf("- expecting 0: result %d\n",SLL_is_empty(SLL_cons("cat",NULL)));
}

void evidence_SLL_cons()
{
  printf("*** testing SLL_cons\n");
  printf("- expecting (list cat): result %s\n",
	 SLL_tos("(list ",")",' ',SLL_cons("cat",NULL)));
  printf("- expecting (list cat worm): result %s\n",
	 SLL_tos("(list ",")",' ',SLL_cons("cat",SLL_cons("worm",NULL))));
  printf("- expecting (list 1 2 3):result %s\n",
	 SLL_tos("(list ",")",' ',SLL_cons("1",SLL_cons("2",SLL_cons("3",NULL)))));
}

void evidence_SLL_singleton()
{
  printf("*** testing SLL_singleton\n");
  printf("- expecting (list cat): result %s\n",
	 SLL_tos("(list ",")",' ',SLL_singleton("cat")));
  printf("- expecting (list 1): result %s\n",
	 SLL_tos("(list ",")",' ',SLL_singleton("1")));
}

void evidence_SLL_length()
{
  printf("*** testing SLL_length\n");
  printf("- expecting 0: result %d\n",
	 SLL_length(NULL));
  printf("- expecting 1: result %d\n",
	 SLL_length(SLL_cons("cat",NULL)));
  printf("- expecting 3: result %d\n",
	 SLL_length(SLL_cons("cat",SLL_cons("cat",SLL_cons("cat",NULL)))));
}

void evidence_SLL_shallow_copy()
{
  SLL *list1 = SLL_cons("cat",NULL);
  SLL *list2 = SLL_shallow_copy(list1);
  SLL *test1 = SLL_cons("cat",SLL_cons("dog",NULL));
  SLL *test2 = SLL_shallow_copy(test1);
  printf("*** testing SLL_shallow_copy\n");
  printf("- expecting 1: result %d\n",SLL_same(list1,list2));
  printf("- expecting 1; result %d\n",SLL_same(test1,test2));
}
	 

void evidence_SLL_tos()
{
  printf("*** testing SLL_tos\n");
  printf("- expecting (test): result %s\n",
	 SLL_tos("(",")",' ',SLL_cons("test",NULL)));
  printf("- expecting '(test,cat): result %s\n",
         SLL_tos("'(",")",',',SLL_cons("test",SLL_cons("cat",NULL))));
  printf("- expecting *yeah|boyee*: result %s\n",
         SLL_tos("*","*",'|',SLL_cons("yeah",SLL_cons("boyee",NULL))));
  printf("- expecting |1^mo^test}: result %s\n",
         SLL_tos("|","}",'^',SLL_cons("1",SLL_cons("mo",SLL_cons("test",NULL)))));
}

void evidence_SLL_same()
{
  printf("*** testing SLL_same\n");
  printf("- expecting 1: result %d\n",
	 SLL_same(SLL_cons("cat",NULL),SLL_cons("cat",NULL)));
  printf("- expecting 1: result %d\n",
	 SLL_same(SLL_cons("cat",SLL_cons("dog",NULL)),
		  SLL_cons("cat",SLL_cons("dog",NULL))));
  printf("- expecting 1: result %d\n",
	 SLL_same(NULL,NULL));
  printf("- expecting 0: result %d\n",
	 SLL_same(SLL_cons("cat",NULL),SLL_cons("dog",NULL)));
  printf("- expecting 0: result %d\n",
	 SLL_same(SLL_cons("cat",SLL_cons("cat",NULL)),
		  SLL_cons("cat",SLL_cons("dog",NULL))));
  printf("- expecting 0: result %d\n",
	 SLL_same(SLL_cons("cat",NULL),NULL));
}


int main(int argc,char *argv[]) 
{
  /*evidence_SLL_is_empty();
  evidence_SLL_cons();
  evidence_SLL_singleton();
  evidence_SLL_length();
  evidence_SLL_shallow_copy();
  evidence_SLL_tos();
  evidence_SLL_same();*/

  SLL* list = SLL_cons("cat", NULL);
  insert("many", list);
  char *list_items = SLL_tos("(",")",' ',list);
  printf("%s", list_items);
  return 0;
}

  

