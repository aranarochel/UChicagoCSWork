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
#include "drawable.hxx"
#include "depth-buffer.hxx"

//! an abstract base class that wraps a renderer behavior
//
class Renderer {
  public:

  //! enable the renderer
  //! \param projectMat the projection matrix for the current camera state
    virtual void Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight) = 0;

  //! render a mesh using this renderer
  //! \param modelViewMat the model-view matrix for the mesh and camera
  //! \param mesh the mesh to be rendered
    virtual void Render (cs237::mat4f const &modelViewMat,cs237::mat4f const &modelViewMatLight, Drawable * drawable) = 0;

    virtual ~Renderer ();

  protected:
    cs237::ShaderProgram  *_shader; //!< the shader program
  // Common uniform variable locations
    int       _mvLoc;   //!< the model-view matrix uniform
    int       _projLoc; //!< the projection matrix uniform
    int       _mvLocLight;
    int       _projLocLight;

    GLint Vcolor;
    GLint _samplerLoc;
    GLint _samplerLocShadow;
    GLint direction;
    GLint ambient;
    GLint intensity;

    Renderer (cs237::ShaderProgram *sh);

};

class LightingRenderer : public Renderer {
  public:
    LightingRenderer ();
    virtual ~LightingRenderer ();

    void Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight);

    /** YOUR CODE HERE **/
    void Render (cs237::mat4f const &modelViewMat,cs237::mat4f const &modelViewMatLight, Drawable *drawable);
    
};

class TextureRenderer : public Renderer {
  public:
    TextureRenderer ();
    virtual ~TextureRenderer ();

    void Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight);

    /** YOUR CODE HERE **/
    void Render (cs237::mat4f const &modelViewMat,cs237::mat4f const &modelViewMatLight, Drawable *drawable);
    
};

class DepthRenderer : public Renderer {
  public:
    DepthRenderer ();
    virtual ~DepthRenderer ();

    void Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight);

    /** YOUR CODE HERE **/
    void Render (cs237::mat4f const &modelViewMat, cs237::mat4f const &modelViewMatLight, Drawable *drawable);
    
};

#endif // !_RENDER_HXX_