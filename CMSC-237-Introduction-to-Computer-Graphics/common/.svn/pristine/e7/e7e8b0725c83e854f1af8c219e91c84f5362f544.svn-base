/*! \file cs237-sphere.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_SPHERE_HXX_
#define _CS237_SPHERE_HXX_

#ifndef _CS237_HXX_
#error "cs237-sphere.hxx should not be included directly"
#endif

namespace cs237 {

  //! A spherical mesh
    struct Sphere {
	int		numTris;	//!< number of triangles in the sphere mesh
	int		numVerts;	//!< number of vertices and vertex normals in the mesh
	unsigned short	*tris;		//!< array of 3*numTris triangle indices
	vec3f		*verts;		//!< array of numVerts vertices
	vec3f		*norms;		//!< array of numVerts vertex normals

	Sphere (float radius = 1.0f);
	~Sphere ();

    };

} /* namespace cs237 */

#endif /* !_CS237_SPHERE_HXX_ */
