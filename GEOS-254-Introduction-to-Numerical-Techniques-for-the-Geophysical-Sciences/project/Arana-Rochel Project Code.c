/*
 * Author: Jaime Arana-Rochel
 * Class: GEOS 254 Winter 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define M_PI 3.14159265358979323846


/* function used to illustrate the root finding algorithms
 * f(x) = x^3 - 8x^2 + x +42
 * Roots: x = -2, x = 7, x = 3
 */
double f1(double x)
{
	double y = pow(x,3) - (8*x*x) + x + 42;
	return y;
}

/* function to illustrate secant method non-convergence 
 * Root: x = 0.44743154
 */
double f2(double x)
{
	double y = tan(M_PI*x) -6;
	return y;
}

/* implementation of secant method */
int secant_f(double x1, double x2, int nmax, double eps)
{
	int n = 0;
	double d;

	printf("Iteration no. %d, Initial guesses x1=%f, x2=%f, f(x1)=%f, f(x2)=%f\n", n,x1,x2,f2(x1),f2(x2));
	for(n = 1; n < nmax; n++)
	{
		double dfdx = (f2(x2)-f2(x1))/(x2-x1);

		if(fabs(dfdx) < 0.00001) {	// if the derivative is near zero
			printf("The derivative is near zero, dfdx=%f\n",dfdx);
			return 0;
		}

		d = f2(x2)/dfdx;
		double x3 = x2 - d;
		printf("Iteration no. %d, x1=%f, x2=%f, x=%f, f(x)=%f\n",n,x1,x2,x3,f2(x3));

		if(fabs(d) < eps) {
			printf("Converged at Iteration no. %d | the root = %f\n", n, x3);
			return n;
		}

		x1 = x2;
		x2 = x3;
	}

	printf("Did not converge. Try a different nmax value\n");
}

// implementation of Double False Position method
int double_false_position(double x1, double x2, int nmax, double eps, double (*f)(double))
{
	int n=0;
	double c, d, fx1, fx2, fx3;

	fx1 = f(x1);
	fx2 = f(x2);

	printf("Iteration no. %d, Initial guesses x1=%f, x2=%f, f(x1)=%f, f(x2)=%f\n", n,x1,x2,f(x1),f(x2));

	if((fx1 * fx2) > 0 ) {	// if the ranges have the same sign
		printf("The range of values [%f, %f] produce values f(%f)=%f and f(%f)=%f with the same sign\n",x1,x2,x1,fx1,x2,fx2);
		printf("Choose different range values\n");
		exit(1);
	}
	else {	//the root lies within this range
		for(n = 1; n < nmax; n++)
		{
			/* calculate estimate using secant method */
			double dfdx = (f(x2)-f(x1))/(x2-x1);

			if(fabs(dfdx) < 0.00001) {	// if the derivative is near zero
				printf("The derivative is near zero, dfdx=%f\n",dfdx);
				exit(1);
			}

			d = f(x2)/dfdx;
			double x3 = x2 - d;
			printf("Iteration no. %d, x1=%f, x2=%f, x=%f, f(x)=%f\n",n,x1,x2,x3,f(x3));

			// check if the difference in estimate from last time is less than epsilon
			if((fabs(d) < eps) || (fabs((x3-x2)/x2) < eps)) {
				printf("Converged at Iteration no. %d | the root = %f\n", n, x3);
				return n;
			}
			
			/* This is the bisection part */
			/* Use our current estimate x3 to bracket the root in next iteration */
			if((f(x2) * f(x3)) < 0) {
				x1 = x2;
				x2 = x3;
			}
			else {
				x1 = x1;
				x2 = x3;
			}
		}

		printf("Did not converge. Try a different nmax value\n");
	}
}


/* Function which uses Horner's method to evaluate a polynomial P(x)
 * Input: Vector of polynomial coefficients, degree of poly, and value of x
          at which to evaluate the polynomial
 * Output: Vector of Coefficients after synthetically dividing the polynomial
           by (x-x0). b_coeffs[0] = P(x0).
 */
double* horner_poly(double *coeffs, int degree, double x0)
{
	// initialize b coefficient of degree n
	double bn = coeffs[degree];

	// initialize vector of b coefficients
	double *b_coeffs = (double*)malloc((degree+1)*sizeof(double));
	b_coeffs[degree] = coeffs[degree];

	// recursively evaluate the rest of the b coefficients of Q(x) 
	int k;
	for(k = degree-1; k >= 0 ;k--)
	{
		bn = coeffs[k] + x0*bn;
		b_coeffs[k] = bn;
	}

	return b_coeffs;
}

/* Function which uses Horner's method to evaluate the derivative of polynomial P(x)
 * Input: Vector of polynomial coefficients, degree of poly, and value of x
          at which to evaluate the polynomial
 * Output: Derivative of Polynomial P(x) at x=x0
 */
