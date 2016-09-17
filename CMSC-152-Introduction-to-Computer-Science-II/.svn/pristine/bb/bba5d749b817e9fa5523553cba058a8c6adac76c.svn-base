/* Jaime Arana-Rochel, aranarochel */
/* CS152, Winter 2013 */
/* Lab 6  */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

/* color_mod : double -> int */
/* color modifier when -c is called */
int color_mod(double var)
{
  if(var < 0.0) 
    var = -var;
  if(var < 1.0)
    var = var + 1.0;
  return (int)floor(255.0/var);
}

/* julia_P3 : int int double double double double double -> <void> */
/* given an image size, iteration limit,complex number c, */
/* upper left coordinates and the logical length of the view plane */
/* produce a plain ppm (P3) julia set fractal */
void julia_P3(int len, int lim, double c_re, double c_im,
	      double x_coor, double y_coor, double loglen,int color)
{
  int x, y, i;
  double z_re, z_im, z_rei, z_imi;

  printf("P3\n");
  printf("%d %d\n",len,len);
  printf("255\n");

  for(y = 0; y < len; y++) {
    for(x = 0; x < len; x++) {
      if(x == 0 && y == 0) {
        z_re = x_coor;
        z_im = y_coor;
      } else {
        z_re = x_coor + ((loglen/len) * (double)x);
        z_im = y_coor - ((loglen/len) * (double)y);
      }
      for (i = 0; i < lim; i++) {
        z_rei = z_re;
        z_imi = z_im;
        z_re = z_rei * z_rei - z_imi * z_imi + c_re;
        z_im = 2 * z_rei * z_imi + c_im;
      }
      if((z_rei * z_rei + z_imi * z_imi) < 4) {
	if(color == 1) { 
	  printf("0 0 0\n");
	} else {
	  printf("%d %d %d\n",
		 color_mod(z_re),color_mod(z_im),color_mod(c_re));
	}
      } else {
	if(color == 1) {
	  printf("255 255 255\n");
	} else {
	  printf("50 100 180\n");
	}
      }
    }
  }
}	

/* julia_P6 : int int double double double double double -> <void> */
/* given an image size, iteration limit,complex number c, */
/* upper left coordinates and the logical length of the view plane */
/* produce a ppm image (P6) julia set fractal */
void julia_P6(int len, int lim, double c_re, double c_im,
	      double x_coor, double y_coor, double loglen,int color)
{
  int x, y, i;
  double z_re, z_im, z_rei, z_imi;

  printf("P6\n");
  printf("%d %d\n",len,len);
  printf("255\n");

  for(y = 0; y < len; y++) {
    for(x = 0; x < len; x++) {
      if(x == 0 && y == 0) {
        z_re = x_coor;
        z_im = y_coor;
      } else {
        z_re = x_coor + ((loglen/len) * (double)x);
        z_im = y_coor - ((loglen/len) * (double)y);
      }
      for (i = 0; i < lim; i++) {
        z_rei = z_re;
        z_imi = z_im;
        z_re = z_rei * z_rei - z_imi * z_imi + c_re;
        z_im = 2 * z_rei * z_imi + c_im;
      }
      if((z_rei * z_rei + z_imi * z_imi) < 4) {
	if(color == 1) {
	  putchar(0);
	  putchar(0);
	  putchar(0);
	} else {
	  putchar(color_mod(z_re));
	  putchar(color_mod(z_im));
	  putchar(color_mod(c_re));
	}
      } else {
	if(color == 1) {
	  putchar(255);
	  putchar(255);
	  putchar(255);
	} else {
	  putchar(30);
	  putchar(100);
	  putchar(190);
	}
      }
    }
  }
}


