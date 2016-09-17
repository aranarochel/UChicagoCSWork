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

#include "cs237.hxx"
#include "view.hxx"
#include "cube.hxx"
#include "floor.hxx"


/* clip planes in world coordinates */
#define NEAR_Z  0.2f 
#define FAR_Z   500.0f

#define FOVY		65.0f	/* field-of-view in Y-dimension */

//! the directional light's direction in world coordinates
static cs237::vec4f DirLight(-1.0, 1.0, 0.25, 0.0);

/* View initialization.
 */
View::View (GLFWwindow *win, int wid, int ht)
{
    // link the window and the view together
    glfwSetWindowUserPointer (win, this);
    this->win = win;

  /* view info */
    this->wid		= wid;
    this->ht		= ht;
    this->isVis		= GL_TRUE;
    this->shouldExit	= false;
    this->needsRedraw	= true;

  /* initialize the camera */
    this->angle = 0; 
    this->camPos = cs237::vec3f(0.0, 8.0, 10.0);
    this->camAt = cs237::vec3f(0.0, 0.0, 0.0);
    this->camUp   = cs237::vec3f(0.0, 1.0, 0.0);


  /* initialize the rendering state */
    this->mode = LIGHTING;

  /* initialize drawable objects */ 
    Cube * c = new Cube(); 
    Floor * f = new Floor(); 
    this->drawables.push_back(f); 
    this->drawables.push_back(c); 

}
/* InitProjMatrix:
 *
 * initialize the projection matrix based on the view state.
 */
void View::InitProjMatrix ()
{
  this->projectionMat = cs237::perspective (
	FOVY,
	(float)this->wid / (float)this->ht,
	NEAR_Z,
	FAR_Z);

}
/* InitDepthBuffer:
 *
 * Hint: Initialize the depth buffer for the view state (you can hard-code the width & height)
 * Hint: Make sure to bind the framebuffer to the screen. 
 */
void View::InitDepthBuffer ()
{
  this->depthBuffer = new DepthBuffer(1024,1024);
  BindFrameBuffer();
}
/* BindFrameBuffer:
 *
 * initialize the framebuffer back to the screen. 
 */
void View::BindFrameBuffer()
{ 
    int fwid, fht; 
    CS237_CHECK (glBindFramebuffer (GL_FRAMEBUFFER, 0) );
    CS237_CHECK (glfwGetFramebufferSize (this->win, &fwid, &fht) );
    CS237_CHECK (glViewport (0, 0, fwid, fht) );

}
/* InitLightMatrices:
 *
 * initialize the light's model-view, projection matrices and the shadow matrixx for the view state.
 *
 *
 * Hint: Make sure to use the cs237::lookAt (modelView) and cs237::ortho (projection)
 * Hint: The modelView should use the directional light position and point towards the origin. 
 * Hint: You want the projection to contain all the objects (cube and floor) in the scene. Think carefully 
 * about with left,right,bottom,near,far should be in order to get all objects. Usually you don't want it 
 * to be huge but compact as possible but don't worry about that for now. 
 * 
 */
void View::InitLightMatrices ()
{
  this->modelViewMatLight = cs237::lookAt (
  (cs237::vec3f)DirLight * 65.0f,
  cs237::vec3f(0.0,0.0,0.0),
  cs237::vec3f(0.0,1.0,0.0));

  this->projectionMatLight = cs237::ortho (-10.0f,10.0f,-10.0f,10.0f,1.0f,100.0f);



}

/* InitModelViewMatrix:
 *
 * initialize the model-view matrix based on the view state.
 */
void View::InitModelViewMatrix()
{

  this->modelViewMat = cs237::lookAt (
  this->camPos,
  this->camAt,
  this->camUp);

}
void View::MoveAround() { 

  float radius = 10.0;  
  float camX = radius * sin(cs237::radians(this->angle)); 
  float camZ = radius * cos(cs237::radians(this->angle)); 
  this->camPos = cs237::vec3f(camX, 8.0, camZ);

  InitModelViewMatrix(); 
}

/* InitRenderers:
 * 
 * Initialze the renderes 
 */
void View::InitRenderers ()
{
    this->ltRender = new LightingRenderer();
    this->txRender = new TextureRenderer();
    this->dsRender = new DepthRenderer();
}

/* Render:
 *
 * render the current state of the animation
 */
void View::Render ()
{
  /* clear the screen */
  glClearColor (0.2f, 0.2f, 0.4f, 1.0f);	// clear the surface
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Do depth buffer rendering - first pass rendering */
  //bind to the write depth-buffer
  this->depthBuffer->BindForWriting();

  // declare the renderer
  Renderer *r;
  r = this->dsRender;
  r->Enable(this->projectionMat,this->projectionMatLight);
  for (auto it = drawables.begin();  it != drawables.end();  it++) {
      
      Drawable * drawable = *it;
      r->Render(this->modelViewMat,this->modelViewMatLight,drawable);
      
  }


 /* Do second pass rendering */
  // bind the frame buffer back to the screen
  BindFrameBuffer();
  this->depthBuffer->BindForReading(GL_TEXTURE1);

  for (auto it = drawables.begin();  it != drawables.end();  it++) {
      
      Drawable * drawable = *it;

      if (drawable->shouldTexturize == true)
        r = this->txRender;
      else
        r = this->ltRender;

      r->Enable (this->projectionMat,this->projectionMatLight);

      r->Render(modelViewMat,modelViewMatLight,drawable);
  }
}









