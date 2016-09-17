/*! \file cs237-vec.cxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Operations on vector values.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"

namespace cs237 {
    namespace __detail {

	#define PROTO(VEC,T) std::ostream& operator<< (std::ostream& s, VEC<T> const &v)

	// output the vector to a stream as a sequence of column vectors
	//
	template <typename T> PROTO(vec2,T)
	{
	    return (s << "<" << v.x << "," << v.y << ">");
	}
	template PROTO(vec2,float);
	template PROTO(vec2,double);

	// output the vector to a stream as a sequence of column vectors
	//
	template <typename T> PROTO(vec3,T)
	{
	    return (s << "<" << v.x << "," << v.y << "," << v.z << ">");
	}
	template PROTO(vec3,float);
	template PROTO(vec3,double);

	// output the vector to a stream as a sequence of column vectors
	//
	template <typename T> PROTO(vec4,T)
	{
	    return (s << "<" << v.x << "," << v.y << "," << v.z << "," << v.w << ">");
	}
	template PROTO(vec4,float);
	template PROTO(vec4,double);

	#undef PROTO
    } // namespace __detail

} // namespace cs237
