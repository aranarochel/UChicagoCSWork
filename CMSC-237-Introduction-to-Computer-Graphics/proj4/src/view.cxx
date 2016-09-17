/*! \file view.c
 *
 * \brief This file defines the viewer operations.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "view.hxx"
#include "axes.hxx"
#include "cone-mesh.hxx"

/* clip planes in world coordinates */
#define NEAR_Z		0.5f
#define FAR_Z		1000.0f
#define FOVY		65.0f	/* field-of-view in Y-dimension */

#define MIN_DIST	5.0f
#define MAX_DIST	200.0f

/* View initialization.
 */
View::View (Scene const &scene, GLFWwindow *win)
{
  // link the window and the view together
    glfwSetWindowUserPointer (win, this);
    this->win = win;

  /* view info */
    this->wid		= scene.Width();
    this->ht		= scene.Height();
    this->isVis		= GL_TRUE;
    this->shouldExit	= false;
    this->needsRedraw	= true;

  /* initialize the camera */
    this->camPos	= scene.CameraPos();
    this->camAt		= scene.CameraLookAt();
    this->camUp		= scene.CameraUp();
    this->fov		= scene.HorizontalFOV();

  /* initialzize the current camera transform state */
    this->camRot	= cs237::mat4f(1.0f);
    this->camOffset	= 0;
    float dist		= distance(this->camPos, this->camAt);
    this->minOffset	= cs237::min(0.0f, MIN_DIST - dist);
    this->maxOffset	= cs237::max(0.0f, MAX_DIST - dist);

  /* initialize the axes */
    this->axes		= new Axes(5.0f);
    this->drawAxes	= false;

  /* initialize the light direction line */
    this->lightDir	= new Line(-scene.LightDir(), cs237::color3f(1,1,0), 10.0f);
    this->drawLightDir	= false;


  /* light variables */
	this->dir = scene.LightDir();
	this->i = scene.LightIntensity();
	this->amb = scene.AmbientLight();

  /* initialize the cone bool */
    this->drawCone = false;

  /* initialize the rendering state */
    this->mode		= WIREFRAME;

  // initialize the spot lights
    {
	int i = 0;
	this->lights.resize(scene.NumLights());
	for (auto it = scene.beginLights();  it != scene.endLights();  it++, i++) {
	    this->lights[i].name	= it->name;
	    this->lights[i].pos		= it->pos;
	    this->lights[i].dir		= it->dir;
	    this->lights[i].cutoff	= it->cutoff;
	    this->lights[i].exponent	= it->exponent;
	    this->lights[i].intensity	= it->intensity;
	    for (int j = 0;  j < 3;  j++)
		this->lights[i].atten[j] = it->atten[j];
	    this->lights[i].Init();
	}
	std::cerr << i << " lights" << std::endl;
    }

   // intitalize the cones
    for (auto it = this->lights.begin();  it != this->lights.end();  it++) {
				SpotLight sl = *it;
				Instance *cone = MakeCone(sl.pos, sl.dir, sl.cutoff, sl.maxDist);
				this->cones.push_back(*cone);
		    } 
  
  /* convert the scene objects to instances that will serve as templates for the objects in the scene */
    std::vector<GObject> templates;
    templates.reserve (scene.NumModels());
    {
	int i = 0;
	std::map<std::string, cs237::texture2D *> texMap;
	for (auto it = scene.beginModels();  it != scene.endModels();  it++) {
	    const OBJ::Model *model = (*it);
	    GObject gobj;
	    gobj.reserve (model->NumGroups());
	    for (auto git = model->beginGroups();  git != model->endGroups();  git++) {
		cs237::texture2D *cmap, *smap, *nmap;
	      // get the group's material
		const OBJ::Material *material = &(model->Material(git->material));
	      // find the groups's diffuse texture
		std::string texName = material->diffuseMap;
		if (texName.empty()) {
		    cmap = nullptr;
		}
		else {
		    auto imgIt = texMap.find(texName);
		    if (imgIt != texMap.end()) {
			cmap = imgIt->second;
		    }
		    else {
			cs237::image2d *img = scene.TextureByName(texName);
			if (img == nullptr) {
			    std::cerr << "Unable to find diffuse texture \"" << texName << "\"" << std::endl;
			    exit (1);
			}
			cmap = new cs237::texture2D(GL_TEXTURE_2D, img);
			cmap->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			cmap->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			texMap.insert(std::pair<std::string, cs237::texture2D *>(texName, cmap));
		    }
		}
	      // find the groups's specular texture
		std::string smapName = material->specularMap;
		if (smapName.empty()) {
		    smap = nullptr;
		}
		else {
		    auto imgIt = texMap.find(texName);
		    if (imgIt != texMap.end()) {
			cmap = imgIt->second;
		    }
		    else {
			cs237::image2d *img = scene.TextureByName(texName);
			if (img == nullptr) {
			    std::cerr << "Unable to find diffuse texture \"" << texName << "\"" << std::endl;
			    exit (1);
			}
			cmap = new cs237::texture2D(GL_TEXTURE_2D, img);
			cmap->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			cmap->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			texMap.insert(std::pair<std::string, cs237::texture2D *>(texName, cmap));
		    }
		}
	      // find the groups's normal map
		std::string nmapName = model->Material(git->material).normalMap;
		if (nmapName.empty()) {
		    nmap = nullptr;
		}
		else {
		    auto imgIt = texMap.find(texName);
		    if (imgIt != texMap.end()) {
			nmap = imgIt->second;
		    }
		    else {
			cs237::image2d *img = scene.TextureByName(texName);
			if (img == nullptr) {
			    std::cerr << "Unable to find bump texture \"" << texName << "\"" << std::endl;
			    exit (1);
			}
			nmap = new cs237::texture2D(GL_TEXTURE_2D, img);
			nmap->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			nmap->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			texMap.insert(std::pair<std::string, cs237::texture2D *>(texName, nmap));
		    }
		}
	      // add mesh to gobj
		MeshInfo *mesh = new MeshInfo(
		    GL_TRIANGLES, *git, material->diffuse,
		    material->specular, material->shininess,
		    cmap, nmap);
		gobj.push_back(mesh);
	    }
	    templates.push_back(gobj);
	}
	std::cerr << i << " models" << std::endl;
    }

  /* initialize the object instances in the view */
    {
	int i = 0;
	this->objects.resize (scene.NumObjects());
	for (auto it = scene.beginObjs();  it != scene.endObjs();  it++, i++) {
	    this->objects[i].meshes = templates[it->model];
	    this->objects[i].toWorld = it->toWorld;
	    this->objects[i].normToWorld = it->toWorld.normalMatrix();
	  // the mapping for normals from world space to object space is (M^{-1})^{T}, where
	  // M is the mapping for tangent vectors, which is = toWorld^{-1}.  Therefore
	  // toWorld^{T} is the mapping from world space to object space
	    this->objects[i].normFromWorld = cs237::mat3x3f(it->toWorld).transpose();
	    this->objects[i].color = it->color;
	  // compute the world-space bounding box around the object
	    cs237::AABBf modelBB = scene.Model(it->model)->BBox();
	    this->objects[i].bbox.clear();
	    for (int j = 0;   j < 8;  j++) {
		cs237::vec3f pt = cs237::vec3f(it->toWorld * cs237::vec4f(modelBB.corner(j), 1));
		this->objects[i].bbox.addPt(pt);
	    }
	}
	std::cerr << i << " objects" << std::endl;
    }

  // initialize the ground (if present)
    if (scene.Ground() != nullptr) {
	this->ground = new Ground(scene.Ground());
        std::cerr << "ground bbox: " << this->ground->BBox() << std::endl;
    }
    else {
	this->ground = nullptr;
    }

}


