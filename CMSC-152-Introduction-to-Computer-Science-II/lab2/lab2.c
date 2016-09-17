/* Jaime Arana-Rochel, aranarochel */
/* CS 152, Winter 2013 */
/* Lab 2 */
#include<stdio.h>

/* expt : int int -> int */
/* computes some integer a to the nth power, for natural n */
long int expt(int a, unsigned int n)
{
  int c;

  if (n == 0) {
    return 1;
  } else if (n > 0) {
    return a * expt(a, n - 1);
  }
}


/* ss : int int -> int */
/* exponentiation by successive squaring */
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

/* ssm : int int int -> int */
/* exponentiation modulo m */
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


/* main : int char*-array -> int */
/* tests the exponential functions */
int main(int argc, char *argv[])
{
  printf("%d\n %d\n %d\n",ssm(4,15,9),ssm(13,8435,67),ssm(3,597854,486));
  return 0;
}




