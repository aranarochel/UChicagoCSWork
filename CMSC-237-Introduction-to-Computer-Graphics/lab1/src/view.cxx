/*! \file view.cxx
 *
 * \author Lamont Samuels 
 */
#include "view.hxx" 

/* clip planes in world coordinates */
#define NEAR_Z		0.2f 
#define FAR_Z		100.0f
#define FOVY		65.0f	/* field-of-view in Y-dimension */

/*! \brief performs initializations needed before starting up the view*/ 
void View::Init(int w, int h){ 

	/* Initialize the camera vectors for the camera matrix */ 
    
    this->width =  w; this->camPos = cs237::vec3f(0.0, 0.0, -3.0);
    this->camDir = cs237::vec3f(0.0, 0.0, 1.0);
    this->camUp    = cs237::vec3f(0.0, 1.0, 0.0);
    this->height = h;
    this->shader = nullptr; 

}
//! \brief performs any initialization before using the view */  
void View::InitShaders() { 
	
	//#1 Compile and initialize the shaders; 
	  if (this->shader == nullptr) {
	    cs237::VertexShader vsh("../data/shader.vert");
	    cs237::FragmentShader fsh("../data/shader.frag");
	    this->shader = new cs237::ShaderProgram (vsh, fsh);
	    if (this->shader == nullptr)
	        exit (1);
	  }

	  //#2 Retrieve the uniform locations for the model and projection matrices 
	  cameraLoc = shader->UniformLocation("camera"); 
	  projectionLoc = shader->UniformLocation("projection"); 
}

//! \brief initialize the projection matrix based on the view state. */
void View::InitProjMatrix(){
	this->projectionMat = cs237::perspective (
		FOVY,
		(float)this->width / (float)this->height,NEAR_Z, FAR_Z); 
}

//! \brief initialize the model-view matrix based on the view state. */
void View::InitCamera(){    
    this->camera = cs237::lookAt(
	this->camPos,
	this->camPos + this->camDir,
	this->camUp);
}   
//! \brief init triangle mesh */
void View::InitTriangle() {
	
	this->tri = new struct Triangle();
	this->tri->Init();

	this->tri->posLoc = shader->AttributeLocation("position");
	this->tri->colorLoc = shader->AttributeLocation("color");

}
void View::Render() { 

	//Declare a black color 
   const GLfloat color[] = { 0.0f, 0.0f , 0.0f, 1.0f}; 
              
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Set the background to black 
   glClearBufferfv(GL_COLOR,0,color); 

    glEnable(GL_DEPTH_TEST);

    //Begin using the shader program 
    shader->Use();

    //Pass in the camera and projection matrices 
    cs237::setUniform(cameraLoc,camera);  
    cs237::setUniform(projectionLoc,projectionMat);
 
    //Render the triangle. 
    tri->Render(); 
}
