/*! \file instance.hxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _INSTANCE_HXX_
#define _INSTANCE_HXX_

#include "cs237.hxx"
#include "mesh.hxx"

//! one or more meshes that correspond to the groups in the OBJ file
typedef std::vector<MeshInfo*> GObject;

//! An instance of a graphical object in the scene
struct Instance {
    GObject		meshes;		//!< the mesh data
    cs237::mat4f	toWorld;	//!< affine transform from object space to world space
    cs237::mat3f	normToWorld;	//!< linear transform that maps object-space normals
					//!  to world-space normals
    cs237::mat3f	normFromWorld;	//!< linear transform that maps world-space normals
					//!  to object-space normals (e.g., light)
    cs237::AABBf	bbox;		//!< the bounding box around the transformed mesh
    cs237::color3f	color;		//!< the color of the object
};

#endif /*! _INSTANCE_HXX_ */