double horner_deriv(double *coeffs, int degree, double x0)
{
	// initialize b coefficient of degree n
	double cn = coeffs[degree];

	// recursively evaluate the rest of the b coefficients of Q(x) 
	int k;
	for(k = degree-1; k >= 1 ;k--)
	{
		cn = coeffs[k] + x0*cn;
	}

	return cn;
}

/* Implementation of Birge-Vieta Method for Root Finding */
/* Input: Coefficients of Polynomial, degree of poly, initial guess, max iterations, epsilon */
/* Output: Approximation of a root of the Polynomial */
double birge_vieta(double *coeffs, int degree, double x0, int nmax, double eps)
{
	int n = 0;
	int j = 0;
	double d;
	/* bn will be P(x) and cn will be P'(x) evaluated at x0 */
	double bn, cn;

	printf("Iteration no. %d, Initial guess x=%f\n", n,x0);

	// Outer Loop corresponds to Newton method
	for(n = 1; n < nmax; n++)
	{
		// Use Horner Method to evaluate P(x) and P'(x)
		bn = coeffs[degree];
		cn = bn; 

		for(j = degree-1; j >= 1; j--)
		{
			bn = coeffs[j] + x0*bn;
			cn = bn + x0*cn;
		}
		bn = coeffs[0] + x0*bn;

		if(fabs(cn) < 0.00001) {	// if the derivative is near zero
			printf("The derivative is near zero, dfdx(x)=%f\n",cn);
			exit(1);
		}

		// Apply Newton Method to approximate the root
		d = bn/cn;
		x0 = x0 - d;
		printf("Iteration no. %d, x=%f\n", n,x0);

		if(fabs(d) < eps) {
			printf("Converged at Iteration no. %d | the root = %f\n", n, x0);
			return x0;
		}
	}

	printf("Did not converge. Try a different nmax value\n");
	return -1;
}

/* Implementation of Deflation Technique for finding all roots of Polynomial */
/* Input: Coefficients of Polynomial, degree of poly, initial guess, max iterations, epsilon */
/* Output: Vector of all non-complex roots for the polynomial */
double* deflation(double *coeffs, int degree, double x0, int nmax, double eps)
{
	int l = 0;
	int k = 0;
	int dgs = degree;
	
	// vector to hold roots of polynomial
	double *roots = (double*)malloc(degree*sizeof(double));
	
	// Guess first root of polynomial
	double root = birge_vieta(coeffs, degree, x0, nmax, eps);
	roots[0] = root;

	// Loop to run deflation technique for every possible root
	for(l = 1; l < dgs; l++)
	{
		double* b_coeffs = horner_poly(coeffs,degree,root);
		b_coeffs = b_coeffs+1;

		degree = degree-1;
		root = birge_vieta(b_coeffs,degree,root,nmax,eps);
		roots[l] = root;

		coeffs = b_coeffs;
	}

	return roots;
}


int main(int argc, char* argv[])
{
	/* Series of Procedures to Test Methods */

	printf("*** False Position Method ***\n");
	int n = double_false_position(0.0, 5.0, 50, 0.000001, f1);
	printf("\n");

	printf("*** Testing Secant Method Non-Convergence ***\n");
	int u = secant_f(0.0, 0.48, 50, 0.000001);
	printf("\n");

	printf("*** Testing Convergence on Same Funtion ***\n");
	int w = double_false_position(0.0, 0.48, 50, 0.000001, f2);
	printf("\n");

	printf("*** Testing Horner's Method for Evaluating Polynomial ***\n");
	printf("** P(x) = x^3 - 8x^2 + x +42 **\n");
	double *cs = (double*)malloc(4*sizeof(double));
	cs[0] = 42.0; cs[1] = 1.0; cs[2] = -8.0; cs[3] = 1.0;

	double* h = horner_poly(cs, 3, 3.0);
	printf("P(%f) = %f\n\n",3.0, h[0]);

	printf("*** Testing Horner's Method for Evaluating Derivative ***\n");
	double dhdx = horner_deriv(h,3,3.0);
	printf("P'(%f) = %f\n\n",3.0,dhdx);

	printf("*** Testing Birge-Vieta Method ***\n");
	double bv = birge_vieta(cs, 3,2.6,50,0.000001);
	printf("P(x) root = %f\n\n",bv);


	printf("*** Testing Deflation Technique ***\n");
	double *roots = deflation(cs, 3, 8.0, 50, 0.000001);
	printf("\nRoots of x^3 - 8x^2 + x + 42:\n");
	int l;
	for(l = 0; l < 3; l++)
		printf("Root #%d = %f, \n",l,roots[l]);

	return 0;
}
