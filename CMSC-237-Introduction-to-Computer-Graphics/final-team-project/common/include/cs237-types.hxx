/*! \file cs237-types.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_TYPES_HXX_
#define _CS237_TYPES_HXX_

#ifndef _CS237_HXX_
#error "c237-types.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      /* Vector types */
	template <typename T> struct vec2;
	template <typename T> struct vec3;
	template <typename T> struct vec4;

      /* Matrix types */
	template <typename T> struct mat2x2;
	template <typename T> struct mat2x3;
	template <typename T> struct mat2x4;
	template <typename T> struct mat3x2;
	template <typename T> struct mat3x3;
	template <typename T> struct mat3x4;
	template <typename T> struct mat4x2;
	template <typename T> struct mat4x3;
	template <typename T> struct mat4x4;

      /* Quaternions */
	template <typename T> struct quat;

    } /* namespace __detail */

  /* Colors */
    struct color3ub;
    struct color3f;
    struct color4ub;
    struct color4f;

} /* namespace cs237 */

#endif /* !_CS237_TYPES_HXX_ */
