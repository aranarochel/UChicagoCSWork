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
#include "cube.hxx"
#include "height-field.hxx"


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
    this->dirLight = scene.LightDir();
    
    // retrieve mesh information
    this->drawables = View::getDrawable (scene);
}

/* InitDepthBuffer:
 *
 */
void View::InitDepthBuffer ()
{
  this->depthBuffer = new DepthBuffer(2048,1024);
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
 */
void View::InitLightMatrices ()
{
  // please ignore this hacky code
  this->modelViewMatLight = cs237::lookAt (
  cs237::vec3f((dirLight.x+fov),(dirLight.y+fov+20),-(dirLight.z+fov)),
  cs237::vec3f(0.0,0.0,0.0),
  cs237::vec3f(0.0,1.0,0.0));

  this->projectionMatLight = cs237::ortho (-21.0f,21.0f,-21.0f,21.0f,NEAR_Z,FAR_Z);
}


/* InitProjMatrix:
 *
 * initialize the projection matrix based on the view state.
 */
void View::InitProjMatrix ()
{
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
    this->ltRender = new LightingRenderer();
    this->txRender = new TextureRenderer();
    this->dsRender = new DepthRenderer();
    this->shRender = new ShadowRenderer();
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
  r->Enable (this->projectionMat,this->projectionMatLight);
  break;
      case FLAT_SHADING:
  r = this->fsRender;
  r->Enable (this->projectionMat,this->projectionMatLight);
  break;
      case LIGHTING:    
  r = this->ltRender;
  r->Enable (this->projectionMat,this->projectionMatLight);
  break;
      case TEXTURING:   
  r = this->txRender;
  r->Enable (this->projectionMat,this->projectionMatLight);
  break;
      case SHADOWS: { 
  // do the first pass rendering
  // set renderer to depth shader renderer
  this->depthBuffer->BindForWriting();
  r = this->dsRender;
  r->Enable (this->projectionMat,this->projectionMatLight);
  for (int j = 0; j < this->drawables.size(); j++) {
        r->Render(this->modelViewMat,this->modelViewMatLight,this->drawables[j]);   
    }
  }

  // set renderer to shadow renderer and enable
  r = this->shRender;
  r->Enable (this->projectionMat,this->projectionMatLight);
  break;
      case NUM_RENDER_MODES:  
  r = this->fsRender;
  break;
    }

  // bind to the frame buffer and read from the depth buffer
  BindFrameBuffer();
  this->depthBuffer->BindForReading(GL_TEXTURE1);

  // loop through meshes vector and render each one
  int i;
  for(i = 0; i < this->drawables.size(); i++)
  {
    r->Render(modelViewMat,modelViewMatLight,this->drawables[i]);
  }

}

/* getDrawable:
 *
 * gets the mesh data from the scene and loads it into the cube and puts it in a drawable vector
 */
std::vector<Drawable *> View::getDrawable(Scene const &scene)
{
  std::vector<Drawable *> drawable;
  std::vector<SceneObj>::const_iterator it;

  // lighting data
  cs237::vec3f dir = scene.LightDir();
  cs237::color3f ints = scene.LightIntensity();
  cs237::color3f amb = scene.AmbientLight();
  float shadowF = scene.ShadowFactor();

  for (it = scene.beginObjs(); it != scene.endObjs(); it++) {
    // get the scene object
    SceneObj obj = *it;

    // create the drawable cube
    Cube *c = new Cube();

    // create new mesh to fill data
    Mesh *mesh_new = new Mesh(GL_TRIANGLES);

    // set the mesh modelMat and lighting vectors and shadow factor
    c->modelMat = obj.toWorld;
    c->color = cs237::color4f(obj.color,1.0);
    c->direc = dir;
    c->intensity = ints;
    c->ambient = amb;
    c->shadowFactor = shadowF;

    //get the mesh data
    OBJ::Model const *model = scene.Model(obj.model);
    OBJ::Group const group = model->Group(0);
    OBJ::Material const material = model->Material(group.material);

    mesh_new->LoadVertices (group.nVerts, group.verts);
    mesh_new->LoadIndices (group.nIndices, group.indices);
    mesh_new->LoadNormals (group.nVerts, group.norms);
    mesh_new->LoadTexCoords (group.nVerts, group.txtCoords);


    // get the texture data
    cs237::image2d *image = scene.TextureByName(material.diffuseMap);
    cs237::texture2D *texture = new cs237::texture2D(GL_TEXTURE_2D, image);
    texture->Bind();
    texture->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    texture->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    c->image = image;
    c->texture = texture;

    // get the texture data for normal map
    cs237::image2d *image2 = scene.TextureByName(material.normalMap);
    cs237::texture2D *texture2 = new cs237::texture2D(GL_TEXTURE_2D, image2);
    texture2->Bind();
    texture2->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    texture2->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    c->normalImage = image2;
    c->normalTexture = texture2;

    c->mesh = mesh_new;

    drawable.push_back(c);
  }

    // retrieve floor mesh if any
  Floor *f;
  if(scene.Ground() != nullptr)
  {
    f = View::getFloor(scene.Ground());

    f->direc = dir;
    f->intensity = ints;
    f->ambient = amb;
    f->shadowFactor = shadowF;

    drawable.push_back(f);
  }
  

  return drawable;
} 


