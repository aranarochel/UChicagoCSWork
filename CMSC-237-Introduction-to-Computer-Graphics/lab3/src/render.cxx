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
      /**HINT (Lab3): Get the _samplerloc location */
      /** YOUR CODE HERE **/
  modelView = sh->UniformLocation("modelView"); 
  projection = sh->UniformLocation("projection");
  //Vcolor =  sh->UniformLocation("Vcolor");
  _samplerLoc = sh->UniformLocation("sampler");
}

Renderer::~Renderer ()
{ }

void Renderer::Render (cs237::mat4f const &modelViewMat, Mesh *mesh)
{
    /**HINT: You should set the model-view matrix */ 
    /**HINT (Lab3): Setup texture rendering code. See the section on Rendering a Texture (Lab3)  */

  // Texture rendering operations
  CS237_CHECK(glActiveTexture(GL_TEXTURE0));  // activate first texture unit
  mesh->texture->Bind();

  cs237::setUniform(_samplerLoc, 0);

    /**HINT: Draw the mesh **/
    /** YOUR CODE HERE **/
  cs237::setUniform(modelView,modelViewMat);
  //cs237::setUniform(Vcolor,mesh->color);    // not used in this lab
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
    /**HINT (Lab3): Make sure to enable depth testing **/
    /** YOUR CODE HERE **/
  _shader->Use();
  cs237::setUniform(projection,projectionMat);
  CS237_CHECK( glEnable (GL_DEPTH_TEST));
}
