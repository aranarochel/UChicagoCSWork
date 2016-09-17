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
#include <map>

/* The path to the shaders; this is usually set from the compiler command-line.
 * but it defaults to a path relative to the build directory.
 */
#ifndef SHADER_DIR
#  define SHADER_DIR "../shaders/"
#endif

//! Load a shader from the file system and compile and link it.  We keep
//! a cache of shaders that have already been loaded to allow two renderers
//! to share the same shader program without compiling it twice.
//
static cs237::ShaderProgram *LoadShader (std::string const & shaderPrefix)
{
    static std::map<std::string, cs237::ShaderProgram *> Shaders;

    auto it = Shaders.find(shaderPrefix);
    if (it == Shaders.end()) {
      // load, compile, and link the shader program
  cs237::VertexShader vsh((shaderPrefix + ".vsh").c_str());
  cs237::FragmentShader fsh((shaderPrefix + ".fsh").c_str());
  cs237::ShaderProgram *shader = new cs237::ShaderProgram (vsh, fsh);
  if (shader == nullptr) {
      std::cerr << "Cannot build " << shaderPrefix << std::endl;
      exit (1);
  }
  Shaders.insert (std::pair<std::string, cs237::ShaderProgram *>(shaderPrefix, shader));
  return shader;
    }
    else {
  return it->second;
    }

}

/***** virtual class Renderer member functions *****/

Renderer::Renderer (cs237::ShaderProgram *sh)
    : _shader(sh)
{
  mPosition = sh->UniformLocation("mPosition");
  modelView = sh->UniformLocation("modelView"); 
  projection = sh->UniformLocation("projection");
}

Renderer::~Renderer ()
{ }


void Renderer::Render (cs237::mat4f const &modelViewMat,Mesh *mesh)
{
  
  cs237::setUniform(modelView,modelViewMat);
  cs237::setUniform(Vcolor,mesh->color);
  cs237::setUniform(mPosition,mesh->posn);

  mesh->Draw();
} 


/***** class WireframeRenderer member functions *****/

WireframeRenderer::WireframeRenderer ()
    : Renderer (LoadShader ("../shaders/shader"))
{ 
  Vcolor =  _shader->UniformLocation("Vcolor");
}

WireframeRenderer::~WireframeRenderer ()
{ }

void WireframeRenderer::Enable (cs237::mat4f const &projectionMat)
{
    /** YOUR CODE HERE **/
    _shader->Use();
    cs237::setUniform(projection,projectionMat);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    CS237_CHECK( glEnable (GL_DEPTH_TEST));

}


/***** class FlatShadingRenderer member functions *****/

FlatShadingRenderer::FlatShadingRenderer ()
    : Renderer (LoadShader ("../shaders/shader"))
{ 
  Vcolor =  _shader->UniformLocation("Vcolor");
}

FlatShadingRenderer::~FlatShadingRenderer ()
{ }

void FlatShadingRenderer::Enable (cs237::mat4f const &projectionMat)
{
    /** YOUR CODE HERE **/
    _shader->Use();
    cs237::setUniform(projection,projectionMat);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    CS237_CHECK( glEnable (GL_DEPTH_TEST));
}


/***** class LightingRenderer member functions *****/

LightingRenderer::LightingRenderer ()
    : Renderer (LoadShader ("../shaders/shaderL"))
{ 
  Vcolor =  _shader->UniformLocation("Vcolor");
  direction = _shader->UniformLocation("direction");
  ambient = _shader->UniformLocation("ambient");
  intensity = _shader->UniformLocation("intensity");
}

LightingRenderer::~LightingRenderer ()
{ }

void LightingRenderer::Enable (cs237::mat4f const &projectionMat)
{
    /** YOUR CODE HERE **/
    _shader->Use();
    cs237::setUniform(projection,projectionMat);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    CS237_CHECK( glEnable (GL_DEPTH_TEST));
}

void LightingRenderer::Render (cs237::mat4f const &modelViewMat,Mesh *mesh)
{

  // Texture rendering operations
  cs237::setUniform(modelView,modelViewMat);
  cs237::setUniform(mPosition,mesh->posn);
  cs237::setUniform(Vcolor,mesh->color);
  cs237::setUniform(direction,mesh->direc);
  cs237::setUniform(ambient,mesh->ambient);
  cs237::setUniform(intensity,mesh->intensity);

  mesh->Draw();
} 

/***** class LightingRenderer member functions *****/

TextureRenderer::TextureRenderer ()
    : Renderer (LoadShader ("../shaders/shaderT"))
{ 
  _samplerLoc = _shader->UniformLocation("sampler"); 
  direction = _shader->UniformLocation("direction");
  ambient = _shader->UniformLocation("ambient");
  intensity = _shader->UniformLocation("intensity");
}

TextureRenderer::~TextureRenderer ()
{ }

void TextureRenderer::Enable (cs237::mat4f const &projectionMat)
{
    /** YOUR CODE HERE **/
    _shader->Use();
    cs237::setUniform(projection,projectionMat);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    CS237_CHECK( glEnable (GL_DEPTH_TEST));
}


void TextureRenderer::Render (cs237::mat4f const &modelViewMat,Mesh *mesh)
{

  // Texture rendering operations
  CS237_CHECK(glActiveTexture(GL_TEXTURE0));  // activate first texture unit
  mesh->texture->Bind();

  cs237::setUniform(_samplerLoc, 0);
  cs237::setUniform(modelView,modelViewMat);
  cs237::setUniform(mPosition,mesh->posn);
  cs237::setUniform(direction,mesh->direc);
  cs237::setUniform(ambient,mesh->ambient);
  cs237::setUniform(intensity,mesh->intensity);

  mesh->Draw();
} 




