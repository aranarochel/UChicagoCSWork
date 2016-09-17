/*! \file render.cxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "render.hxx"
#include "view.hxx"
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

/***** virtual base-class RenderPass member functions *****/

RenderPass::RenderPass (cs237::ShaderProgram *sh)
    : _shader(sh)
{ }

RenderPass::~RenderPass ()
{ }

/***** class WireframeRenderer member functions *****/

WireframeRenderer::WireframeRenderer ()
    : RenderPass (LoadShader (SHADER_DIR "no-light")),
	_mvpLoc(this->_shader->UniformLocation ("mvpMat")),
	_colorLoc(this->_shader->UniformLocation ("color"))
{ }

WireframeRenderer::~WireframeRenderer ()
{ }

void WireframeRenderer::Enable ()
{
    this->_shader->Use();
    CS237_CHECK( glDisable (GL_CULL_FACE) );
    CS237_CHECK( glEnable (GL_DEPTH_TEST) );
    CS237_CHECK( glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) );
}

void WireframeRenderer::Render (cs237::mat4f const &projMat, cs237::mat4f const &viewMat, const Instance *obj,SpotLight light)
{
  // set model-view-projection matrix
    cs237::mat4f mvpMat = projMat * viewMat * obj->toWorld;
    cs237::setUniform (this->_mvpLoc, mvpMat);

  // set object color
    cs237::setUniform (this->_colorLoc, cs237::color4f(obj->color));

  // draw
    for (auto it = obj->meshes.begin(); it != obj->meshes.end();  it++) {
	(*it)->Draw (false, false, false);
    }

}

/***** class TexturingRenderer member functions *****/

TexturingRenderer::TexturingRenderer (
    cs237::vec3f const &dir,
    cs237::color3f const &i,
    cs237::color3f const &amb)
    : RenderPass(LoadShader (SHADER_DIR "texture")),
	_lightDir(-dir), _lightI(i), _ambI(amb),
	_mvpLoc(this->_shader->UniformLocation ("mvpMat")),
	_normMatLoc(this->_shader->UniformLocation ("normMat")),
	_lightDirLoc(this->_shader->UniformLocation ("lightDir")),
	_lightILoc(this->_shader->UniformLocation ("lightIntensity")),
	_ambILoc(this->_shader->UniformLocation ("ambIntensity")),
	_hasDiffuseMapLoc(this->_shader->UniformLocation ("hasDiffuseMap")),
	_diffuseCLoc(this->_shader->UniformLocation ("diffuseC")),
	_diffuseMapLoc(this->_shader->UniformLocation ("diffuseMap"))
{
    this->_shader->Use();

  // set lighting uniforms
    cs237::setUniform (this->_lightDirLoc, this->_lightDir);
    cs237::setUniform (this->_lightILoc, this->_lightI);
    cs237::setUniform (this->_ambILoc, this->_ambI);

}

TexturingRenderer::~TexturingRenderer ()
{
}

void TexturingRenderer::Enable ()
{
    this->_shader->Use();
    CS237_CHECK( glEnable (GL_DEPTH_TEST) );
    CS237_CHECK( glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) );
}

void TexturingRenderer::Render (cs237::mat4f const &projMat, cs237::mat4f const &viewMat, const Instance *obj,SpotLight light)
{
  // set model-view-projection matrix
    cs237::mat4f mvpMat = projMat * viewMat * obj->toWorld;
    cs237::setUniform (this->_mvpLoc, mvpMat);

  // set the normal matrix
    cs237::setUniform (this->_normMatLoc, obj->toWorld.normalMatrix());

  // set the diffuse-color texture unit
    cs237::setUniform (this->_diffuseMapLoc, 0);

    for (auto it = obj->meshes.begin(); it != obj->meshes.end();  it++) {
	MeshInfo *mesh = *it;
      // set up the diffuse color
	if (mesh->hasDiffuseMap()) {
	    mesh->BindDiffuseMap (GL_TEXTURE0);
	    cs237::setUniform (this->_hasDiffuseMapLoc, GL_TRUE);
	    cs237::setUniform (this->_diffuseMapLoc, 0);
	}
	else {
	    cs237::setUniform (this->_hasDiffuseMapLoc, GL_FALSE);
	    cs237::setUniform (this->_diffuseCLoc, mesh->DiffuseColor());
	}
      // draw the mesh with normals and texture coordinates enabled, but not normal-mapping tangents
	mesh->Draw (true, true, false);
    }

}

/***** class GBufferRenderer member functions *****/

GBufferRenderer::GBufferRenderer ()
    : RenderPass (LoadShader (SHADER_DIR "gbuf")),
  _mvpLoc(this->_shader->UniformLocation ("mvpMat")),
  _normMatLoc(this->_shader->UniformLocation ("normMat")),
  _toWorldLoc(this->_shader->UniformLocation ("toWorldMat")),
  _hasDiffuseMapLoc(this->_shader->UniformLocation ("hasDiffuseMap")),
  _diffuseCLoc(this->_shader->UniformLocation ("diffuseC")),
  _diffuseMapLoc(this->_shader->UniformLocation ("diffuseMap"))
{ }

GBufferRenderer::~GBufferRenderer ()
{ }

void GBufferRenderer::Enable ()
{
    this->_shader->Use();

    CS237_CHECK( glEnable (GL_DEPTH_TEST) );
    CS237_CHECK( glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) );
}

