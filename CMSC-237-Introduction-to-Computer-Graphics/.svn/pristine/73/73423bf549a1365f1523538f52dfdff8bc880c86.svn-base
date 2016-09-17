/*! \file render.hxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Autumn 2014)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _RENDER_HXX_
#define _RENDER_HXX_

#include <vector>
#include "cs237.hxx"
#include "mesh.hxx"

//! an abstract base class that wraps a renderer behavior
//
class Renderer {
  public:

  //! enable the renderer
  //! \param projectMat the projection matrix for the current camera state
    virtual void Enable (cs237::mat4f const &projectionMat) = 0;

    virtual ~Renderer ();

    /** YOUR CODE HERE **/
    void Render (cs237::mat4f const &modelViewMat, Mesh *mesh);

  protected:
    cs237::ShaderProgram	*_shader;	//!< the shader program

    /** YOUR CODE HERE **/
    GLint mPosition;
    GLint modelView;              /*!< the uniform location for camera matrix in the shaders*/ 
    GLint projection;           /*!< the uniform location for project view matrix in the shaders*/ 
    GLint Vcolor;

    Renderer (cs237::ShaderProgram *sh);

};

class WireframeRenderer : public Renderer {
  public:
    WireframeRenderer ();
    virtual ~WireframeRenderer ();

    void Enable (cs237::mat4f const &projectionMat);

    /** YOUR CODE HERE **/
};

class FlatShadingRenderer : public Renderer {
  public:
    FlatShadingRenderer ();
    virtual ~FlatShadingRenderer ();

    void Enable (cs237::mat4f const &projectionMat);

    /** YOUR CODE HERE **/
};

#endif // !_RENDER_HXX_
