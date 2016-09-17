/*! \file cs237-shader.cxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * This file implements an API for loading and using shaders.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include <sys/stat.h>
#include <cstring>

using namespace cs237;

static GLchar *ReadFile (const char *file);
static const char *ShaderName (GLenum kind);

/***** class ShaderObj member functions *****/

__detail::Shader::Obj::~Obj ()
{
    glDeleteShader (this->_id);
}

/***** class Shader member functions *****/

__detail::Shader::Shader (GLenum kind)
    : _obj(std::make_shared<__detail::Shader::Obj>(glCreateShader(kind))), _kind(kind)
{
    if (this->Id() == 0) {
	std::cerr << "Error creating shader: kind = " << ShaderName(kind) << ", error = " << glGetError() << std::endl;
	exit (1);
    }
}

//! \brief load and compile a shader from a file
//! \param file the source file name
//! \return true if successful and fals if there was an error
//
bool __detail::Shader::LoadFromFile (const char *file)
{
    GLint sts;

    GLchar *src = ReadFile (file);

    if (src == nullptr)
	return false;

    glShaderSource (this->Id(), 1, &src, 0);
    delete src;

    glCompileShader (this->Id());

  /* check for errors */
    glGetShaderiv (this->Id(), GL_COMPILE_STATUS, &sts);
    if (sts != GL_TRUE) {
      /* the compile failed, so report an error */
	glGetShaderiv (this->Id(), GL_INFO_LOG_LENGTH, &sts);
	if (sts != 0) {
	    GLchar *log =  new GLchar[sts];
	    glGetShaderInfoLog (this->Id(), sts, 0, log);
	    std::cerr << "Error compiling " << ShaderName(this->_kind) << " shader \""
		<< file << "\":\n" << log << "\n" << std::endl;
	    delete log;
	}
	else
	    std::cerr << "Error compiling " << ShaderName(this->_kind) << " shader \""
		<< file << "\":\n  no log\n" << std::endl;
	return false;
    }

    return true;

}

bool __detail::Shader::LoadFromString (const char **src)
{
    GLint sts;

    if (src == nullptr)
	return false;

    int n = 0;
    while (src[n] != nullptr) n++;
    glShaderSource (this->Id(), n, src, 0);

    glCompileShader (this->Id());

  /* check for errors */
    glGetShaderiv (this->Id(), GL_COMPILE_STATUS, &sts);
    if (sts != GL_TRUE) {
      /* the compile failed, so report an error */
	glGetShaderiv (this->Id(), GL_INFO_LOG_LENGTH, &sts);
	if (sts != 0) {
	    GLchar *log =  new GLchar[sts];
	    glGetShaderInfoLog (this->Id(), sts, 0, log);
	    std::cerr << "Error compiling " << ShaderName(this->_kind) << " shader:\n"
		<< log << "\n" << std::endl;
	    delete log;
	}
	else
	    std::cerr << "Error compiling " << ShaderName(this->_kind)
		<< " shader:\n  no log\n" << std::endl;
	return false;
    }

    return true;

}

/***** class VertexShader member functions *****/

VertexShader::VertexShader (const char *file)
    : Shader (GL_VERTEX_SHADER)
{
    if (! this->LoadFromFile (file)) {
	exit (1);
    }
}

/*! \brief define a shader from an array of strings
 *  \param file the name of the shader source file
 *  \returns a pointer to the vertex shader.
 *
 * This function loads and compiles a vertex shader from a file.  If there is
 * an error, it will print information to stderr and return 0.
 */
VertexShader::VertexShader (const char **prog)
    : Shader (GL_VERTEX_SHADER)
{
    if (! this->LoadFromString (prog)) {
	exit (1);
    }
}


/***** class GeometryShader member functions *****/

GeometryShader::GeometryShader (const char *file)
    : Shader (GL_GEOMETRY_SHADER)
{
    if (! this->LoadFromFile (file)) {
	exit (1);
    }
}

/*! \brief define a shader from an array of strings
 *  \param file the name of the shader source file
 *  \returns a pointer to the vertex shader.
 *
 * This function loads and compiles a geometry shader from a file.  If there is
 * an error, it will print information to stderr and return 0.
 */
GeometryShader::GeometryShader (const char **prog)
    : Shader (GL_GEOMETRY_SHADER)
{
    if (! this->LoadFromString (prog)) {
	exit (1);
    }
}


/***** class FragmentShader member functions *****/

FragmentShader::FragmentShader (const char *file)
    : Shader (GL_FRAGMENT_SHADER)
{
    if (! this->LoadFromFile (file)) {
	exit (1);
    }
}