Floor * View::getFloor(const HeightField * ground)
{
  int vertPos = 0;

  // create the drawable floor
  Floor *f = new Floor();

  // create the new mesh to fill data
  Mesh *mesh_new = new Mesh(GL_TRIANGLES);

  // create vec3's which hold mesh data
  uint32_t ncols = ground->NumCols();
  uint32_t nrows = ground->NumRows();
  uint32_t nverts = ground->NumVerts();
  uint32_t nindices = 6 * (ncols-1) * (nrows-1);

  cs237::vec3f *verts = new cs237::vec3f[nverts];
  uint32_t *indices = new uint32_t[nindices];
  cs237::vec2f *tcoords = new cs237::vec2f[nverts];
  cs237::vec3f *norms = new cs237::vec3f[nverts];


  // iterate through height field map and populate arrays for mesh
  for(int i = 0; i < nrows; i++) {
    for(int j = 0; j < ncols; j++) {
      // get vertex at current position and place in vertex array
      cs237::vec3f vertex = ground->VertexAt(i,j);
      verts[vertPos] = vertex;
      // calculate texture coordinate
      cs237::vec2f texc = cs237::vec2f(((float)j)/(nrows-1), -((float)i)/(ncols-1));
      tcoords[vertPos] = texc;

      // compute vertex normals
      cs237::vec3f e_left;
      if(j > 0)
        e_left = ground->VertexAt(i,j-1) - vertex;
      else
        e_left = vertex - ground->VertexAt(i,j+1);

      cs237::vec3f e_up;
      if(i > 0)
        e_up = ground->VertexAt(i-1,j) - vertex;
      else
        e_up = vertex - ground->VertexAt(i+1,j);

      cs237::vec3f e_right;
      if(j < ncols-1)
        e_right = ground->VertexAt(i,j+1) - vertex;
      else
        e_right = vertex - ground->VertexAt(i,j-1);

      cs237::vec3f e_down;
      if(i < nrows-1)
        e_down = ground->VertexAt(i+1,j) - vertex;
      else
        e_down = vertex - ground->VertexAt(i-1,j);

            
      // compute face normals
      cs237::vec3f nAB = -cs237::vec3f(
          (e_left.y * e_up.z) - (e_left.z * e_up.y),
          (e_left.z * e_up.x) - (e_left.x * e_up.z),
          (e_left.x * e_up.y) - (e_left.y * e_up.x)
      );
      cs237::vec3f nCD = -cs237::vec3f(
          (e_right.y * e_down.z) - (e_right.z * e_down.y),
          (e_right.z * e_down.x) - (e_right.x * e_down.z),
          (e_right.x * e_down.y) - (e_right.y * e_down.x)
      );
      
      // average triangle normals and place in normal arrays
      norms[vertPos] = 0.5f*(nAB + nCD);
      vertPos++;
    }
  }

  // compute indices
  int indPos = 0;
    for(int x = 0; x < nrows-1; x++) {
        for(int y = 0; y < ncols-1; y++) {
            int ind = x * ncols + y;
            indices[indPos++] = (uint32_t)ind;
            indices[indPos++] = (uint32_t)(ind + 1);
            indices[indPos++] = (uint32_t)(ind + ncols);
            indices[indPos++] = (uint32_t)(ind + 1);
            indices[indPos++] = (uint32_t)(ind + 1 + ncols);
            indices[indPos++] = (uint32_t)(ind + ncols);
        }
    }

    // populate mesh data structure and the floor drawable
    mesh_new->LoadVertices(nverts, verts);
    mesh_new->LoadIndices(nindices, indices);
    mesh_new->LoadNormals(nverts, norms);
    mesh_new->LoadTexCoords(nverts, tcoords);

    f->color = cs237::color4f(ground->Color(),0.0f);
    f->modelMat = cs237::translate(cs237::vec3f(0.0,-1.0,0.0)); 
    f->mesh = mesh_new;

    // get the texture data
    cs237::image2d *image = ground->ColorMap();
    cs237::texture2D *texture = new cs237::texture2D(GL_TEXTURE_2D, image);
    texture->Bind();
    texture->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    texture->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    f->image = image;
    f->texture = texture;

    // get the texture data for normal map
    cs237::image2d *image2 = ground->NormalMap();
    cs237::texture2D *texture2 = new cs237::texture2D(GL_TEXTURE_2D, image2);
    texture2->Bind();
    texture2->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    texture2->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    f->normalImage = image2;
    f->normalTexture = texture2;

  return f;
}










