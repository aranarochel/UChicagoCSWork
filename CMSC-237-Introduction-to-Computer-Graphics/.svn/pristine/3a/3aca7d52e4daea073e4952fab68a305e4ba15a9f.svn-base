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

#define NEAR_Z  0.2f 
#define FAR_Z   500.0f

#define FOVY		65.0f	/* field-of-view in Y-dimension */

#define RED_COLOR  cs237::color4f(1.0f, 0.0f, 0.0f, 1.0f)   /*!< Red color */ 
#define GREEN_COLOR cs237::color4f(0.0f, 1.0f, 0.0f, 1.0f)    /*!< Green color */
#define YELLOW_COLOR  cs237::color4f(1.0f, 1.0f, 0.0f, 1.0f)    /*!< Yellow color */
#define BLUE_COLOR cs237::color4f(0.0f, 0.0f, 1.0f, 1.0f)   /*!< Blue color */
#define SKY_BLUE_COLOR cs237::color4f(0.0f, 1.0f, 1.0f, 1.0f)   /*!< Sky Blue color */
#define PINK_COLOR cs237::color4f(1.0f, 0.0f, 0.5f, 1.0f)   /*!< Pink */ 

struct Wall { 
  cs237::color4f color; 
  cs237::vec3f norm;
  cs237::vec3f corners[4]; 
  cs237::vec2f texCoords[4]; 
}; 

/* The vertices for the triangle */ 
    /*! the five visible walls of the box */  
    Wall  walls[6] = {
    { /* left */
        RED_COLOR,    // color
        cs237::vec3f{  1.0f,  0.0f,  0.0f },  // norm
        {cs237::vec3f{ -1.0f,  1.0f, -1.0f },  // corner[0]
        cs237::vec3f{ -1.0f,  1.0f,  1.0f },
        cs237::vec3f{ -1.0f, -1.0f,  1.0f },
        cs237::vec3f{ -1.0f, -1.0f, -1.0f }},
        {cs237::vec2f{  0.333f,  0.666f},  // texture coordinates 
        cs237::vec2f{  0.666f,  0.666f},
        cs237::vec2f{  0.666f,  0.333f},
        cs237::vec2f{  0.334f,  0.333f}}

    },
    { /* right */
        GREEN_COLOR,
        cs237::vec3f{ -1.0f,  0.0f,  0.0f },
        {cs237::vec3f{  1.0f,  1.0f,  1.0f },
        cs237::vec3f{  1.0f,  1.0f, -1.0f },
        cs237::vec3f{  1.0f, -1.0f, -1.0f },
        cs237::vec3f{  1.0f, -1.0f,  1.0f }}, 
        {cs237::vec2f{  0.666f,  0.666f}, //texture coordinates 
        cs237::vec2f{  1.0f,  0.666f},
        cs237::vec2f{  1.0f,  0.333f},
        cs237::vec2f{  0.666f,  0.333f}}

    },
    { /* top */
        YELLOW_COLOR,
        cs237::vec3f{  0.0f, -1.0f,  0.0f },
        {cs237::vec3f{ -1.0f,  1.0f, -1.0f },
        cs237::vec3f{  1.0f,  1.0f, -1.0f },
        cs237::vec3f{  1.0f,  1.0f,  1.0f },
        cs237::vec3f{ -1.0f,  1.0f,  1.0f }}, 
        {cs237::vec2f{  0.333f,  0.333f}, 
        cs237::vec2f{  0.666f,  0.333f},
        cs237::vec2f{  0.666f,  0.0f},
          cs237::vec2f{  0.333f,  0.0f}}
    },
    { /* bottom */
        BLUE_COLOR,
        cs237::vec3f{  0.0f,  1.0f,  0.0f },
        {cs237::vec3f{ -1.0f, -1.0f,  1.0f },
        cs237::vec3f{  1.0f, -1.0f,  1.0f },
        cs237::vec3f{  1.0f, -1.0f, -1.0f },
        cs237::vec3f{ -1.0f, -1.0f, -1.0f }}, 
        {cs237::vec2f{  0.0f,  0.666f}, //texture coordinates 
        cs237::vec2f{  0.333f,  0.666f},
        cs237::vec2f{  0.333f,  0.333f}, 
        cs237::vec2f{  0.0f,  0.333f}}
    },
    { /* front */
        SKY_BLUE_COLOR, 
        cs237::vec3f{  0.0f,  0.0f,  1.0f },
        {cs237::vec3f{  1.0f,  1.0f, -1.0f },
        cs237::vec3f{ -1.0f,  1.0f, -1.0f },
        cs237::vec3f{ -1.0f, -1.0f, -1.0f },
        cs237::vec3f{  1.0f, -1.0f, -1.0f }}, 
        {cs237::vec2f{  0.0f,  0.333f}, // texture coordinates 
        cs237::vec2f{  0.333f,  0.333f},
        cs237::vec2f{  0.333f,  0.0f},
        cs237::vec2f{  0.0f,  0.0f}}

    },
    { /* back */
        PINK_COLOR,
        cs237::vec3f{  0.0f,  0.0f, -1.0f },
        {cs237::vec3f{ -1.0f,  1.0f,  1.0f },
        cs237::vec3f{  1.0f,  1.0f,  1.0f },
        cs237::vec3f{  1.0f, -1.0f,  1.0f },
        cs237::vec3f{ -1.0f, -1.0f,  1.0f }}, 
        { cs237::vec2f{  0.666f,  0.333f},
            cs237::vec2f{  1.0f,  0.333f},
            cs237::vec2f{  1.0f,  0.0f}, 
            cs237::vec2f{  0.666f,  0.0f}}

    }
      };

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
  this->camPos = cs237::vec3f(0.0, 0.0, 8.0);
  this->camAt = cs237::vec3f(0.0, 0.0, 0.0);
  this->camUp   = cs237::vec3f(0.0, 1.0, 0.0);

  /* HINT (Lab3): Setup the texture here. See Section on Generate/Setup a texture (Lab3) */
  cs237::image2d *image = new cs237::image2d("../data/cubetex.png");  // load the image
  cs237::texture2D *texture = new cs237::texture2D(GL_TEXTURE_2D, image); // setup the texture based on image
  texture->Bind();
  texture->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  texture->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);


  /* HINT (Lab3): convert the walls into to meshes */
  /* NOTE (Lab3): USE GL_TRIANGLE_FAN AND NOT GL_TRIANGLES as the mesh primitive */
  /* HINT (Lab3): Use this as the indices arrray : const uint32_t indices[4] = {0, 1, 2, 3}; */
  int i;
  const uint32_t indices[4] = {0,1,2,3};
  for(i = 0; i < 6; i++) {
    this->cube[i] = new Mesh(GL_TRIANGLE_FAN);
    cube[i]->LoadVertices(4,walls[i].corners);
    cube[i]->LoadIndices(4,indices);

    cs237::vec3f normals[4] = {
      walls[i].norm,
      walls[i].norm,
      walls[i].norm,
      walls[i].norm
    };

    cube[i]->LoadNormals(4,normals);
    cube[i]->LoadTexCoords(4,walls[i].texCoords);
    cube[i]->image = image;
    cube[i]->texture = texture;
    cube[i]->color = walls[i].color;
  }
  
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

  /* Draw the meshes in the scene */
  for(int i = 0; i < 6; i++) {
    fsRender->Render(this->modelViewMat, cube[i]);
  }

}

