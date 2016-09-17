/*! \file render.hxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _RENDER_HXX_
#define _RENDER_HXX_

#include "cs237.hxx"
#include "instance.hxx"
#include "g-buffer.hxx"
#include "spot-light.hxx"

//! an abstract base class that wraps a renderer behavior
//
class RenderPass {
  public:

  //! enable the renderer; this function does initialization that is
  //! common to multiple objects rendered with the same renderer
    virtual void Enable () = 0;

  //! render a mesh using this renderer
  //! \param projMat the projection matrix for the current camera state
  //! \param viewMat the view matrix for the camera
  //! \param mesh the mesh to be rendered
    virtual void Render (cs237::mat4f const &projMat, cs237::mat4f const &viewMat, const Instance *mesh,SpotLight light) = 0;

    virtual ~RenderPass ();

  protected:
    cs237::ShaderProgram	*_shader;	//!< the shader program

  //! constructor (protected)
    RenderPass (cs237::ShaderProgram *sh);

};

//! the renderer for WIREFRAME mode
class WireframeRenderer : public RenderPass {
  public:
    WireframeRenderer ();
    virtual ~WireframeRenderer ();

    void Enable ();

    void Render (cs237::mat4f const &projMat, cs237::mat4f const &viewMat, const Instance *mesh,SpotLight light);

  protected:
    int				_mvpLoc;	//!< the model-view-projection matrix uniform
    int				_colorLoc;	//!< the color or colormap uniform
};

//! A renderer to draw the scene using just the directional light and texture mapping.
//
class TexturingRenderer : public RenderPass {
  public:
    TexturingRenderer (cs237::vec3f const &dir, cs237::color3f const &i, cs237::color3f const &amb);
    ~TexturingRenderer ();

    void Enable ();
    void Render (cs237::mat4f const &projMat, cs237::mat4f const &viewMat, const Instance *mesh,SpotLight light);

  protected:
    cs237::vec3f	_lightDir;	//!< unit vector that specifies the light's direction
    cs237::color3f	_lightI;	//!< the directional light's intensity
    cs237::color3f	_ambI;		//!< the ambient light's intensity
  // uniform variable locations for lighting
    int			_mvpLoc;	//!< the model-view-projection matrix uniform
    int			_normMatLoc;	//!< the normal-vector transform matrix uniform
    int			_lightDirLoc;	//!< the uniform vector toward the light
    int			_lightILoc;	//!< the light intensity uniform
    int			_ambILoc;	//!< the ambient intensity uniform
    int			_hasDiffuseMapLoc; //!< the location of the diffuse texture-mapping flag 
    int			_diffuseCLoc;	//!< the diffuse-color uniform
    int			_diffuseMapLoc;	//!< the diffuse-color-map texture uniform

};

//! the GBuffer (aka geometry pass) renderer
class GBufferRenderer : public RenderPass {
  public:
    GBufferRenderer ();
    virtual ~GBufferRenderer ();

    void Enable ();

    void Render (cs237::mat4f const &projMat, cs237::mat4f const &viewMat, const Instance *mesh,SpotLight light);

  protected:
    int       _mvpLoc;  //!< the model-view-projection matrix uniform
    int       _normMatLoc;  //!< the normal-vector transform matrix uniform
    int       _toWorldLoc;  //!< affine transform from object space to world space
    int     _hasDiffuseMapLoc; //!< the location of the diffuse texture-mapping flag 
    int     _diffuseCLoc; //!< the diffuse-color uniform
    int     _diffuseMapLoc; //!< the diffuse-color-map texture uniform

};

// the deferred renderer for the lights
class DeferredRenderer : public RenderPass {
  public:
    DeferredRenderer (cs237::vec3f const &dir, cs237::color3f const &i, cs237::color3f const &amb);
    ~DeferredRenderer ();

    void Enable ();
    void Render (cs237::mat4f const &projMat, cs237::mat4f const &viewMat, const Instance *mesh, SpotLight light);

  protected:
    cs237::vec3f  _lightDir;  //!< unit vector that specifies the light's direction
    cs237::color3f  _lightI;  //!< the directional light's intensity
    cs237::color3f  _ambI;    //!< the ambient light's intensity
  // uniform variable locations for lighting
    int     _lightType; // will be set to 1 for directional or 2 for spot

    int       _mvpLoc;  //!< the model-view-projection matrix uniform
    int     _lightDirLoc; //!< the uniform vector toward the light
    int     _lightILoc; //!< the light intensity uniform
    int     _ambILoc; //!< the ambient intensity uniform

    int     _samplerPos;    //samplers for G-Buffer Data
    int     _samplerDiff;
    int     _samplerNorm;

    int     _spotPos;       // uniforms dealing with spotlights
    int     _spotDir;
    int     _spotIntensity;
    int     _spotAtt;
    int     _spotCutt;
    int     _spotExp;
    int     _camPos;
    int     _windowSize;
    int     _specularC;
    int     _shininess;

    
};

//! the final quad renderer for deferred mode
class QuadRenderer : public RenderPass {
  public:
    QuadRenderer ();
    virtual ~QuadRenderer ();

    void Enable ();

    void Render (cs237::mat4f const &projMat, cs237::mat4f const &viewMat, const Instance *mesh,SpotLight light);

  protected:
    GLuint vaoId;
    int _texLoc;
};

//! An actual renderer can involve multiple passes (e.g., deferred rendering)
//
typedef std::vector<RenderPass *>	Renderer;


#endif // !_RENDER_HXX_
