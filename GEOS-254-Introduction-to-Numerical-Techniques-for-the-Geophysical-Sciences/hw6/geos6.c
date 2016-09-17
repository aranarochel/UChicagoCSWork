#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// first derivative
double f(double t, double x)
{
	return (x + x*t);
}

// second derivative
double ff(double t, double x)
{
	return f(t,x) + (t*f(t,x));
}


// prey model function
double rabbits(double R, double F, double alpha)
{
	return (2*R) - (2*alpha*R*F);
}

// predator model function
double foxes(double R, double F, double alpha)
{
	return (-1*F) + (0.5*alpha*R*F);
}

double euler(double initial, double end, double xa, int n)
{
	int k = 0;
	double h,t,a,b,x;

	b = end;
	a = initial;
	t = a;
	x = xa;
	h = (b-a)/n;

	printf("Iteration\t   t\t    x(t)\t\n");
	printf("%d\t\t%0.5f\t  %0.5f\t\n",k,t,x);

	for(k = 1; k <= n; k++)
	{
		x = x + (h*f(t,x));
		t = t+h;
		printf("%d\t\t%0.5f\t  %0.5f\t\n",k,t,x);
	}
	return x;
}

double euler_second(double initial, double end, double xa, int n)
{
	int k = 0;
	double h,t,a,b,x;

	b = end;
	a = initial;
	t = a;
	x = xa;
	h = (b-a)/n;

	printf("Iteration\t   t\t    x(t)\t\n");
	printf("%d\t\t%0.5f\t  %0.5f\t\n",k,t,x);

	for(k = 1; k <= n; k++)
	{
		x = x + (h*f(t,x)) + (((h*h) * ff(t,x))/2.0);
		t = t+h;
		printf("%d\t\t%0.5f\t  %0.5f\t\n",k,t,x);
	}
	return x;
}


// predator-prey integration
double predator_prey(double initial, double end, double R0, double F0, double alpha, int n)
{
	int k = 0;
	double h,t,a,b,R,F;

	b = end;			// time at which we stop
	a = initial;	// initial time start
	t = a;
	R = R0;	// initial rabbit population
	F = F0;	// initial fox population
	h = (b-a)/n;

	printf("Iteration\t   t\t    R(t)\t  F(t)\n");
	printf("%d\t\t%0.5f\t  %0.5f\t  %0.5f\n",k,t,R,F);

	for(k = 1; k <= n; k++)
	{
		R = R + (h*rabbits(R,F,alpha));
		F = F + (h*foxes(R,F,alpha));
		t = t+h;
		printf("%d\t\t%0.5f\t  %0.5f\t  %0.5f\n",k,t,R,F);
	}
	return R;


}


// MAIN FUNCTION
int main(int argc, char* argv[])
{
	
	printf("*** Problem 1 ***\n");
	printf("** EULER METHOD w/ 1 DERIVATIVE\n");
	double x_5 = euler(0.0,5.0,4.0,50);
	printf("\n");
	printf("** EULER METHOD w/ 2nd DERIVATIVE\n");
	double xx_5 = euler_second(0.0,5.0,4.0,50);

	/*
	printf("\n*** Problem 2 ***\n");
	double rab = predator_prey(0.0,100, 10, 200, 0.05, 1900);
	*/

	return 0;

}