/*! \brief define a shader from an array of strings
 *  \param file the name of the shader source file
 *  \returns a pointer to the vertex shader.
 *
 * This function loads and compiles a vertex shader from a file.  If there is
 * an error, it will print information to stderr and return 0.
 */
FragmentShader::FragmentShader (const char **prog)
    : Shader (GL_FRAGMENT_SHADER)
{
    if (! this->LoadFromString (prog)) {
	exit (1);
    }
}

/***** class ShaderProgram member functions *****/

//! \brief crete a shader program from a vertex and fragment shaders
//
ShaderProgram::ShaderProgram (VertexShader const &vsh, FragmentShader const &fsh)
    : _obj(std::make_shared<ShaderProgram::Obj>(glCreateProgram ()))
{
    if (this->Id() == 0) {
	std::cerr << "Error creating shader program\n" << std::endl;
	exit (1);
    }

  // attach the shaders
    glAttachShader (this->Id(), vsh.Id());
    glAttachShader (this->Id(), fsh.Id());

    this->_Link();

}

//! \brief crete a shader program from a vertex and fragment shaders
//
ShaderProgram::ShaderProgram (VertexShader const &vsh, GeometryShader const &gsh, FragmentShader const &fsh)
    : _obj(std::make_shared<ShaderProgram::Obj>(glCreateProgram ()))
{
    if (this->Id() == 0) {
	std::cerr << "Error creating shader program\n" << std::endl;
	exit (1);
    }

  // attach the shaders
    glAttachShader (this->Id(), vsh.Id());
    glAttachShader (this->Id(), gsh.Id());
    glAttachShader (this->Id(), fsh.Id());

    this->_Link();

}

//! \brief delete the underlying shader program object
//
ShaderProgram::Obj::~Obj ()
{
    glDeleteProgram (this->_id);
}

//! \brief return the location of an attribute variable in this shader
//
GLint ShaderProgram::AttributeLocation (const char *name)
{
    GLint id = glGetAttribLocation (this->Id(), name);
    if (id < 0) {
	std::cerr << "Error: shader attribute \"" << name << "\" is invalid\n" << std::endl;
	exit (1);
    }
    return id;
}

//! \brief return the location of a uniform variable in this shader
//
GLint ShaderProgram::UniformLocation (const char *name)
{
    GLint id = glGetUniformLocation (this->Id(), name);
    if (id < 0) {
	std::cerr << "Error: shader uniform \"" << name << "\" is invalid\n" << std::endl;
	exit (1);
    }
    return id;
}

//! \brief link a shader program
//
void ShaderProgram::_Link ()
{
    GLint	sts;

    glLinkProgram (this->Id());

  /* check for errors in the linking */
    glGetProgramiv (this->Id(), GL_LINK_STATUS, &sts);
    if (sts != GL_TRUE) {
      /* the link failed, so report an error */
	glGetProgramiv (this->Id(), GL_INFO_LOG_LENGTH, &sts);
	if (sts != 0) {
	    GLchar *log = new GLchar[sts];
	    glGetProgramInfoLog (this->Id(), sts, 0, log);
	    std::cerr << "Error linking program:\n" << log << "\n" << std::endl;
	    std::cerr << "Shader ID = " << this->Id() << "\n";
	    delete log;
	}
	else
	    std::cerr << "Error linking program:\n  no log\n" << std::endl;
	exit (1);
    }

}


/***** Local functions *****/

// ReadFile:
//
// Utility function for reading a source file
//
static GLchar *ReadFile (const char *file)
{
    struct stat	st;

  /* get the size of the file */
    if (stat(file, &st) < 0) {
	std::cerr << "error reading \"" << file << "\": " << strerror(errno) << std::endl;
	return 0;
    }
    off_t sz = st.st_size;

  /* open the file */
    FILE *strm = fopen(file, "r");
    if (strm == NULL) {
	std::cerr << "error opening \"" << file << "\": " << strerror(errno) << std::endl;
	return 0;
    }

  /* allocate the buffer */
    GLchar *buf = new GLchar[sz+1];

  /* read the file */
    if (fread(buf, 1, sz, strm) != size_t(sz)) {
	std::cerr << "error reading \"" << file << "\": " << strerror(errno) << std::endl;
	free (buf);
    }
    buf[sz] = '\0';

    fclose (strm);

    return buf;

}

static const char *ShaderName (GLenum kind)
{
    if (kind == GL_FRAGMENT_SHADER) return "fragment";
    else if (kind == GL_VERTEX_SHADER) return "vertex";
    else if (kind == GL_GEOMETRY_SHADER) return "geometry";
    else return "unknown";
}
