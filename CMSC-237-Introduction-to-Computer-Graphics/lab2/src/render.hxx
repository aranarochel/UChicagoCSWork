/*! \file render.hxx
 *
 * \author John Reppy
 * \author Lamont Samuels
 */

/* CMSC23700 Lab 2 code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr) & Lamont Samuels
 * All rights reserved.
 */

#ifndef _RENDER_HXX_
#define _RENDER_HXX_

#include "cs237.hxx"
#include "mesh.hxx"

//! an abstract base class that wraps a renderer behavior
//
class Renderer {
  public:

  //! enable the renderer
  //! \param projectMat the projection matrix for the current camera state
    virtual void Enable (cs237::mat4f const &projectionMat) = 0;

  //! render a mesh using this renderer
  //! \param modelViewMat the model-view matrix for the mesh and camera
  //! \param mesh the mesh to be rendered
    void Render (cs237::mat4f const &modelViewMat, Mesh *mesh);

    virtual ~Renderer ();

  protected:
    cs237::ShaderProgram  *_shader; //!< the shader program
    /** HINT: You shold create variables for your uniform locations: model-view, projection, and color **/
    /** YOUR CODE HERE **/

    GLint modelView;              /*!< the uniform location for camera matrix in the shaders*/ 
    GLint projection;           /*!< the uniform location for project view matrix in the shaders*/ 
    GLint Vcolor;                /* uniform location of color */
    
    Renderer (cs237::ShaderProgram *sh);

};
class FlatShadingRenderer : public Renderer {
  public:
    FlatShadingRenderer ();
    virtual ~FlatShadingRenderer ();

    void Enable (cs237::mat4f const &projectionMat);

};

#endif // !_RENDER_HXX_