/* Initialize the G buffer
 *
 */
void View::InitGBuffer()
{
	this->gBuffer = new GBuffer(this->wid, this->ht);
	this->sBuffer = new GBuffer(this->wid, this->ht);
	BindFramebuffer();
}

/* BindFramebuffer:
 *
 * initialize the framebuffer back to the screen. 
 */
void View::BindFramebuffer()
{ 
    int fwid, fht; 
    CS237_CHECK (glBindFramebuffer (GL_FRAMEBUFFER, 0) );
    CS237_CHECK (glfwGetFramebufferSize (this->win, &fwid, &fht) );
    CS237_CHECK (glViewport (0, 0, fwid, fht) );

}

/* rotate the camera around the look-at point by the given angle (in degrees)
 */
void View::RotateLeft (float angle)
{
    this->camRot = cs237::rotateY(-angle) * this->camRot;
}

/* rotate the camera up by the given angle (in degrees)
 */
void View::RotateUp (float angle)
{
    this->camRot = cs237::rotateX(angle) * this->camRot;
}

/* move the camera towards the look-at point by the given distance
 */
void View::Move (float dist)
{
    if (dist < 0.0f) {  // move away from camera
	this->camOffset = cs237::max(this->minOffset, this->camOffset - dist);
    }
    else {  // move toward camera
	this->camOffset = cs237::min(this->maxOffset, this->camOffset - dist);
    }
}

