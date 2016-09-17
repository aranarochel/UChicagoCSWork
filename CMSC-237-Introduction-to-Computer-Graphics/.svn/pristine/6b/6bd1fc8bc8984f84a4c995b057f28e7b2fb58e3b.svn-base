/*! \file view.hxx
 *
 * \brief type definitions for tracking the view state.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _VIEW_HXX_
#define _VIEW_HXX_

#include "cs237.hxx"
#include "scene.hxx"
#include "instance.hxx"
#include "render.hxx"
#include "mesh.hxx"
#include "ground.hxx"
#include "spot-light.hxx"
#include "g-buffer.hxx"

enum RenderMode {
    WIREFRAME = 0,	//!< render scene as a wireframe
    TEXTURING,		//!< render the scene using texture mapping and direct diffuse lighting
    DEFERRED,		//!< render the scene using deffered lighting
    NUM_RENDER_MODES	//!< == to the number of rendering modes
};

/*! \brief The current state of the view */
struct View {
    GLFWwindow		*win;		//!< the application window
    bool		shouldExit;	//!< set to true when the application should exit
    bool		needsRedraw;	//!< set to true when the display is out of date
  // Camera state
    cs237::vec3f	camPos;		//!< initial camera position in world space
    cs237::vec3f	camAt;		//!< camera look-at point in world space
    cs237::vec3f	camUp;		//!< initial camera up vector in world space
    float		fov;		//!< horizontal field of view specified by scene
    cs237::mat4f	camRot;		//!< cumlative camera rotation
    float		camOffset;	//!< offset for camera from initial distance to lookAt point.
    float		minOffset;	//!< minimum allowed offset
    float		maxOffset;	//!< maximum allowed offset
  // view info 
    cs237::mat4f	viewMat;	//!< the current view matrix
    cs237::mat4f	projMat;	//!< the current projection matrix
    int			wid, ht;	//!< window dimensions
    bool		isVis;		//!< true, when the window is visible
  // axes support
    class Axes		*axes;		//!< for drawing the world-space axes
    bool		drawAxes;	//!< draw the axes when true
  // light direction support
    class Line		*lightDir;	//!< for drawing the light direction
    bool		drawLightDir;	//!< draw the light direction when true

  // cone support
    bool    drawCone;

  // rendering state
    RenderMode		mode;		//!< the current rendering mode
    Renderer		renderers[NUM_RENDER_MODES];
					//!< the array of renderers indexed by mode
  // scene info
    std::vector<SpotLight> lights;	//!< the spot lights in the scene
    std::vector<MeshInfo> meshes;	//!< the meshes that represent the objects
    std::vector<Instance> objects;	//!< the objects in the scene
    std::vector<Instance> cones;
    Ground		*ground;	//!< ground mesh; nullptr if not present

    // G-Buffer
    GBuffer *gBuffer;   // the g buffer used for deffered rendering
    GBuffer *sBuffer;   // buffer which holds the specular lighting

    cs237::vec3f dir; 
    cs237::color3f i; 
    cs237::color3f amb;

  //! constructor for the view
    View (Scene const &scene, GLFWwindow *win);

  //! Bind the default frame buffer to the rendering context and set the viewport
    void BindFramebuffer ();

  //! initialize the G buffer for the view state
    void InitGBuffer();

  //! Make the view's window be the current OpenGL context
    void MakeCurrent () { glfwMakeContextCurrent (this->win); }

  /* rotate the camera around the look-at point by the given angle (in degrees) */
    void RotateLeft (float angle);

  /*! rotate the camera up by the given angle (in degrees) */
    void RotateUp (float angle);

  /*! move the camera towards the look-at point by the given distance */
    void Move (float dist);

  /*! \brief initialize the renderers by loading and compiling their shaders.
   *  Note that this function needs to be called after the current
   *  OpenGL context has been set.
   */
    void InitRenderers (Scene const &scene);

  /*! \brief initialize the projection matrix based on the current camera state. */
    void InitProjMatrix ();

  /*! \brief initialize the view matrix based on the current camera state. */
    void InitViewMatrix ();

  /*! \brief render the state of the scene
   */
    void Render ();

};

#endif /* !_VIEW_HXX_ */
