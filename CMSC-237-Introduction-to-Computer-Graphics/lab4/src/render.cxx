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
#include "floor.hxx"
#include <map> 
/* The path to the shaders; this is usually set from the compiler command-line.
 * but it defaults to a path relative to the build directory.
 */
#ifndef SHADER_DIR
#  define SHADER_DIR "../shaders/"
#endif

/**** define lighting values ****/
cs237::vec3f dir = cs237::vec3f(-1.0f,1.0f,0.25f);
cs237::color3f ints = cs237::color3f(0.8f,0.8f,0.8f);
cs237::color3f amb = cs237::color3f(0.8f,0.8f,0.8f);


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
    : _shader(sh),
	_mvLoc(sh->UniformLocation ("modelView")),
  _projLoc(sh->UniformLocation ("projection"))
{ }

Renderer::~Renderer ()
{ }


/***** class LightingRenderer member functions *****/

LightingRenderer::LightingRenderer ()
    : Renderer (LoadShader ("../shaders/shaderL"))
{ 
  Vcolor =  _shader->UniformLocation("Vcolor");
  direction = _shader->UniformLocation("direction");
  ambient = _shader->UniformLocation("ambient");
  intensity = _shader->UniformLocation("intensity");
  _samplerLocShadow = _shader->UniformLocation("samplerShadow");
  _mvLocLight = _shader->UniformLocation("modelViewLight");
  _projLocLight = _shader->UniformLocation("projectionLight");
}

LightingRenderer::~LightingRenderer ()
{ }

void LightingRenderer::Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight)
{
    /** YOUR CODE HERE **/
    _shader->Use();
    cs237::setUniform(_projLoc,projectionMat);
    cs237::setUniform(_projLocLight,projectionMatLight);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    CS237_CHECK( glEnable (GL_DEPTH_TEST));
}

void LightingRenderer::Render (cs237::mat4f const &modelViewMat,cs237::mat4f const &modelViewMatLight,Drawable *drawable)
{

  // Texture rendering operations
  cs237::setUniform(_samplerLocShadow, 1); 

  cs237::setUniform(_mvLoc,modelViewMat * drawable->modelMat);
  cs237::setUniform(_mvLocLight,modelViewMatLight * drawable->modelMat);
  cs237::setUniform(Vcolor,drawable->color);
  cs237::setUniform(direction,dir);
  cs237::setUniform(ambient,amb);
  cs237::setUniform(intensity,ints);

  drawable->Draw();
} 

/***** class TextureRenderer member functions *****/

TextureRenderer::TextureRenderer ()
    : Renderer (LoadShader ("../shaders/shaderT"))
{ 
  _samplerLoc = _shader->UniformLocation("sampler"); 
  direction = _shader->UniformLocation("direction");
  ambient = _shader->UniformLocation("ambient");
  intensity = _shader->UniformLocation("intensity");
  _samplerLocShadow = _shader->UniformLocation("samplerShadow");
  _mvLocLight = _shader->UniformLocation("modelViewLight");
  _projLocLight = _shader->UniformLocation("projectionLight");
}

TextureRenderer::~TextureRenderer ()
{ }

void TextureRenderer::Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight)
{
    /** YOUR CODE HERE **/
    _shader->Use();
    cs237::setUniform(_projLoc,projectionMat);
    cs237::setUniform(_projLocLight,projectionMatLight);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    CS237_CHECK( glEnable (GL_DEPTH_TEST));
}


void TextureRenderer::Render (cs237::mat4f const &modelViewMat,cs237::mat4f const &modelViewMatLight, Drawable *drawable)
{

  // Texture rendering operations
  CS237_CHECK(glActiveTexture(GL_TEXTURE0));  // activate first texture unit
  drawable->texture->Bind();
  cs237::setUniform(_samplerLocShadow, 1); 

  cs237::setUniform(_samplerLoc, 0);
  cs237::setUniform(_mvLoc,modelViewMat * drawable->modelMat);
  cs237::setUniform(_mvLocLight,modelViewMatLight * drawable->modelMat);
  cs237::setUniform(direction,dir);
  cs237::setUniform(ambient,amb);
  cs237::setUniform(intensity,ints);

  drawable->Draw();
} 


/***** class DepthRenderer member functions *****/

DepthRenderer::DepthRenderer ()
    : Renderer (LoadShader ("../shaders/shaderDS"))
{ }

DepthRenderer::~DepthRenderer ()
{ }

void DepthRenderer::Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight )
{
    /** YOUR CODE HERE **/
    _shader->Use();
    cs237::setUniform(_projLoc,projectionMatLight);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    static const GLfloat ones[] = { 1.0f };
    glDepthMask (GL_TRUE);
    CS237_CHECK(glEnable (GL_DEPTH_TEST));
    CS237_CHECK( glClearBufferfv(GL_DEPTH, 0, ones) );
}


void DepthRenderer::Render (cs237::mat4f const &modelViewMat,cs237::mat4f const &modelViewMatLight,Drawable *drawable)
{
  cs237::setUniform(_mvLoc,modelViewMatLight * drawable->modelMat);
  drawable->Draw();
} 





