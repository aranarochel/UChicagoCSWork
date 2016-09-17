/*! \file view.c
 *
 * \brief This file defines the viewer operations.
 *
 * \author John Reppy
 * \author Lamont Samuels 
 */

/* CMSC23700 Lab 2 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr) & Lamont Samuels.
 * All rights reserved.
 */

#ifndef _VIEW_HXX_
#define _VIEW_HXX_

#include <vector>
#include <iostream>
#include "cs237.hxx"
#include "render.hxx"
#include "drawable.hxx"
#include "depth-buffer.hxx"


enum RenderMode {
    LIGHTING = 0,
    TEXTURING = 1
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
    float angle;    //!< angle use for rotating around the origin 


  // view info 
    cs237::mat4f	modelViewMat;	//!< the current model-view matrix
    cs237::mat4f	projectionMat;	//!< the current projection matrix
    int			wid, ht;	//!< window dimensions
    bool		isVis;		//!< true, when the window is visible

  // light matrices
    cs237::mat4f  modelViewMatLight; //!< the Light model-view matrix
    cs237::mat4f  projectionMatLight;  //!< the Light projection matrix

  // rendering state
    RenderMode    mode;   //!< the current rendering mode
    Renderer    *ltRender;  // the lighting renderer
    Renderer    *txRender;   // the texturing renderer
    Renderer    *dsRender;

  // depth buffer
    DepthBuffer   * depthBuffer;  //!< the depth buffer used for shadow mapping 

  // scene info
    std::vector<Drawable *> drawables;	//!< the vector that contains the collection of drawables

    View (GLFWwindow *win, int wid, int ht); 


  //! \brief bind the framebuffer back to the screen. 
   void BindFrameBuffer(); 

  //! \brief initializes the depth buffer for the view state
    void InitDepthBuffer(); 

  //! \brief initialize the light's model-view, projection matrices and the shadow matrixx for the view state.
    void InitLightMatrices(); 

  /*! \brief initialize the renderers by loading and compiling their shaders.
   *  Note that this function needs to be called after the current
   *  OpenGL context has been set.
   */
    void InitRenderers ();

  /*! \brief initialize the projection matrix based on the current camera state. */
    void InitProjMatrix ();

  /*! \brief initialize the model-view matrix based on the current camera state. */
    void InitModelViewMatrix ();

  /*! \brief moves the camera in a circular motion. \ */
    void MoveAround(); 

  /*! \brief render the state of the scene
   */
    void Render ();

};

#endif /* !_VIEW_HXX_ */
