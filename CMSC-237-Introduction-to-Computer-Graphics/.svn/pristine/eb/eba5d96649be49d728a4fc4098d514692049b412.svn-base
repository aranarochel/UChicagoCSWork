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
#include "drawable.hxx"

//! an abstract base class that wraps a renderer behavior
//
class Renderer {
  public:

  //! enable the renderer
  //! \param projectMat the projection matrix for the current camera state
    virtual void Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight) = 0;

    virtual ~Renderer ();

    virtual void Render (cs237::mat4f const &modelViewMat,cs237::mat4f const &modelViewMatLight, Drawable *drawable);

  protected:
    cs237::ShaderProgram	*_shader;	//!< the shader program


    /* Vertex uniforms */
    //GLint mPosition;
    GLint modelView;              /*!< the uniform location for camera matrix in the shaders*/ 
    GLint projection;           /*!< the uniform location for project view matrix in the shaders*/ 
    GLint modelViewLight;
    GLint projectionLight;

    /* Fragment Uniforms */
    GLint Vcolor;
    GLint _samplerLoc;          /* uniform for the sampler loc */
    GLint _samplerLocShadow;
    GLint direction;
    GLint ambient;
    GLint intensity;
    GLint shadow;


    Renderer (cs237::ShaderProgram *sh);

};

class WireframeRenderer : public Renderer {
  public:
    WireframeRenderer ();
    virtual ~WireframeRenderer ();

    void Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight);
};

class FlatShadingRenderer : public Renderer {
  public:
    FlatShadingRenderer ();
    virtual ~FlatShadingRenderer ();

    void Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight); 
};

class LightingRenderer : public Renderer {
  public:
    LightingRenderer ();
    virtual ~LightingRenderer ();

    void Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight);

    void Render (cs237::mat4f const &modelViewMat,cs237::mat4f const &modelViewMatLight, Drawable *drawable); 
};

class TextureRenderer : public Renderer {
  public:
    TextureRenderer ();
    virtual ~TextureRenderer ();

    void Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight);

    void Render (cs237::mat4f const &modelViewMat,cs237::mat4f const &modelViewMatLight, Drawable *drawable); 
};

class DepthRenderer : public Renderer {
  public:
    DepthRenderer ();
    virtual ~DepthRenderer ();

    void Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight);

    void Render (cs237::mat4f const &modelViewMat, cs237::mat4f const &modelViewMatLight, Drawable *drawable);  
};

class ShadowRenderer : public Renderer {
  public:
    ShadowRenderer ();
    virtual ~ShadowRenderer ();

    void Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight);

    void Render (cs237::mat4f const &modelViewMat,cs237::mat4f const &modelViewMatLight, Drawable *drawable);
};

#endif // !_RENDER_HXX_