/* InitProjMatrix:
 *
 * initialize the projection matrix based on the view state.
 */
void View::InitProjMatrix ()
{
  // compute vertical field of view as per 5.3.1 of M3D
    float eInv = tan (cs237::radians(this->fov) * 0.5f);
    float a = float(this->ht) / float(this->wid);
    float beta = cs237::degrees(2.0 * atan (eInv * a));

    this->projMat = cs237::perspective (
	beta,
	1.0f / a,
	NEAR_Z,
	FAR_Z);

}

/* InitViewMatrix:
 *
 * initialize the model-view matrix based on the view state.
 */
void View::InitViewMatrix ()
{
    cs237::mat4f mvMat = cs237::lookAt (this->camPos, this->camAt, this->camUp);
  // apply rotation followed by translation
    this->viewMat = 
	mvMat * cs237::translate(cs237::vec3f(0.0f, 0.0f, this->camOffset)) * this->camRot;

}

/* InitRenderers:
 */
void View::InitRenderers (Scene const &scene)
{
    this->renderers[WIREFRAME].push_back(new WireframeRenderer());
    this->renderers[TEXTURING].push_back(new TexturingRenderer(
	scene.LightDir(), scene.LightIntensity(), scene.AmbientLight()));
/* TODO: add the proper rendering passes for deferred rendering */
    this->renderers[DEFERRED].push_back(new GBufferRenderer());

}

/* Render:
 *
 * render the current state of the animation
 */
