/*! \file cs237-quat.cxx
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

	//! \brief output the quaternion to a stream
	//! \param s the stream to print to
	//! \param q the quaternion to print
	#define PROTO(T) std::ostream& operator<< (std::ostream& s, quat<T> const &q)
	template <typename T> PROTO(T)
	{
	    return (s << "<" << q.x << "," << q.y << "," << q.z << "," << q.w << ">");
	}
	template PROTO(float);
	template PROTO(double);
	#undef PROTO


      //! return the 3x3 linear rotation matrix represented by this quaternion
	template <typename T>
	mat3x3<T> quat<T>::toMat3x3 () const
	{
	    T x2 = T(2)*this->x*this->x;
	    T y2 = T(2)*this->y*this->y;
	    T z2 = T(2)*this->z*this->z;
	    T xy = T(2)*this->x*this->y;
	    T xz = T(2)*this->x*this->z;
	    T yz = T(2)*this->y*this->z;
	    T wx = T(2)*this->w*this->x;
	    T wy = T(2)*this->w*this->y;
	    T wz = T(2)*this->w*this->z;

	    return mat3x3<T> (
		1 - y2 - z2,	x2 + wz,	xz - wy,	// first column
		xy - wz,	1 - x2 - z2,	yz + wx,	// second column
		xz + wy,	yz - wx,	1 - x2 - y2);	// third column
	}

      //! return the 3x3 affine rotation matrix represented by this quaternion
	template <typename T> 
	mat4x4<T> quat<T>::toMat4x4 () const
	{
	    T x2 = T(2)*this->x*this->x;
	    T y2 = T(2)*this->y*this->y;
	    T z2 = T(2)*this->z*this->z;
	    T xy = T(2)*this->x*this->y;
	    T xz = T(2)*this->x*this->z;
	    T yz = T(2)*this->y*this->z;
	    T wx = T(2)*this->w*this->x;
	    T wy = T(2)*this->w*this->y;
	    T wz = T(2)*this->w*this->z;

	    return mat4x4<T> (
		T(1) - y2 - z2,	x2 + wz,	xz - wy,	T(0),	// first column
		xy - wz,	T(1) - x2 - z2,	yz + wx,	T(0),	// second column
		xz + wy,	yz - wx,	T(1) - x2 - y2,	T(0),	// third column
		T(0),		T(0),		T(0),		T(1));	// fourth column
	}

	template struct quat<float>;
	template struct quat<double>;
    } // namespace __detail

} // namespace cs237
