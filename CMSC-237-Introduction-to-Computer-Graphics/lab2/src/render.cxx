/*! \file render.cxx
 *
 * \author John Reppy
 * \author Lamont Samuels
 */

/* CMSC23700 Lab2 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr) & Lamont Samuels
 * All rights reserved.
 */

#include "render.hxx"

/* The path to the shaders; this is usually set from the compiler command-line.
 * but it defaults to a path relative to the build directory.
 */
#ifndef SHADER_DIR
#  define SHADER_DIR "../shaders/"
#endif

// We only need to build the shader once, so cache it here
//
static cs237::ShaderProgram *LoadShader (std::string const & shaderPrefix)
{
    static cs237::ShaderProgram *Shader = nullptr;

    if (Shader == nullptr) {
      // load, compile, and link the shader program
	cs237::VertexShader vsh((shaderPrefix + ".vsh").c_str());
	cs237::FragmentShader fsh((shaderPrefix + ".fsh").c_str());
	Shader = new cs237::ShaderProgram (vsh, fsh);
	if (Shader == nullptr) {
	    std::cerr << "Cannot build " << shaderPrefix << std::endl;
	    exit (1);
	}
    }

    return Shader;
}

/***** virtual class Renderer member functions *****/

Renderer::Renderer (cs237::ShaderProgram *sh)
    : _shader(sh)
{ 
      /**HINT: You should get the uniform locations of model-view, projection, and color
       using sh->UniformLocation(...) */ 
      /** YOUR CODE HERE **/
  modelView = sh->UniformLocation("modelView"); 
  projection = sh->UniformLocation("projection");
  Vcolor =  sh->UniformLocation("Vcolor");
}

Renderer::~Renderer ()
{ }

void Renderer::Render (cs237::mat4f const &modelViewMat, Mesh *mesh)
{
    /**HINT: You should set the model-view matrix */ 
    /**HINT: You should set the uniform for the mesh's color **/
    /**HINT: Draw the mesh **/
    /** YOUR CODE HERE **/

  cs237::setUniform(modelView,modelViewMat);
  cs237::setUniform(Vcolor,mesh->color);
  mesh->Draw();  
}
/***** class FlatShadingRenderer member functions *****/
FlatShadingRenderer::FlatShadingRenderer ()
    : Renderer (LoadShader ("../shaders/shader"))
{ }

FlatShadingRenderer::~FlatShadingRenderer ()
{ }

void FlatShadingRenderer::Enable (cs237::mat4f const &projectionMat)
{
    /**HINT: You should enable the shader. */ 
    /**HINT: You should set the uniform for the projection matrix **/
    /** YOUR CODE HERE **/

    _shader->Use();
    cs237::setUniform(projection,projectionMat);
}
