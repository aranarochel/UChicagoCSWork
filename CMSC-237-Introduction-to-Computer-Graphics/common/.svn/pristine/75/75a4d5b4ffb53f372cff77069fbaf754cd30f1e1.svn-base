/*! \file cs237-quat.inl
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Inline operations on quaternions.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_QUAT_INL_
#define _CS237_QUAT_INL_

#ifndef _CS237_HXX_
#error "c237-quat.inl should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      //! create a quaternion from an angle (in degrees) and an axis vector
	template <typename T>
	inline quat<T>::quat (T theta, vec3<T> const &axis)
	{
	    T len = axis.length();
	    if (len <= T(0.0001)) {
	      // no rotation, since axis is degenerate, so return I
		this->x = T(0);
		this->y = T(0);
		this->z = T(0);
		this->w = T(1);
	    }
	    else {
		__detail::vec3<T> a = axis / len;
		T angle = 0.5 * radians(theta);
		T s = sin(angle);
		T x = s * a.x;
		T y = s * a.y;
		T z = s * a.z;
		T w = cos(angle);
	      // normalize the quaternion
		s = T(1) / sqrt(x*x + y*y + z*z + w*w);
		this->x = s*x;
		this->y = s*y;
		this->z = s*z;
		this->w = s*w;
	    }
	}

      // assignment
	template <typename T>
	inline quat<T> & quat<T>::operator= (quat<T> const &v)
	{
	    this->x = v.x;
	    this->y = v.y;
	    this->z = v.z;
	    this->w = v.w;
	    return *this;
	}

      // subscript
	template <typename T>
	inline T & quat<T>::operator[] (unsigned int const &i)
	{
	    assert (i < 4);
	    return (&this->x)[i];
	}
	template <typename T>
	T const & quat<T>::operator[] (unsigned int const &i) const
	{
	    assert (i < 4);
	    return (&this->x)[i];
	}

      // + assignment: +quaternion
	template <typename T>
	inline quat<T> & quat<T>::operator+= (quat<T> const &q)
	{
	    this->x += q.x;
	    this->y += q.y;
	    this->z += q.z;
	    this->w += q.w;
	    return *this;
	}

      // - assignment: -quaternion
	template <typename T>
	inline quat<T> & quat<T>::operator-= (quat<T> const &q)
	{
	    this->x -= q.x;
	    this->y -= q.y;
	    this->z -= q.z;
	    this->w -= q.w;
	    return *this;
	}

      // * assignment: *scalar, *quaternion, *vec3
	template <typename T>
	inline quat<T> & quat<T>::operator*= (T const &s)
	{
	    this->x *= s;
	    this->y *= s;
	    this->z *= s;
	    this->w *= s;
	    return *this;
	}
	template <typename T>
	inline quat<T> & quat<T>::operator*= (quat<T> const &q)
	{
	    this->x = this->w*q.x + this->x*q.w + this->y*q.z - this->z*q.y;
	    this->y = this->w*q.y - this->x*q.z + this->y*q.w + this->z*q.x;
	    this->z = this->w*q.z + this->x*q.y - this->y*q.x + this->z*q.w;
	    this->w = this->w*q.w - this->x*q.x - this->y*q.y - this->z*q.z;
	    return *this;
	}
	template <typename T>
	inline quat<T> & quat<T>::operator*= (vec3<T> const &v)
	{
	    this->x = this->w*v.x + this->y*v.z - this->z*v.y;
	    this->y = this->w*v.y - this->x*v.z + this->z*v.x;
	    this->z = this->w*v.z + this->x*v.y - this->y*v.x;
	    this->w = -this->x*v.x - this->y*v.y - this->z*v.z;
	    return *this;
	}

      // / assignment: /scalar
	template <typename T>
	inline quat<T> & quat<T>::operator/= (T const &s)
	{
	    T one_s = T(1.0)/s;
	    this->x *= one_s;
	    this->y *= one_s;
	    this->z *= one_s;
	    this->w *= one_s;
	    return *this;
	}

	template <typename T>
	inline quat<T> quat<T>::conjugate () const
	{
	    return quat<T>(-this->x, -this->y, -this->z, this->w);
	}

	template <typename T>
	inline quat<T> quat<T>::inverse () const
	{
	    T s = T(1) / (this->x*this->x + this->y*this->y + this->z*this->z + this->w*this->w);
	    return quat<T>(-s * this->x, -s * this->y, -s * this->z, s * this->w);
	}

	template <typename T>
	inline T quat<T>::length () const
	{
	    return sqrt(this->x*this->x + this->y*this->y + this->z*this->z + this->w*this->w);
	}

	template <typename T>
	inline quat<T> & quat<T>::normalize ()
	{
	    return (*this /= this->length());
	}

	template <typename T>
	inline vec3<T> quat<T>::rotate (vec3<T> const &v) const
	{
	  // the rotation of a vector by q is defined to be q * v * q^{-1}, but since we
	  // are assuming that this quaternion is unit, we can replace q^{-1} by q's conjugate.
	  // Furthermore, the w component of the result will be 0,

	  // compute q = this * v;
	    quat<T> q = quat(
	    	this->w*v.x + this->y*v.z - this->z*v.y,
	    	this->w*v.y - this->x*v.z + this->z*v.x,
	    	this->w*v.z + this->x*v.y - this->y*v.x,
	    	-this->x*v.x - this->y*v.y - this->z*v.z);

	  // compute the <x,y,z> part of q * this->conjugate();
	    return vec3<T> (
		-q.w*this->x + q.x*this->w - q.y*this->z + q.z*this->y,
		-q.w*this->y + q.x*this->z + q.y*this->w - q.z*this->x,
		-q.w*this->z - q.x*this->y + q.y*this->x + q.z*this->w);

	}

    // addition: quaternion+quaternion
	template <typename T>
	inline quat<T> operator+ (quat<T> const &v1, quat<T> const &v2)
	{
	    return quat<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
	}

    // subtraction: quaternion-quaternion
	template <typename T>
	inline quat<T> operator- (quat<T> const &v1, quat<T> const &v2)
	{
	    return quat<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
	}

    // multiplication: scalar*quaternion, quaternion*scalar, quaternion*quaternion, quaternion*vec3
	template <typename T>
	inline quat<T> operator* (T const &s, quat<T> const &v)
	{
	    return quat<T>(T(s) * v.x, T(s) * v.y, T(s) * v.z, T(s) * v.w);
	}
	template <typename T>
	inline quat<T> operator* (quat<T> const &v, T const &s)
	{
	    return quat<T>(v.x * T(s), v.y * T(s), v.z * T(s), v.w * T(s));
	}
	template <typename T>
	inline quat<T> operator* (quat<T> const &q1, quat<T> const &q2)
	{
	    return quat<T> (
		q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
		q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x,
		q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w,
		q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z);
	}
	template <typename T>
	inline quat<T> operator* (quat<T> const &q, vec3<T> const &v)
	{
	    return quat<T> (
		q.w*v.x + q.y*v.z - q.z*v.y,
		q.w*v.y - q.x*v.z + q.z*v.x,
		q.w*v.z + q.x*v.y - q.y*v.x,
		-q.x*v.x - q.y*v.y - q.z*v.z);
	}

    // division: quaternion/scalar
	template <typename T>
	inline quat<T> operator/ (quat<T> const &q, T const &s)
	{
	    return (T(1.0)/s) * q;
	}

      // unary negation
	template <typename T>
	inline quat<T> operator- (quat<T> const &q)
	{
	    return quat<T>(-q.x, -q.y, -q.z, -q.w);
	}

      /********** Other functions **********/

      // length of a vector
	template <typename T>
	inline T length (quat<T> const &v)
	{
	    return v.length();
	}

      // return the unit vector in the direction of v
	template <typename T>
	inline quat<T> normalize (quat<T> const &v)
	{
	    return v / v.length();
	}

      // dot product (inner product)
	template <typename T>
	inline T dot (quat<T> const &q1, quat<T> const &q2)
	{
	    return (q1.x*q2.x) + (q1.y*q2.y) + (q1.z*q2.z) + (q1.w*q2.w);
	}

	template <typename T>
	inline T const *valuePtr (quat<T> const &q)
	{
	    return &(q.x);
	}

      // linear interpolation between unit quats.
	template <typename T>
	inline quat<T> mix (quat<T> const &q1, T t, quat<T> const &q2) 
	{
	   return q1 + t *(q2 - q1); 
	}

      //! \brief normalized linear interpolation of unit quats
	template <typename T>
	inline quat<T> nlerp (quat<T> const &q1, T t, quat<T> const &q2) 
	{
	   return normalize(q1 + t *(q2 - q1)); 
	}

      //! \brief spherical linear interpolation of quats
      //! \param a value for t = 0
      //! \param t interpolation factor
      //! \param b value for t = 1
	template <typename T>
	inline quat<T> slerp (quat<T> const &a, T t, quat<T> const &b)
	{
	    T cosTheta = dot(a, b);
	    if (cosTheta > 0.9995) { // theta is close to 0, so lerp
		return nlerp(a, t, b);
	    }
	    quat<T> c = b;
	    if (cosTheta < 0.0) {
		cosTheta = -cosTheta;
		c = -c;
	    }
	    T theta = acos(cosTheta);
	    return (std::sin((T(1)-t)*theta) * a + std::sin(t*theta) * c) / std::sin(theta);
	}

    } /* namespace __detail */

} /* namespace cs237 */

#endif /* !_CS237_QUAT_INL_ */
