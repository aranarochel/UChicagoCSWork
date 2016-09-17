/*! \file drawable.hxx
 *
 * \author Lamont Samuels
 */

/* CMSC23700 Lab 4 code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 Lamont Samuels
 * All rights reserved.
 */

#ifndef _DRAWABLE_HXX_
#define _DRAWABLE_HXX_

#include "cs237.hxx"
#include "mesh.hxx"
#include <vector> 

struct Drawable {
    Mesh  *mesh;  				//! meshes for this drawable object
    cs237::mat4f modelMat;      //! drawable's model matrix
    cs237::color4f  color;      //! drawable's color

    float shadowFactor;

    cs237::vec3f direc;			//! light matrices
    cs237::color3f intensity;
    cs237::color3f ambient;

    cs237::image2d  *  image;   //! drawable's image
    cs237::texture2D * texture; //! drawable's texture
    cs237::image2d * normalImage;   //! drawable's normal image
    cs237::texture2D * normalTexture;  //!drawable's normal texture map

    //! render a drawable using this renderer
    //! \param modelViewMat the model-view matrix for the drawable and camera
    virtual void Draw () = 0;
}; 

#endif /* _DRAWABLE_HXX_ */ 