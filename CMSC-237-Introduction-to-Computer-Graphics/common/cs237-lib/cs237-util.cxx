/*! \file cs237-util.cxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"

void cs237::__detail::ReportError (GLenum err, const char *file, int line, const char *msg)
{
    const char *error;

    switch (err) {
      case GL_INVALID_ENUM: error = "invalid enum"; break;
      case GL_INVALID_VALUE: error = "invalid value"; break;
      case GL_INVALID_OPERATION: error = "invalid operation"; break;
      case GL_INVALID_FRAMEBUFFER_OPERATION: error = "invalid framebuffer operation"; break;
      case GL_OUT_OF_MEMORY: error = "out of memory"; break;
      default: error = "unknown"; break;
    }

    std::cerr << "[" << file << ":" << line << "] " << error << "; " << msg << "\n" << std::endl;

    exit (1);
}

// helper function to get integer values
static int32_t GetInt (GLenum tag)
{
    GLint i;
    glGetIntegerv (tag, &i);
    return i;
}

void cs237::DumpGLState (std::ostream &os)
{
    os << "***** OpenGL State *****\n";
    os << "** Version " << GetInt(GL_MAJOR_VERSION) << "." << GetInt(GL_MINOR_VERSION) << "\n";

/*
GL_ACTIVE_TEXTURE
GL_ARRAY_BUFFER_BINDING
GL_CURRENT_PROGRAM
GL_DRAW_BUFFER
GL_ELEMENT_ARRAY_BUFFER_BINDING
GL_RENDERBUFFER_BINDING
GL_TEXTURE_BINDING_1D
GL_TEXTURE_BINDING_2D
GL_TEXTURE_BINDING_3D
GL_TEXTURE_BINDING_BUFFER
GL_TEXTURE_BINDING_RECTANGLE
GL_VERTEX_ARRAY_BINDING
*/

  // culling state
    if (glIsEnabled(GL_CULL_FACE)) {
	os << "*****\n";
	os << "** Culling enabled:\n";
/*
GL_CULL_FACE_MODE
*/
    }

  // blending state
    if (glIsEnabled(GL_BLEND)) {
	os << "*****\n";
	os << "** Blending enabled:\n";
/*
GL_BLEND_COLOR
GL_BLEND_DST_ALPHA
GL_BLEND_DST_RGB
GL_BLEND_EQUATION_RGB
GL_BLEND_EQUATION_ALPHA
GL_BLEND_SRC_ALPHA
GL_BLEND_SRC_RGB
*/
    }

  // depth testing
    if (glIsEnabled(GL_DEPTH_TEST)) {
	os << "*****\n";
	os << "** Depth testing enabled:\n";
/*
GL_DEPTH_FUNC
GL_DEPTH_RANGE
GL_DEPTH_TEST
GL_DEPTH_WRITEMASK
*/
    }

  // stencil testing
    if (glIsEnabled(GL_STENCIL_TEST)) {
	os << "*****\n";
	os << "** Stencil testing enabled:\n";
/*
GL_STENCIL_BACK_FAIL
GL_STENCIL_BACK_FUNC
GL_STENCIL_BACK_PASS_DEPTH_FAIL
GL_STENCIL_BACK_PASS_DEPTH_PASS
GL_STENCIL_BACK_REF
GL_STENCIL_BACK_VALUE_MASK
GL_STENCIL_BACK_WRITEMASK
GL_STENCIL_CLEAR_VALUE
GL_STENCIL_FAIL
GL_STENCIL_FUNC
GL_STENCIL_PASS_DEPTH_FAIL
GL_STENCIL_PASS_DEPTH_PASS
GL_STENCIL_REF
GL_STENCIL_TEST
GL_STENCIL_VALUE_MASK
GL_STENCIL_WRITEMASK
*/
    }

    os << "********************" << std::endl;

}