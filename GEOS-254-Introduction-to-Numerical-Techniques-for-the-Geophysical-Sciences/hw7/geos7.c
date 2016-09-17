#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// finite difference method for the heat equation 
// Input: the thermal diffusivity (th_diff)
//        and a vector of initial temperatures for each position
double *finite_diff(double th_diff, double *initial_cond)
{
	int i = 0;
	int k = 0;
	// create vector to store old temperatures
	double *old_temps = (double*)malloc(101*sizeof(double));

	// number of time steps is 300 milleniums 
	int endtime = 300;	
	// size of our timestep is 1000 years (in seconds)
	double dt = 3.154E10; 
	// our position step is 1km in meters
	double dx = 1000.0;

	// D*dt/(dx)^2
	double c = (th_diff*dt)/(dx*dx);
	// (1-2c)
	double c2 = 1.0 - (2.0*c);

	// loop over time steps
	for(i = 0; i < endtime; i++)
	{
		// make copy of old temps from previous time step
		memcpy(old_temps, initial_cond, 101*sizeof(double));
		// loop over positions of slab
		for(k = 1; k < 100; k++)
		{
			initial_cond[k] = c*old_temps[k-1] + c2*old_temps[k] + c*old_temps[k+1];
		}
		printf("TIME STEP %d millennia :: Temp(50km)=%f K\n",i+1,initial_cond[50]);
	}
	return initial_cond;
}



// MAIN FUNCTION
int main(int argc, char* argv[])
{
	int i;
	// create initial condition vector
	double *init = (double*)malloc(101*sizeof(double));
	for(i = 0; i < 101; i++)
	{
		init[i] = 280.0;
		//printf("%d: %f\n",i,init[i]);
	}

	// change temps of z=49,50,51 to 1000K
	init[49]=1000.0;
	init[50]=1000.0;
	init[51]=1000.0;

	double *heat = finite_diff((double)8E-7,init);

	return 0;
}