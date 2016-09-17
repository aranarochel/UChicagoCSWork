/*! \file cs237-sphere.cxx
 *
 * \brief code to represent a sphere
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include <cstring>

using namespace cs237;

/* the representation of the Icosahedron */
static vec3f IcosVerts[12];		// vertices
static unsigned short IcosTris[60] = {	// vertex indices for the faces in CCW order
	0, 8, 4,	// T0
	0, 5, 10,	// T1
	2, 4, 9,	// T2
	2, 11, 5,	// T3
	1, 6, 8,	// T4
	1, 10, 7,	// T5
	3, 9, 6,	// T6
	3, 7, 11,	// T7
	0, 10, 8,	// T8
	1, 8, 10,	// T9
	2, 9, 11,	// T10
	3, 11, 9,	// T11
	4, 2, 0,	// T12
	5, 0, 2,	// T13
	6, 1, 3,	// T14
	7, 3, 1,	// T15
	8, 6, 4,	// T16
	9, 4, 6,	// T17
	10, 5, 7,	// T18
	11, 7, 5	// T19
    };

static bool isFirst = true;

//! \brief initialize the vertices of the base Icosahedron.
//
//! This code is based on the treatment in
//! Geometric Tools for Computer Graphics (Morgan Kaufmann 2003).
//
static void InitIcosahedron ()
{
    float t = (1.0 + sqrt(5.0)) * 0.5;
    float c = 1.0 / sqrt(1.0 + t*t);

    IcosVerts[ 0] = c * vec3f( t,   1.0,   0.0);
    IcosVerts[ 1] = c * vec3f(-t,   1.0,   0.0);
    IcosVerts[ 2] = c * vec3f( t,  -1.0,   0.0);
    IcosVerts[ 3] = c * vec3f(-t,  -1.0,   0.0);
    IcosVerts[ 4] = c * vec3f( 1.0,  0.0,  t);
    IcosVerts[ 5] = c * vec3f( 1.0,  0.0, -t);
    IcosVerts[ 6] = c * vec3f(-1.0,  0.0,  t);
    IcosVerts[ 7] = c * vec3f(-1.0,  0.0, -t);
    IcosVerts[ 8] = c * vec3f( 0.0,  t,    1.0);
    IcosVerts[ 9] = c * vec3f( 0.0, -t,    1.0);
    IcosVerts[10] = c * vec3f( 0.0,  t,   -1.0);
    IcosVerts[11] = c * vec3f( 0.0, -t,   -1.0);

} // InitIcosahedron

/***** struct Sphere member functions *****/

/*! \brief construct a sphere with the given radius
 */
Sphere::Sphere (float radius)
{
    if (isFirst) {
	InitIcosahedron ();
	isFirst = false;
    }

    this->numTris = 20;
    this->numVerts = 12;
    this->tris = new unsigned short [3 * this->numTris];
    this->verts = new vec3f [this->numVerts];
    this->norms = new vec3f [this->numVerts];

    memcpy (this->tris, IcosTris, 3 * this->numTris * sizeof(unsigned short));

    for (int i = 0;  i < this->numVerts;  i++) {
	this->verts[i] = radius * IcosVerts[i];
	this->norms[i] = normalize(IcosVerts[i]);
    }

}

/*! \brief delete a sphere mesh and its associated data arrays.
 */
Sphere::~Sphere ()
{
    delete this->tris;
    delete this->verts;
    delete this->norms;
}
