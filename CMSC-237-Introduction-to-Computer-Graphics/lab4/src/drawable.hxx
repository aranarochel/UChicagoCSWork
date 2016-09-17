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
    std::vector<Mesh *> meshes; //! meshes for this drawable object
    cs237::mat4f modelMat;      //! drawable's model matrix
    cs237::color4f  color;      //! drawable's color
    cs237::image2d  *  image;   //! drawable's image
    cs237::texture2D * texture; //! drawable's texture
    bool shouldTexturize;       //! true, if the drawable should be textured, otherwise false.

    //! render a drawable using this renderer
    //! \param modelViewMat the model-view matrix for the drawable and camera
    virtual void Draw () = 0;
}; 

#endif /* _DRAWABLE_HXX_ */ 