#include <stdlib.h>
#include <stdio.h>
#include "expt.h"

int main(int argc, char *argv[])
{
  int a = atoi(argv[1]);
  unsigned int n = atoi(argv[2]);
  printf("%ld\n",ss(a,n));
  return 0;
}
