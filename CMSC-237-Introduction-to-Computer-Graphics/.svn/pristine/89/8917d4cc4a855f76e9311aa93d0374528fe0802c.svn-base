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

/* clip planes in world coordinates */
//#define NEAR_Z		-1.1f
//#define FAR_Z		1.1f

#define NEAR_Z    0.2f 
#define FAR_Z   50.0f

#define FOVY		65.0f	/* field-of-view in Y-dimension */

#define YELLOW_COLOR  cs237::color4f(1.0f, 1.0f, 0.0f, 1.0f);    /*!< Yellow color */

/* The vertices for the cube */ 
static cs237::vec3f cubeVertices[8]= {cs237::vec3f(-1.0f,  -1.0f,  1.0f), //0
                   cs237::vec3f (-1.0f,  1.0f,  1.0f), //1
                   cs237::vec3f ( 1.0f,  1.0f,  1.0f), //2
                   cs237::vec3f( 1.0f,  -1.0f,  1.0f), //3
                   cs237::vec3f (-1.0f,  -1.0f, -1.0f), //4
                   cs237::vec3f (-1.0f,  1.0f, -1.0f), //5
                   cs237::vec3f ( 1.0f,  1.0f, -1.0f), //6
                   cs237::vec3f ( 1.0f,  -1.0f, -1.0f)}; //7 

/* the indices that allow us to create the cube. */ 
static const uint32_t cubeIndices[36] = {
    0,2,1,  0,3,2,
    4,3,0,  4,7,3,
    4,1,5,  4,0,1,
    3,6,2,  3,7,6,
    1,6,5,  1,2,6,
    7,5,6,  7,4,5
  };  

static const cs237::color4f cubeColor = YELLOW_COLOR; 

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

  /* HINT: initialize the camera */
  /* HINT: Try setting your camera's position to vec3(0.0,0.0,8.0); */ 
  /* HINT: Try setting your camera's look-at to vec3(0.0,0.0,0.0); looking at the origin */
  /* HINT: As always, set your camera-up to vec3(0.0,1.0,0.0); */ 
  /** YOUR CODE HERE **/

  this->camPos = cs237::vec3f(0.0, 0.0, 8.0);
  this->camAt = cs237::vec3f(0.0,0.0,0.0);
  this->camUp = cs237::vec3f(0.0,1.0,0.0);

  /* HINT:Setup cube mesh */
  /* HINT: Primitive should be GL_TRIANGLES */ 
  /* HINT: Load the vertices and indices using the global variables: cubeVertices, cubeIndices, above */ 
  /* HINT: You may also want to assign the color to the mesh at this point */ 
  /** YOUR CODE HERE **/

  this->cube = new Mesh(GL_TRIANGLES);
  cube->LoadVertices (8, cubeVertices);
  cube->LoadIndices (36, cubeIndices);
  cube->color = cubeColor;
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

/* InitModelViewMatrix:
 *
 * initialize the model-view matrix based on the view state.
 */
void View::InitModelViewMatrix ()
{
  this->modelViewMat = cs237::lookAt (
  this->camPos,
  this->camAt,
  this->camUp);

}

/* InitRenderers:
 */
void View::InitRenderers ()
{
    this->fsRender = new FlatShadingRenderer();
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

  /* Enable the renderer */ 
  fsRender->Enable (this->projectionMat);

  /* HINT: Draw the objects in the scene */
  /** YOUR CODE HERE **/
  fsRender->Render(modelViewMat,cube);
}

