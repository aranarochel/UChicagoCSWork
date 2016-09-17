#include <stdio.h>
#include <stdlib.h>

/* mandel : int int -> <void> */
/* given a side length and limit,
   produces a mandelbrot set */
void mandel(int len,int lim)
{
  int x,y,i;
  double z_re, z_im, z_rei;
  double c_re, c_im;
  printf("P3\n");
  printf("%d %d\n",len,len);
  printf("255\n");

  for(y = 0; y < len; y++) {
    for(x = 0; x < len; x++) {
      if(x == 0 && y == 0) {  /* initiates c constant */
	c_re = -2;
	c_im = 2;
      } else {
	c_re = -2 + ((4.0/len) * (double)x);
	c_im = 2 - ((4.0/len) * (double)y);
      }
      z_re = 0.0;
      z_im = 0.0;
      for (i = 0; i < lim; i++) {  /* iteration process */
	z_rei = z_re * z_re - z_im * z_im + c_re;
	z_im = 2 * z_re * z_im + c_im;
	z_re = z_rei;
      }
      if((z_re * z_re + z_im * z_im) < 4) {
	printf("0 0 0\n");
      } else {
	printf("255 255 255\n");
      }
    }
  }
}

int main(int argc,char *argv[])
{
  int c;
  int len = 480;

  while (--argc > 0 && (*++argv)[0] == '-')
    while (c = *++argv[0])
      switch (c) {
      case 's':
	len = atoi(argv[1]);
	break;
      }
  mandel(len,13);
  return 0;
}
