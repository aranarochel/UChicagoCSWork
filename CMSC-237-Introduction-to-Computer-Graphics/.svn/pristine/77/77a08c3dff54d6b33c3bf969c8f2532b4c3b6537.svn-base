/*! \file render.cxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
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
  mPosition = sh->UniformLocation("mPosition");
  modelView = sh->UniformLocation("modelView"); 
  projection = sh->UniformLocation("projection");
  Vcolor =  sh->UniformLocation("Vcolor"); }

Renderer::~Renderer ()
{ }

void Renderer::Render (cs237::mat4f const &modelViewMat,Mesh *mesh)
{
    /**HINT: You should set the model-view matrix */ 
    /**HINT: You should set the uniform for the mesh's color **/
    /**HINT: Draw the mesh **/
    /** YOUR CODE HERE **/
  cs237::setUniform(modelView,modelViewMat);
  cs237::setUniform(Vcolor,mesh->color);
  cs237::setUniform(mPosition,mesh->posn);

  mesh->Draw();
}

/***** class WireframeRenderer member functions *****/

WireframeRenderer::WireframeRenderer ()
    : Renderer (LoadShader ("../shaders/shader" /** YOUR SHADER'S NAME **/))
{ }

WireframeRenderer::~WireframeRenderer ()
{ }

void WireframeRenderer::Enable (cs237::mat4f const &projectionMat)
{
    /** YOUR CODE HERE **/
    _shader->Use();
    cs237::setUniform(projection,projectionMat);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

}

/***** class FlatShadingRenderer member functions *****/

FlatShadingRenderer::FlatShadingRenderer ()
    : Renderer (LoadShader ("../shaders/shader" /** YOUR SHADER'S NAME **/))
{ }

FlatShadingRenderer::~FlatShadingRenderer ()
{ }

void FlatShadingRenderer::Enable (cs237::mat4f const &projectionMat)
{
    /** YOUR CODE HERE **/
    _shader->Use();
    cs237::setUniform(projection,projectionMat);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
