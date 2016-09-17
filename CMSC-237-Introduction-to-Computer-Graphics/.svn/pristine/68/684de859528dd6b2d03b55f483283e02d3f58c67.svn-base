/*! \file view.c
 *
 * \brief This file defines the viewer operations.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Winter 2014)
 *
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "view.hxx"



#define NEAR_Z    0.2f 
#define FAR_Z   500.0f
//#define FOVY   65.0f

/* View initialization.
 */
View::View (Scene const &scene, GLFWwindow *win)
{
  // link the window and the view together
    glfwSetWindowUserPointer (win, this);
    this->win = win;

  /* view info */
    this->wid   = scene.Width();
    this->ht    = scene.Height();
    this->isVis   = GL_TRUE;
    this->shouldExit  = false;
    this->needsRedraw = true;

  /* initialize the camera */
    this->camPos  = scene.CameraPos();
    this->camAt   = scene.CameraLookAt();
    this->camUp   = scene.CameraUp();
    this->fov   = scene.HorizontalFOV();
    
  /* initialize the rendering state */
    this->mode    = WIREFRAME;
    
    // retrieve mesh information
    this->meshes = View::getMesh (scene);
    
  
}

/* InitProjMatrix:
 *
 * initialize the projection matrix based on the view state.
 */
void View::InitProjMatrix ()
{
    /** YOUR CODE HERE **/
  this->projectionMat = cs237::perspective (
  ((float)this->fov * ((float)this->ht / (float)this->wid)),
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
    /** YOUR CODE HERE **/
  this->modelViewMat = cs237::lookAt (
  this->camPos,
  this->camAt,
  this->camUp);
}

/* InitRenderers:
 */
void View::InitRenderers ()
{
    this->wfRender = new WireframeRenderer();
    this->fsRender = new FlatShadingRenderer();

}

/* Render:
 *
 * render the current state of the animation
 */
void View::Render ()
{
  /* clear the screen */
    glClearColor (0.2f, 0.2f, 0.4f, 1.0f);  // clear the surface
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Renderer *r;
    switch (this->mode) {
      case WIREFRAME:
  r = this->wfRender;
  break;
      case FLAT_SHADING:
  r = this->fsRender;
  break;
    }
    r->Enable (this->projectionMat);

  // loop through meshes vector and render each one
  int i;
  for(i = 0; i < this->meshes.size(); i++)
  {
    r->Render(modelViewMat,&(this->meshes[i]));
  }

}

/* getMesh:
 *
 * gets the mesh data from the scene and loads it into the cube
 */
std::vector<Mesh> View::getMesh(Scene const &scene)
{
  std::vector<Mesh> meshes;
  std::vector<SceneObj>::const_iterator it;

  for (it = scene.beginObjs(); it != scene.endObjs(); it++) {
    // get the scene object
    SceneObj obj = *it;

    // create new mesh to fill data
    Mesh *mesh_new = new Mesh(GL_TRIANGLES);

    //get the mesh data
    OBJ::Model const *model = scene.Model(obj.model);
    OBJ::Group const group = model->Group(0);

    mesh_new->LoadVertices (group.nVerts, group.verts);
    mesh_new->LoadIndices (group.nIndices, group.indices);

    // set the mesh position and color
    mesh_new->posn = obj.pos;
    mesh_new->color = cs237::color4f(obj.color,1.0);

    meshes.push_back(*mesh_new);
  }
  return meshes;
} 







