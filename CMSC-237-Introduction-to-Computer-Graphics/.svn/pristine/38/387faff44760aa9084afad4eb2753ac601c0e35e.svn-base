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
static cs237::ShaderProgram *LoadShader (std::string const & shaderPrefix, bool hasGeom = false)
{
    static std::map<std::string, cs237::ShaderProgram *> Shaders;

    auto it = Shaders.find(shaderPrefix);
    if (it == Shaders.end()) {
      // load, compile, and link the shader program
	cs237::VertexShader vsh((shaderPrefix + ".vsh").c_str());
	cs237::FragmentShader fsh((shaderPrefix + ".fsh").c_str());
	cs237::ShaderProgram *shader;
	if (hasGeom) {
	    cs237::GeometryShader gsh((shaderPrefix + ".gsh").c_str());
	    shader = new cs237::ShaderProgram (vsh, gsh, fsh);
	}
	else {
	    shader = new cs237::ShaderProgram (vsh, fsh);
	}
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
  modelView = sh->UniformLocation("modelView"); 
  projection = sh->UniformLocation("projection");
}

Renderer::~Renderer ()
{ }


void Renderer::Render (cs237::mat4f const &modelViewMat,cs237::mat4f const &modelViewMatLight,Drawable *drawable)
{
  cs237::setUniform(modelView,modelViewMat * drawable->modelMat);
  cs237::setUniform(Vcolor,drawable->color);

  drawable->Draw();
} 


/***** class WireframeRenderer member functions *****/

WireframeRenderer::WireframeRenderer ()
    : Renderer (LoadShader ("../shaders/shader"))
{ 
  Vcolor =  _shader->UniformLocation("Vcolor");
}

WireframeRenderer::~WireframeRenderer ()
{ }

void WireframeRenderer::Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight)
{
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

void FlatShadingRenderer::Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight)
{
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

void LightingRenderer::Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight)
{
    _shader->Use();
    cs237::setUniform(projection,projectionMat);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    CS237_CHECK( glEnable (GL_DEPTH_TEST));
}

void LightingRenderer::Render (cs237::mat4f const &modelViewMat,cs237::mat4f const &modelViewMatLight,Drawable *drawable)
{
  // Texture rendering operations
  cs237::setUniform(modelView,modelViewMat * drawable->modelMat);
  //cs237::setUniform(mPosition,mesh->posn);
  cs237::setUniform(Vcolor,drawable->color);
  cs237::setUniform(direction,drawable->direc);
  cs237::setUniform(ambient,drawable->ambient);
  cs237::setUniform(intensity,drawable->intensity);

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
}

TextureRenderer::~TextureRenderer ()
{ }

void TextureRenderer::Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight)
{
    _shader->Use();
    cs237::setUniform(projection,projectionMat);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    CS237_CHECK( glEnable (GL_DEPTH_TEST));
}


void TextureRenderer::Render (cs237::mat4f const &modelViewMat,cs237::mat4f const &modelViewMatLight,Drawable *drawable)
{
  // Texture rendering operations
  CS237_CHECK(glActiveTexture(GL_TEXTURE0));  // activate first texture unit
  drawable->texture->Bind();

  cs237::setUniform(_samplerLoc, 0);
  cs237::setUniform(modelView,modelViewMat * drawable->modelMat);
  cs237::setUniform(direction,drawable->direc);
  cs237::setUniform(ambient,drawable->ambient);
  cs237::setUniform(intensity,drawable->intensity);

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
    _shader->Use();
    cs237::setUniform(projection,projectionMatLight);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    static const GLfloat ones[] = { 1.0f };
    glDepthMask (GL_TRUE);
    CS237_CHECK(glEnable (GL_DEPTH_TEST));
    CS237_CHECK( glClearBufferfv(GL_DEPTH, 0, ones) );
}


void DepthRenderer::Render (cs237::mat4f const &modelViewMat,cs237::mat4f const &modelViewMatLight,Drawable *drawable)
{
  cs237::setUniform(modelView,modelViewMatLight * drawable->modelMat);
  drawable->Draw();
} 

/***** class ShadowRenderer member functions *****/

ShadowRenderer::ShadowRenderer ()
    : Renderer (LoadShader ("../shaders/shaderS"))
{ 
  _samplerLoc = _shader->UniformLocation("sampler"); 
  direction = _shader->UniformLocation("direction");
  ambient = _shader->UniformLocation("ambient");
  intensity = _shader->UniformLocation("intensity");
  shadow = _shader->UniformLocation("shadowF");
  _samplerLocShadow = _shader->UniformLocation("samplerShadow");
  modelViewLight = _shader->UniformLocation("modelViewLight");
  projectionLight = _shader->UniformLocation("projectionLight");
}

ShadowRenderer::~ShadowRenderer ()
{ }

void ShadowRenderer::Enable (cs237::mat4f const &projectionMat,cs237::mat4f const &projectionMatLight)
{
    _shader->Use();
    cs237::setUniform(projection,projectionMat);
    cs237::setUniform(projectionLight,projectionMatLight);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    CS237_CHECK( glEnable (GL_DEPTH_TEST));
}


void ShadowRenderer::Render (cs237::mat4f const &modelViewMat,cs237::mat4f const &modelViewMatLight,Drawable *drawable)
{

  // Texture rendering operations
  CS237_CHECK(glActiveTexture(GL_TEXTURE0));  // activate first texture unit
  drawable->texture->Bind();
  cs237::setUniform(_samplerLocShadow, 1); 

  cs237::setUniform(_samplerLoc, 0);
  cs237::setUniform(modelView,modelViewMat * drawable->modelMat);
  cs237::setUniform(modelViewLight,modelViewMatLight * drawable->modelMat);
  cs237::setUniform(direction,drawable->direc);
  cs237::setUniform(ambient,drawable->ambient);
  cs237::setUniform(intensity,drawable->intensity);
  cs237::setUniform(shadow,drawable->shadowFactor);

  drawable->Draw();
} 


