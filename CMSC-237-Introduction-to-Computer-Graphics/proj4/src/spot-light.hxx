/*! \file spot-light.hxx
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _SPOT_LIGHT_HXX_
#define _SPOT_LIGHT_HXX_

#include  "cs237.hxx"

struct SpotLight {
    std::string		name;		//!< the light's name
    cs237::vec3f	pos;		//!< the light's world-space position
    cs237::vec3f	dir;		//!< unit-length direction vector
    float		cutoff;		//!< the cutoff angle for the light (in degrees)
    float		exponent;	//!< the focus exponent
    cs237::color3f	intensity;	//!< the light's intensity
    float		atten[3];	//!< attenuation coefficients (constant, linear, quadratic)
  /* additional fields that are useful for rendering the light volumes */
    float		maxDist;	//!< the maximum distance at which the light has non-zero
					//!< intensity
    float		cosCutoff;	//!< cos(cutoff)

    cs237::vec3f    camPos;     //!< initial camera position in world space
    cs237::vec2f    windowSize; //window size
    float           lightType;   // type of light. 1 == direc, 2 == spot. I declare this here instead of passing it
                                // along to the renderer

  //! initialize the additional fields of the spot light from scene (e.g., maxDist, etc.)
    void Init ();

};

#endif // !_SPOT_LIGHT_HXX_
