/* Jaime Arana-Rochel, aranarochel */
/* CS 152, Winter 2013 */
/* Lab 2 */
#include <stdio.h>
#include "expt.h"

long int expt(int a, unsigned int n)
{
  if (n == 0) {
    return 1;
  } else if (n > 0) {
    return a * expt(a, n - 1);
  }
}

long int ss(int a, unsigned int n)
{
  int i = n / 2;

  if (n == 0) {
    return 1;
  } else if (n == 1) {
    return a;
  } else if (n % 2 == 0) {      /* condition for even case */
    return ss(a * a, i);
  } else {
    return a * ss(a * a, (n - 1) / 2);
  }
}

int ssm(int a, unsigned int n, unsigned int m)
{
  int i = n / 2;

  if (n == 0) {
    return 1;
  } else if (n == 1) {
    return a;
  } else if (n % 2 == 0) {
    return ssm(a * a, i, m) % m;
  } else {
    return a * ssm(a * a, (n - 1) / 2, m) % m;
  }
}

