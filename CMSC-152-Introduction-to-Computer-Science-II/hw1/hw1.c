/* Jaime Arana-Rochel, aranarochel */
/* CS152, Winter 2013 */
/* Homework 1 */
#include <stdio.h>
#include <math.h>

/* gcd : int int -> int */
/* greatest common divisor */
int gcd(unsigned int a, unsigned int b)
{
  if (b == 0) {
    return a;
  } else if (a >= b) {
    return gcd(b, a % b);
  } else if (b >= a) {
    return gcd(a, b % a);
  }
}

/* area_pipe : int int int -> int */
/* computes the area of a pipe */
double area_pipe(double i, double l, double t)
{
  double r = i + t;
  double pi = M_PI;
  double donut = 2 * ((pi * r * r) - (pi * i * i));
  double outer = l * 2 * pi * r;
  double inner = l * 2 * pi * i;

  return donut + outer + inner;
}


/* adj : int int -> int */
/* calculates adjustment given month and year */
int adj(int m, int y)
{
  if (m == 1) {
    if ((y % 400 == 0 && y % 100 != 0) || y % 400 == 0) {
      return 0;
    } else {
      return 1;
    }
  } else if (m == 2) {
    if ((y % 400 == 0 && y % 100 != 0) || y % 400 == 0) {
      return 3;
    } else {
      return 4;
    }
  } else if (m == 3) {
    return 4;
  } else if (m == 4) {
    return 0;
  } else if (m == 5) {
    return 2;
  } else if (m == 6) {
    return 5;
  } else if (m == 7) {
    return 0;
  } else if (m == 8) {
    return 3;
  } else if (m == 9) {
    return 6;
  } else if (m == 10) {
    return 1;
  } else if (m == 11) {
    return 4;
  } else if (m == 12) {
    return 6;
  }
}


/* dow : int int int -> int */
/* computes day of week */
int dow(int m, int d, int y)
{
  int n = (y - 1900) + adj(m,y) + d + (int)floor(y / 4);
  int w = fmod(n,7);
  return w;
}





int main(int argc, char *argv[])
{
  printf("%d %d %d %lf\n",gcd(2,2),gcd(8,2),gcd(2,8),area_pipe(2.0, 3.0, 4.0));
  printf("%d",dow(9,9,1999));
}