/* mandel_P3 : int int -> <void> */
/* given a side length,limit,starting coordinates */
/* and the logical length of the view plane */ 
/* produces a plain ppm (P3) of a mandelbrot set */
void mandel_P3(int len,int lim,double x_coor,double y_coor,double loglen,
	       int color)
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
        c_re = x_coor;
        c_im = y_coor;
      } else {
        c_re = x_coor + ((loglen/len) * (double)x);
        c_im = y_coor - ((loglen/len) * (double)y);
      }
      z_re = 0.0;
      z_im = 0.0;
      for (i = 0; i < lim; i++) {  /* iteration process */
        z_rei = z_re * z_re - z_im * z_im + c_re;
        z_im = 2 * z_re * z_im + c_im;
        z_re = z_rei;
      }
      if((z_re * z_re + z_im * z_im) < 4) {
	if(color == 1) {
	  printf("0 0 0\n");
	} else {
	  printf("%d %d %d\n",color_mod(c_re),color_mod(c_im),color_mod(z_re));
	}
      } else {
	if(color == 1) {
	  printf("255 255 255\n");
	} else {
	  printf("180 70 100\n");
	}
      }
    }
  }
}

/* mandel_P6 : int int -> <void> */
/* given a side length,limit,starting coordinates */
/* and the length of the logical view plane */
/* produces a ppm image (P6) of a mandelbrot set */
void mandel_P6(int len,int lim,double x_coor,double y_coor,double loglen,
	       int color)
{
  int x,y,i;
  double z_re, z_im, z_rei;
  double c_re, c_im;
  printf("P6\n");
  printf("%d %d\n",len,len);
  printf("255\n");

  for(y = 0; y < len; y++) {
    for(x = 0; x < len; x++) {
      if(x == 0 && y == 0) {  /* initiates c constant */
        c_re = x_coor;
        c_im = y_coor;
      } else {
        c_re = x_coor + ((loglen/len) * (double)x);
        c_im = y_coor - ((loglen/len) * (double)y);
      }
      z_re = 0.0;
      z_im = 0.0;
      for (i = 0; i < lim; i++) {  /* iteration process */
        z_rei = z_re * z_re - z_im * z_im + c_re;
        z_im = 2 * z_re * z_im + c_im;
        z_re = z_rei;
      }
      if((z_re * z_re + z_im * z_im) < 4) {
	if(color == 1) {
	  putchar(0);
	  putchar(0);
	  putchar(0);
	} else {
	  putchar(color_mod(c_re));
	  putchar(color_mod(c_im));
	  putchar(color_mod(z_re));
	}
      } else {
	if(color == 1) {
	  putchar(255);
	  putchar(255);
	  putchar(255);
	} else {
	  putchar(180);
	  putchar(70);
	  putchar(110);
	}
      }
    }
  }
}

int main(int argc,char *argv[])
{
  FILE *f;
  int write = 1; //pseudo-bool for testing if -o
  int compact = 1; // pseudo-bool for testing if -p is called
  int jul = 1; // pseudo-bool for testing if -j is called
  int man = 1; // same thing here but for -m
  int color = 1; // pseudo-bool for testing if -c is called
  int c;
  int lim = 10;
  double n = 4.0;
  int side = 480;
  double x = -2.0;
  double y = 2.0;
  double re = 0.0;
  double im = 1.0;

  while((c = getopt(argc,argv,"l:n:s:x:y:mjr:i:o:pc")) != -1)
    switch(c) 
      {
      case 'l':
	lim = atoi(optarg);
	break;
      case 's':
	side = atoi(optarg);
	break;
      case 'n':
	n = atof(optarg);
	break;
      case 'x':
	x = atof(optarg);
	break;
      case 'y':
	y = atof(optarg);
	break;
      case 'r':
	re = atof(optarg);
	break;
      case 'i':
	im = atof(optarg);
	break;
      case 'j':
	jul = 2;
	break;
      case 'm':
	man = 2;
	break;
      case 'p':
	compact = 2;
	break;
      case 'c':
	color = 2;
	break;
      case 'o':
	f = fopen(optarg,"w");
	write = 2;
	break;
      default:
	abort();
      }
 
  if(man == jul) {
    if(compact == 1)
      mandel_P3(side,lim,x,y,n,color);
    mandel_P6(side,lim,x,y,n,color);
  } else if(man > jul) {
    if(compact == 1)
      mandel_P3(side,lim,x,y,n,color);
    mandel_P6(side,lim,x,y,n,color);
  } else {
    if(compact == 1) 
      julia_P3(side,lim,re,im,x,y,n,color);
    julia_P6(side,lim,re,im,x,y,n,color);
  }
  return 0;
}

  

  

