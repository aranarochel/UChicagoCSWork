#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// function to do gaussian elimination for problem 1
void gaussElim(double** A, double b[], int n)
{
	int i,j,k;
	int rows = n;
	int columns = n;

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


// function to solve for the variables in problem 1
double *solve(double** A, double b[], int n)
{
	int i,j,k; 
	int rows = n;
	int columns = n;
	double sum;

	double *vars = (double*)malloc(n*sizeof(double));

	// first step in the backwards substitution
	vars[n-1] = (b[n-1])/(A[n-1][n-1]);

	for(i=rows-2; i >= 0; i--) {
		sum = b[i];

		for(j=i+1; j < columns; j++) {
			sum = sum - (A[i][j] * vars[j]);
		}

		vars[i] = sum/A[i][i];
	}

	return vars;
}

// function which returns the a and b coefficients to the least squares line
double* least_squares(double x[], double y[], int n)
{
	int i;
	double *ret = (double*)malloc(sizeof(double)*2);

	// define variables for easy solving
	double p=0, r=0, q=0, s=0;

	for(i = 0; i < n; i++) {
		p = p+x[i];
		r = r + (x[i]*y[i]);
		q = q + y[i];
		s = s + (x[i]*x[i]);
	}

	double a = ((n*r)-(p*q)) / ((n*s)-(p*p));
	double b = ((s*q)-(p*r)) / ((n*s)-(p*p));

	ret[0] = a;
	ret[1] = b;
	return ret;
}


// helper function to return one row of the monomial basis function matrix
// Input: t= x data point, n=number of data points
double *basis_fun_mon_helper(double t, int n)
{
	double *ret = (double *)malloc(sizeof(double)*n);

	// monomial basis function for loop
	int i;
	for(i = 0; i < n; i++) 
		ret[i] = pow(t,i);

	return ret;
}

double **basis_fun_mon(double t[], int n)
{
	double **mat = (double **)malloc(sizeof(double*)*n);

	int i;
	for(i = 0; i < n; i++)
	{
		mat[i] = basis_fun_mon_helper(t[i], n);
	}

	return mat;
}

// helper function to return one row of the exponential basis function matrix
// Input: t= x data point, n=number of data points
double *basis_fun_exp_helper(double t, int n)
{
	double *ret = (double *)malloc(sizeof(double)*n);

	// exponential basis function for loop
	int i;
	for(i = 0; i < n; i++) 
		ret[i] = exp(-1*i*t);

	return ret;
}

double **basis_fun_exp(double t[], int n)
{
	double **mat = (double **)malloc(sizeof(double*)*n);

	int i;
	for(i = 0; i < n; i++)
	{
		mat[i] = basis_fun_exp_helper(t[i], n);
	}

	return mat;
}

// the curve fit using the monomial basis function
// returns an estimate given an R input (1000's of km's)
double mon_curve_fit(double coeff[], double t, int n)
{
	double sum = 0;
	int i;

	for (i = 0; i < n; i++)
	{
		sum = sum + coeff[i]*pow(t,i);
	}

	return sum;
}

// the curve fit using the exponential basis function
// returns an estimate given an R input (1000's of km's)
double exp_curve_fit(double coeff[], double t, int n)
{
	double sum = 0.0;
	int i;

	for (i = 0; i < n; i++)
	{
		sum = sum + coeff[i]*exp(-1*t*i);
	}

	return sum;
}	


int main(int argc, char* argv[])
{
	int i, j, k;

	// ****** Problem 1 ******
	printf("***** PROBLEM #1 *****\n\n");
	// make our R (km) input into (1000's km)
	double R[14] = {0,500,1000,1500,2000,2500,3000,3500,4000,4500,5000,5500,6000,6371};
	for(k = 0; k < 14; k++)
		R[k] = R[k]/1000.0;

	// make R input for the exponential into (10,000's of km)
	double R2[14] = {0,500,1000,1500,2000,2500,3000,3500,4000,4500,5000,5500,6000,6371};
	for(k = 0; k < 14; k++)
		R2[k] = R2[k]/10000.0;

	// matrices
	double **mat_mon = basis_fun_mon(R, 14);
	double **mat_exp = basis_fun_exp(R2, 14);

	// rho values
	double rho[14] = {13.0885, 13.034, 12.871, 12.010, 11.655,11.191
				   ,10.602, 5.540, 5.305, 5.051, 4.792, 4.504, 3.528, 1.02};
	double rho2[14] = {13.0885, 13.034, 12.871, 12.010, 11.655,11.191
				   ,10.602, 5.540, 5.305, 5.051, 4.792, 4.504, 3.528, 1.02};

	// solve system of equations
	gaussElim(mat_mon, rho, 14);
	double *coefficients = solve(mat_mon, rho, 14);
	gaussElim(mat_exp, rho2, 14);
	double *coefficients2 = solve(mat_exp, rho2, 14);

	printf("***THE COEFFICIENTS w/MONOMIAL BASIS FUNCTION***\n");
	for (j = 0; j < 14; j++)
	{
		if(j % 6 == 0)
			printf("\n");
		printf("C%d = %f, ",j,coefficients[j] );
	}
	printf("\n\n");

	printf("***THE COEFFICIENTS w/EXPONENTIAL BASIS FUNCTION***\n");
	for (j = 0; j < 14; j++)
	{
		if(j % 6 == 0)
			printf("\n");
		printf("C%d = %f, ",j,coefficients2[j]);
	} 
	printf("\n\n");
	printf("***ESTIMATES***\n");
	printf("Monomial: Density of Earth 1200 km from center estimate = %f\n",mon_curve_fit(coefficients,1.2,14) );
	printf("Exponential: Density of Earth 1200 km from center estimate = %f\n",exp_curve_fit(coefficients2,0.12,14) );
	printf("Monomial: Density of Earth 6271 km from center estimate = %f\n",mon_curve_fit(coefficients,6.271,14) );
	printf("Exponential: Density of Earth 6271 km from center estimate = %f\n",exp_curve_fit(coefficients2,0.6271,14) );

	// repeating the process but only using last 4 data points
	double R3[4] = {5000,5500,6000,6371};
	for(k = 0; k < 4; k++)
		R3[k] = R3[k]/1000.0;
	double R4[4] = {5000,5500,6000,6371};
	for(k = 0; k < 4; k++)
		R4[k] = R4[k]/10000.0;

	// matrices for shortened data points
	double **mat_mon2 = basis_fun_mon(R3, 4);
	double **mat_exp2 = basis_fun_exp(R4, 4);

	// rho values
	double rho3[4] = {4.792, 4.504, 3.528, 1.02};
	double rho4[4] = {4.792, 4.504, 3.528, 1.02};

	// solve system of equations
	gaussElim(mat_mon2, rho3, 4);
	double *coefficients3 = solve(mat_mon2, rho3, 4);
	gaussElim(mat_exp2, rho4, 4);
	double *coefficients4 = solve(mat_exp2, rho4, 4);

	printf("\n");
	printf("***ESTIMATES USING LAST 4 DATA POINTS***\n");
	printf("Monomial: Density of Earth 6271 km from center estimate = %f\n",mon_curve_fit(coefficients3,6.271,4) );
	printf("Exponential: Density of Earth 6271 km from center estimate = %f\n",exp_curve_fit(coefficients4,0.6271,4) );

	printf("\n");
	printf("***ESTIMATE IF WE MEASURE DEPTH FROM SURFACE***\n");
	double a[14] = {6371,5871,5371,4871,4371,3871,3371,2871,2371,1871,1371,871,371,0};
	for(k = 0; k < 14; k++)
		a[k] = a[k]/1000.0;
	double **mat_mon_a = basis_fun_mon(a, 14);
	double rho_a[14] = {13.0885, 13.034, 12.871, 12.010, 11.655,11.191
				   ,10.602, 5.540, 5.305, 5.051, 4.792, 4.504, 3.528, 1.02};
	gaussElim(mat_mon_a, rho_a, 14);
	double *coefficients_a = solve(mat_mon_a, rho_a, 14);

	printf("Monomial: Density of Earth 1200 km from center (Depth of 5171 km from surface) estimate = %f\n",mon_curve_fit(coefficients_a,6.371-1.2,14) );
	printf("Monomial: Density of Earth 6271 km from center (Depth of 100 km from surface) estimate = %f\n",mon_curve_fit(coefficients_a,6.371-6.271,14) );
	printf("\n\n");

	// ****** Problem 1 ******
	printf("***** PROBLEM #2 *****\n\n");
	double t[8] = {-3.5,-1.8,-0.4,-0.2,0.5,1.2,2.0,2.8};
	double y[8] = {0.0109,0.989,4.62,4.901,4.412,2.434,0.677,0.099};
	double t2[8] = {-3.5,-1.8,-0.4,-0.2,0.5,1.2,2.0,2.8};
	double y2[8] = {0.0109,0.989,4.62,4.901,4.412,2.434,0.677,0.099};

	// matrices
	double **mat1 = basis_fun_mon(t, 8);
	// solve system of equations
	gaussElim(mat1, y, 8);
	double *coeff1 = solve(mat1, y, 8);

	printf("***PROB 2:THE COEFFICIENTS w/MONOMIAL BASIS FUNCTION***\n");
	for (j = 0; j < 8; j++)
	{
		if(j % 4 == 0)
			printf("\n");
		printf("C%d = %f, ",j,coeff1[j] );
	}
	printf("\n\n");

	double *ls = least_squares(t2,y2,8);

	printf("a:%f, b:%f\n",ls[0],ls[1] );


	return 0;
}
// fooplot
//13.0885 - 1361.62 x + 8233.5 x^2 - 20692.625254 x^3 + 29100.900029 x^4 - 25824.355880 x^5 + 15372.284289 x^6 - 6338.07 x^7 + 1834.1 x^8 - 371.455 x^9 + 51.5547 x^10 - 4.67065 x^11 + 0.248744 x^12 - 0.005905 x^13
//-0.005905 x^13+0.248744 x^12-4.67065 x^11+51.5547 x^10-371.455 x^9+1834.1 x^8-6338.07 x^7+15372.284289 x^6-25824.355880 x^5+29100.900029 x^4-20692.625254 x^3+8233.5 x^2-1361.62 x+13.0885

//-55123799219.207520+1012809726066.899658e^(-x)-8558617899510.612305e^(-2x)+44042422177217.234375e^(-3x)-153982824858716.34375e^(-4x)+386246990022165.4375e^(-5x)-715119599461482.875e^(-6x)+989473854207228.25e^(-7x)-1023150128301797.75e^(-8x)+780885132203473.75e^(-9x)-427576085308108.6875e^(-10x)+159057702910939.5625e^(-11x)-36029994981379.492188e^(-12x)+3753463363136.949707e^(-13x)