void View::Render ()
{
  /* clear the screen */
    glClearColor (0.0f, 0.0f, 0.0f, 0.0f);	// clear the surface
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->InitViewMatrix();

    Renderer passes;

    if (this->mode == DEFERRED) {
	/* YOUR CODE HERE */

    	// bind the g buffer for writing
    	this->gBuffer->BindForWriting();
    	//glDepthMask(GL_TRUE);
    	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	//glEnable(GL_DEPTH_TEST);
    	//glDisable(GL_BLEND);


    	// geometry pass
		passes = this->renderers[this->mode];
		for (auto it = passes.begin(); it != passes.end(); it++) {
		    RenderPass *r = *it;
		    r->Enable ();

		  // Draw the ground (if present)
		    if (this->ground != nullptr) {
				r->Render (this->projMat, this->viewMat, this->ground->Instance(),this->lights[0]);
		    }

		  /* Draw the objects in the scene */
		    for (auto it = this->objects.begin();  it != this->objects.end();  it++) {
				r->Render (this->projMat, this->viewMat, &(*it),this->lights[0]);
		    }
		}
	

		// DIRECTIONAL LIGHT PASS
		this->sBuffer->BindForWriting();
    	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	this->gBuffer->BindForReading(GL_TEXTURE0,0);
  		this->gBuffer->BindForReading(GL_TEXTURE1,1);
  		this->gBuffer->BindForReading(GL_TEXTURE2,2);

  		static RenderPass *f = new DeferredRenderer(this->dir, this->i, this->amb);
		f->Enable();
	  // Draw the ground (if present)
	    if (this->ground != nullptr) {
	    	SpotLight spotlight = this->lights[0];
	    	spotlight.camPos = this->camPos;
	    	spotlight.windowSize = cs237::vec2f(this->wid,this->ht);
	    	spotlight.lightType = 1.0f;
			f->Render (this->projMat, this->viewMat, this->ground->Instance(),spotlight);
	    }

	  /* Draw the objects in the scene */
	    for (auto it = this->objects.begin();  it != this->objects.end();  it++) {
	    	SpotLight spotlight = this->lights[0];
	    	spotlight.camPos = this->camPos;
	    	spotlight.windowSize = cs237::vec2f(this->wid,this->ht);
	    	spotlight.lightType = 1.0f;
			f->Render (this->projMat, this->viewMat, &(*it),spotlight);
	    }
		
    	
		// LIGHTING PASSES
    	//glEnable(GL_CULL_FACE);
    	glDisable(GL_DEPTH_TEST);
    	//glDepthMask(GL_TRUE);
	    glDepthMask(GL_TRUE);
	    glEnable(GL_BLEND);
   		glBlendEquation(GL_FUNC_ADD);
   		glBlendFunc(GL_ONE, GL_ONE);
		static RenderPass *g = new DeferredRenderer(this->dir, this->i, this->amb);
		g->Enable();

		int l = 0;
	  /* Draw the objects in the scene */
	    for (auto it = this->cones.begin();  it != this->cones.end();  it++) {
	    	SpotLight spotLight = this->lights[l];
	    	spotLight.camPos = this->camPos;
	    	spotLight.windowSize = cs237::vec2f(this->wid,this->ht);
	    	spotLight.lightType = 2.0f;

			g->Render (this->projMat, this->viewMat, &(*it),spotLight);
			l++;
	    }

		// FINAL QUAD SCREEN RENDERING
		//Bind back to the screen's framebuffer 
		BindFramebuffer(); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->sBuffer->BindForReading(GL_TEXTURE3,3);
		static RenderPass *q = new QuadRenderer();
		q->Enable();
		q->Render (this->projMat, this->viewMat, this->ground->Instance(), this->lights[0]);

    }
    else { // forward rendering

    	BindFramebuffer();
		if (this->drawAxes) {
		    this->axes->Draw (this->projMat, this->viewMat);
		}

		if (this->drawLightDir) {
		    this->lightDir->Draw (this->projMat, this->viewMat);
		}

		passes = this->renderers[this->mode];
		for (auto it = passes.begin(); it != passes.end(); it++) {
		    RenderPass *r = *it;
		    r->Enable ();

		  // Draw the ground (if present)
		    if (this->ground != nullptr) {
				r->Render (this->projMat, this->viewMat, this->ground->Instance(), this->lights[0]);
		    }

		  // Draw the cones if toggled
		    if (this->drawCone) {
			    for (auto it = this->cones.begin();  it != this->cones.end();  it++) {
					r->Render (this->projMat, this->viewMat, &(*it),this->lights[0]);
			    }
			}

		  /* Draw the objects in the scene */
		    for (auto it = this->objects.begin();  it != this->objects.end();  it++) {
				r->Render (this->projMat, this->viewMat, &(*it),this->lights[0]);
		    }
		}
  	}

}

