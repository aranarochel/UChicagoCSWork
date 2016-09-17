/*! \file cs237.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_HXX_
#define _CS237_HXX_

#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#if defined(__APPLE_CC__)
  #include <OpenGL/glext.h>
#else
#  include <GL/glext.h>
#endif

#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <assert.h>

#include "cs237-types.hxx"

/* Vector types */
#include "cs237-vec2.hxx"
#include "cs237-vec3.hxx"
#include "cs237-vec4.hxx"

/* Matrix types */
#include "cs237-mat2x2.hxx"
#include "cs237-mat2x3.hxx"
#ifdef TODO
#include "cs237-mat2x4.hxx"
#endif /* TODO */
#include "cs237-mat3x2.hxx"
#include "cs237-mat3x3.hxx"
#ifdef TODO
#include "cs237-mat3x4.hxx"
#include "cs237-mat4x2.hxx"
#include "cs237-mat4x3.hxx"
#endif /* TODO */
#include "cs237-mat4x4.hxx"

#include "cs237-quat.hxx"

/* Geometry */
#include "cs237-aabb.hxx"

/* Color types */
#include "cs237-color.hxx"

/* transformations */
#include "cs237-transform.hxx"

/* Images and textures */
#include "cs237-image.hxx"
#include "cs237-texture.hxx"

/* Shader support */
#include "cs237-shader.hxx"

/* Inline operations */
#include "cs237-util.inl"

#include "cs237-vec2.inl"
#include "cs237-vec3.inl"
#include "cs237-vec4.inl"

#include "cs237-mat2x2.inl"
#include "cs237-mat2x3.inl"
#ifdef TODO
#include "cs237-mat2x4.inl"
#endif /* TODO */
#include "cs237-mat3x2.inl"
#include "cs237-mat3x3.inl"
#ifdef TODO
#include "cs237-mat3x4.inl"
#include "cs237-mat4x2.inl"
#include "cs237-mat4x3.inl"
#endif /* TODO */
#include "cs237-mat4x4.inl"

#include "cs237-quat.inl"

#include "cs237-aabb.inl"

#include "cs237-color.inl"

#include "cs237-shader.inl"

#include "cs237-sphere.hxx"

#include "cs237-window.hxx"

namespace cs237 {

  //! Dump the current OpenGL state to the given output stream
  //! \param os the output stream to dump the state to
  //!
  //! This function dumps partial information about the current
  //! state of OpenGL to an output stream.  The information includes
  //! things like the current VAO.  There is lots of information
  //! that is not dumped.
    void DumpGLState (std::ostream &os);

} // namespace cs237

#endif // !_CS237_HXX_
