/*! \file drawable.hxx
 *
 * \author John Reppy
 * \author Lamont Samuels
 */

/* CMSC23700 Lab 4 code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr) & Lamont Samuels
 * All rights reserved.
 */

#ifndef _CUBE_HXX_
#define _CUBE_HXX_

#include "cs237.hxx"
#include "mesh.hxx"
#include "drawable.hxx"

struct Cube : Drawable { 
	
	Cube(); 

  	//! render a drawable using this renderer
  	//! \param modelViewMat the model-view matrix for the drawable and camera
    void Draw ();
}; 

#endif /* _CUBE_HXX_ */ 