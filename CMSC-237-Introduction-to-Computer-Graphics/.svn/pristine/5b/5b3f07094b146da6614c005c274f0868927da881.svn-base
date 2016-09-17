/*! \file view.hxx
 *
 * \brief type definitions for tracking the view state.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _VIEW_HXX_
#define _VIEW_HXX_

#include "cs237.hxx"
#include "scene.hxx"
#include "render.hxx"
#include "mesh.hxx"
#include "obj.hxx"

enum RenderMode {
    WIREFRAME = 0,      //!< render scene as a wireframe
    FLAT_SHADING,       //!< render the scene using flat shading
    LIGHTING = 2,           //!< render the scene using the directional lighting
    TEXTURING = 3,          //!< render the scene using directional lighting and textures
    NUM_RENDER_MODES    //!< == to the number of rendering modes
};

/*! \brief The current state of the view */
struct View {
    GLFWwindow		*win;		//!< the application window
    bool		shouldExit;	//!< set to true when the application should exit
    bool		needsRedraw;	//!< set to true when the display is out of date
  // Camera state
    cs237::vec3f	camPos;		//!< camera position in world space
    cs237::vec3f	camAt;		//!< camera look-at point in world space
    cs237::vec3f	camUp;		//!< camera up vector in world space
    float		fov;		//!< Horizontal field of view specified by scene
  // view info 
    cs237::mat4f	modelViewMat;	//!< the current model-view matrix
    cs237::mat4f	projectionMat;	//!< the current projection matrix
    int			wid, ht;	//!< window dimensions
    bool		isVis;		//!< true, when the window is visible
  // rendering state
    RenderMode		mode;		//!< the current rendering mode
    Renderer		*wfRender;	//!< the renderer for wireframe
    Renderer		*fsRender;	//!< the renderer for flat shading
    Renderer    *ltRender;  // the lighting renderer
    Renderer    *txRender;   // the texturing renderer
  // scene info
/** YOUR CODE HERE **/
    std::vector<Mesh> meshes;

    View (Scene const &scene, GLFWwindow *win);

  /*! \brief initialize the renderers by loading and compiling their shaders.
   *  Note that this function needs to be called after the current
   *  OpenGL context has been set.
   */
    void InitRenderers ();

  /*! \brief initialize the projection matrix based on the current camera state. */
    void InitProjMatrix ();

  /*! \brief initialize the model-view matrix based on the current camera state. */
    void InitModelViewMatrix ();

  /*! \brief render the state of the scene
   */
    void Render ();

  /*  Get the mesh data from the scene and load into mesh
   */
    std::vector<Mesh> getMesh (Scene const &scene);

};

#endif /* !_VIEW_HXX_ */