void GBufferRenderer::Render (cs237::mat4f const &projMat, cs237::mat4f const &viewMat, const Instance *obj,SpotLight light)
{
  // set model-view-projection matrix
    cs237::mat4f mvpMat = projMat * viewMat * obj->toWorld;
    cs237::setUniform (this->_mvpLoc, mvpMat);
    
  // set the normal matrix
    cs237::setUniform (this->_normMatLoc, obj->toWorld.normalMatrix());

  //set the to world matrix
    cs237::setUniform (this->_toWorldLoc, obj->toWorld);

  //draw
    for (auto it = obj->meshes.begin(); it != obj->meshes.end();  it++) {
      MeshInfo *mesh = *it;

      // set up the diffuse color
      if (mesh->hasDiffuseMap()) {
          mesh->BindDiffuseMap (GL_TEXTURE0);
          cs237::setUniform (this->_hasDiffuseMapLoc, GL_TRUE);
          cs237::setUniform (this->_diffuseMapLoc, 0);
      }
      else {
          cs237::setUniform (this->_hasDiffuseMapLoc, GL_FALSE);
          cs237::setUniform (this->_diffuseCLoc, mesh->DiffuseColor());
      }
      // draw the mesh with normals
      mesh->Draw (true, true, false);
    }
}

/***** class DeferredRenderer member functions *****/

DeferredRenderer::DeferredRenderer (
    cs237::vec3f const &dir,
    cs237::color3f const &i,
    cs237::color3f const &amb)
    : RenderPass(LoadShader (SHADER_DIR "deferred")),
  _lightDir(-dir), _lightI(i), _ambI(amb),
  _lightType(this->_shader->UniformLocation ("lightType")),
  _mvpLoc(this->_shader->UniformLocation ("mvpMat")),
  _lightDirLoc(this->_shader->UniformLocation ("lightDir")),
  _lightILoc(this->_shader->UniformLocation ("lightIntensity")),
  _ambILoc(this->_shader->UniformLocation ("ambIntensity")),
  _samplerPos(this->_shader->UniformLocation ("_samplerPos")),
  _samplerDiff(this->_shader->UniformLocation ("_samplerDiff")),
  _samplerNorm(this->_shader->UniformLocation ("_samplerNorm")),
  _spotPos(this->_shader->UniformLocation ("spotPos")),
  _spotDir(this->_shader->UniformLocation ("spotDir")),
  _spotAtt(this->_shader->UniformLocation ("spotAtt")),
  _spotCutt(this->_shader->UniformLocation ("spotCutt")),
  _spotExp(this->_shader->UniformLocation ("spotExp")),
  _camPos(this->_shader->UniformLocation("camPos")),
  _windowSize(this->_shader->UniformLocation("windowSize")),
  _specularC(this->_shader->UniformLocation("specularC")),
  _shininess(this->_shader->UniformLocation("shininess"))
{
  
}

DeferredRenderer::~DeferredRenderer ()
{
}

void DeferredRenderer::Enable ()
{
    this->_shader->Use();
    CS237_CHECK( glEnable (GL_DEPTH_TEST) );
    CS237_CHECK( glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) );

    // set lighting uniforms
    cs237::setUniform (this->_lightDirLoc, this->_lightDir);
    cs237::setUniform (this->_lightILoc, this->_lightI);
    cs237::setUniform (this->_ambILoc, this->_ambI);
}

void DeferredRenderer::Render (cs237::mat4f const &projMat, cs237::mat4f const &viewMat, const Instance *obj,SpotLight light)
{
    cs237::setUniform (this->_lightType, light.lightType);

  // set model-view-projection matrix
    cs237::mat4f mvpMat = projMat * viewMat * obj->toWorld;
    cs237::setUniform (this->_mvpLoc, mvpMat);

    cs237::setUniform (this->_samplerPos, 0);
    cs237::setUniform (this->_samplerDiff, 1);
    cs237::setUniform (this->_samplerNorm, 2);

    cs237::setUniform (this->_spotPos, light.pos);
    cs237::setUniform (this->_spotDir, light.dir);
    cs237::setUniform (this->_spotAtt, cs237::vec3f(light.atten[0],light.atten[1],light.atten[2]));
    cs237::setUniform (this->_spotCutt, light.cutoff);
    cs237::setUniform (this->_spotExp, light.exponent);
    cs237::setUniform (this->_camPos, light.camPos);
    cs237::setUniform (this->_windowSize, light.windowSize);

    for (auto it = obj->meshes.begin(); it != obj->meshes.end();  it++) {
      MeshInfo *mesh = *it;
          
      cs237::setUniform (this->_specularC, mesh->SpecularColor());
      cs237::setUniform (this->_shininess, mesh->Shininess());
      // draw the mesh with texture coordinates enabled, but not normal-mapping tangents
      mesh->Draw (false, false, false);
    }

}


/***** class QuadRenderer member functions *****/

QuadRenderer::QuadRenderer ()
    : RenderPass (LoadShader (SHADER_DIR "quad")), vaoId(0),
    _texLoc(this->_shader->UniformLocation("color"))
{ 
  glGenVertexArrays(1, &(this->vaoId));
}

QuadRenderer::~QuadRenderer ()
{ }

void QuadRenderer::Enable ()
{
    this->_shader->Use();
  
}

void QuadRenderer::Render (cs237::mat4f const &projMat, cs237::mat4f const &viewMat, const Instance *obj,SpotLight light)
{

  glBindVertexArray(this->vaoId);
  cs237::setUniform(this->_texLoc, 3);
  glDrawArrays(GL_TRIANGLES, 0,6 );

}



