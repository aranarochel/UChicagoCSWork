#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// exponential function used in this assignment
double f(double x)
{
	return exp(x);
}

// second function used in this assignment
double g(double x)
{
	return (4*pow((1-x*x),0.5));
}

// implementation of recursive trapezoidal rule.
// returns an array with the rule value at each n value
double* trapezoid(double a, double b, int n) {

	int i,k;
  double h, sum;

  double* T = (double*)malloc(n*sizeof(double));
  h = b - a;
  T[0] = 0.5 * h * (f(a) + f(b));   
  printf("Beginning value:  T[0] = %f\n", T[0]);

  for (i = 1; i <= n; i++)
  { 
     h *= 0.5;
     sum = 0;
     for (k = 1; k <= pow(2,i)-1; k+=2)
     {
       sum += f(a + k * h);
     }

     T[i] = 0.5 * T[i-1] + sum * h;  
   }


   return T;

}

// implementation of simpson 1/3 rule
double simpson(double a, double b, int n) {
                      
  int i = 0;
  //double h = (b - a) / n;     // step size
  double h = (b-a)/ n;

  // 1/3 terms
  double sum = 1.0 / 3.0 * (f(a) + f(b));

  // 4/3 terms
  for (i = 1; i < n; i += 2) {
     double x = a + h * i;
     sum += 4.0 / 3.0 * f(x);
  }

  // 2/3 terms
  for (i = 2; i < n; i += 2) {
     double x = a + h * i;
     sum += 2.0 / 3.0 * f(x);
  }

  return sum * h;
}


// implementation of simpson 1/3 rule but with the funciton "g"
double simpson_g(double a, double b, int n) {
                      
  int i = 0;
  double h = (b - a) / n;     // step size
 

  // 1/3 terms
  double sum = 1.0 / 3.0 * (g(a) + g(b));

  // 4/3 terms
  for (i = 1; i < n; i += 2) {
     double x = a + h * i;
     sum += 4.0 / 3.0 * g(x);
  }

  // 2/3 terms
  for (i = 2; i < n; i += 2) {
     double x = a + h * i;
     sum += 2.0 / 3.0 * g(x);
  }

  return sum * h;
}


// MAIN FUNCTION
int main(int argc, char* argv[])
{
	printf("*** PROBLEM 1 ***\n");
	int j = 0;
	int n = 13;
	printf("Value of n = %d\n",n );
	printf("Integration stepsize = %f\n\n",(5-0)/pow(2,n));

	double* T = trapezoid(0,5,n);
	for(j = 0; j < n; j++)
   {
   	printf("PRINTING ARRAY: T[%d] = %f\n",j,T[j] );
   }
   printf("\n");

  printf("**Calculating if result is within tolerance (0.0001)**\n");
  printf("T[n] - T[n-1] = %f-%f = %f < 0.0001\n",T[n-1],T[n-2],fabs(T[n-1]-T[n-2]) );
  printf("\n");
  printf("** Trapezoidal Rule Approximation **\n");
  printf("Integral Estimate = %f\n\n",T[n-1] );

  printf("*** PROBLEM 2 ***\n");
  int s = pow(2,n);
  printf("** Simpson Rule Approximation **\nIntegral Estimate = %f\n",simpson(0,5,s) );

  printf("*** PROBLEM 3 ***\n");
  printf("g = %f\n",simpson_g(0,1,s) );

	return 0;
}