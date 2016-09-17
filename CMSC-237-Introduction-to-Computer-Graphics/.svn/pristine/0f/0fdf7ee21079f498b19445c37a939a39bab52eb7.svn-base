/*! \file spot-light.cxx
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "spot-light.hxx"

//! initialize the additional fields of the spot light from scene
void SpotLight::Init ()
{
	float maxI = fmax(fmax(this->intensity.r,this->intensity.g),fmax(this->intensity.r,this->intensity.b));
	//std::cout<< "Number " << maxI << "\n";
	float inner1 = this->atten[0] - (255.0f * maxI);
	inner1 = pow(this->atten[1],2.0) - (4.0*this->atten[2]*inner1);
	inner1 = sqrt(inner1);
	float dist = ((-1.0*this->atten[1]) + inner1) / (2.0*this->atten[2]);
	this->maxDist = dist;

}
