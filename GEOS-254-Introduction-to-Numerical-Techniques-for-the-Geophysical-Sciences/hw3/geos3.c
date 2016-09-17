#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// function to do gaussian elimination for problem 1
void gaussElim(double A[][4], double b[])
{
	int i,j,k;
	int rows = 4;
	int columns = 4;

	for(k=0; k < rows-1; k++) {	// equation pivot
		for(i=k+1; i < rows; i++) {	// row operating on

			double val = (A[i][k])/(A[k][k]);

			for(j=k; j < columns; j++) {	// column operating on
				A[i][j] = A[i][j] - (val* A[k][j]);
			}

			b[i] = b[i] - (val*b[k]);
		}
	}
}


// function to perform forward elimination for a tridiagonal system
void tridiagElim(double *a, double *b, double *c, double *d)
{
	int i;

	for(i = 1; i < 100; i++) {

		double f = a[i]/b[i-1];

		b[i] = b[i] - (f*c[i-1]);
		d[i] = d[i] - (f*d[i-1]);
	}
}

// function to solve for the variables in problem 1
double *solve(double A[][4], double b[])
{
	int i,j,k; 
	int rows = 4;
	int columns = 4;
	double sum;

	double *vars = (double*)malloc(4*sizeof(double));

	// first step in the backwards substitution
	vars[3] = (b[3])/(A[3][3]);

	for(i=rows-2; i >= 0; i--) {
		sum = b[i];

		for(j=i+1; j < columns; j++) {
			sum = sum - (A[i][j] * vars[j]);
		}

		vars[i] = sum/A[i][i];
	}

	return vars;
}

// function to solve for the variables in problem 2
double *tridiagSolve(double *b, double *c, double *d)
{
	int i;

	double *vars = (double*)malloc(100*sizeof(double));

	//first step in backwards substitution
	vars[99] = d[99]/b[99];

	for(i=98; i >=0; i--) {
		vars[i] = (d[i] - (c[i]*vars[i+1])) / b[i];
	}

	return vars;
}


int main(int argc, char* argv[])
{
	int i, j, k;

	/*** PROBLEM 1 ***/
	// the matrix A and the vector b
	double A[4][4] = {{1,-1,1,2},{-1,3,2,1},{3,-5,-5,-2},{2,1,-1,-3}};
	double B[4] = {8,-3,14,0};

	gaussElim(A,B);	// perform gauss elimination

	printf("**** PROBLEM #1 ****\n");
	printf("**PRINTING MATRIX A AFTER GAUSSIAN ELIMINATION**\n");
	for(i=0; i < 4; i++) {
		for(j=0; j < 4; j++) {
			printf("%f,", A[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("**PRINTING VECTOR b AFTER GAUSSIAN ELIMINATION**\n");
	for(k=0; k < 4; k++)
		printf("%f,",B[k] );
	printf("\n\n");

	double *vars = solve(A,B);	// solve for variables

	printf("**PRINTING SOLUTIONS TO THE SYSTEM OF EQUATIONS**\n");
	for(i=0; i < 4; i++) {
		printf("X%i = %f, ",i+1,vars[i]);
	}
	printf("\n\n\n");


	/*** PROBLEM 2 ***/
	printf("**** PROBLEM #2 ****\n");

	// create the diagonal vectors of the system
	double *a = (double*)malloc(100*sizeof(double));
	double *b = (double*)malloc(100*sizeof(double));
	double *c = (double*)malloc(100*sizeof(double));
	double *d = (double*)malloc(100*sizeof(double));

	for(i=0; i <100; i++) {
		a[i] = 0.5;
		b[i] = 1.0;
		c[i] = 0.5;
		d[i] = 10.0;
	}
	a[0]=0.0;
	c[99]=0.0;
	d[0]=8.0;
	d[99]=12.0;

	tridiagElim(a,b,c,d);

	double *diagVars = tridiagSolve(b,c,d);

	printf("**PRINTING THE SOLUTIONS TO THE TRIDIAGONAL SYSTEM**\n");
	for(i=0; i < 100; i++) {
		if(i % 5 == 0)
			printf("\n");
		printf("X%i = %f, ",i+1, diagVars[i]);
	}
	printf("\n");

	return 0;
}