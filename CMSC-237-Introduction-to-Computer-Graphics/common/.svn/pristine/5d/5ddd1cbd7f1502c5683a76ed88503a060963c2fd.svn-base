/*! \file cs237-vec3.inl
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Inline operations on vec3 values.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_VEC3_INL_
#define _CS237_VEC3_INL_

#ifndef _CS237_HXX_
#error "c237-vec3.inl should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      // first three components of a 4-vector
	template <typename T>
	vec3<T>::vec3 (vec4<T> const &v) : x(v.x), y(v.y), z(v.z) { }

      // assignment
	template <typename T>
	inline vec3<T> & vec3<T>::operator= (vec3<T> const &v)
	{
	    this->x = v.x;
	    this->y = v.y;
	    this->z = v.z;
	    return *this;
	}

      // subscript
	template <typename T>
	inline T const & vec3<T>::operator[] (unsigned int const &i) const
	{
	    assert (i < 3);
	    return (&this->x)[i];
	}
	template <typename T>
	inline T & vec3<T>::operator[] (unsigned int const &i)
	{
	    assert (i < 3);
	    return (&this->x)[i];
	}

      // + assignment: +scalar and +vector
	template <typename T>
	inline vec3<T> & vec3<T>::operator+= (T const &s)
	{
	    this->x += s;
	    this->y += s;
	    this->z += s;
	    return *this;
	}
	template <typename T>
	inline vec3<T> & vec3<T>::operator+= (vec3<T> const &v)
	{
	    this->x += v.x;
	    this->y += v.y;
	    this->z += v.z;
	    return *this;
	}

      // - assignment: -scalar and -vector
	template <typename T>
	inline vec3<T> & vec3<T>::operator-= (T const &s)
	{
	    this->x -= s;
	    this->y -= s;
	    this->z -= s;
	    return *this;
	}
	template <typename T>
	inline vec3<T> & vec3<T>::operator-= (vec3<T> const &v)
	{
	    this->x -= v.x;
	    this->y -= v.y;
	    this->z -= v.z;
	    return *this;
	}

      // * assignment: *scalar and *vector
	template <typename T>
	inline vec3<T> & vec3<T>::operator*= (T const &s)
	{
	    this->x *= s;
	    this->y *= s;
	    this->z *= s;
	    return *this;
	}
	template <typename T>
	inline vec3<T> & vec3<T>::operator*= (vec3<T> const &v)
	{
	    this->x *= v.x;
	    this->y *= v.y;
	    this->z *= v.z;
	    return *this;
	}

      // / assignment: /scalar
	template <typename T>
	inline vec3<T> & vec3<T>::operator/= (T const &s)
	{
	    T one_s = T(1.0)/s;
	    this->x *= one_s;
	    this->y *= one_s;
	    this->z *= one_s;
	    return *this;
	}

	template <typename T>
	inline T vec3<T>::length () const
	{
	    return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
	}

	template <typename T>
	inline vec3<T> & vec3<T>::normalize ()
	{
	    return (*this /= this->length());
	}

    // addition: scalar+vector, vector+scalar, vector+vector
	template <typename T>
	inline vec3<T> operator+ (T const &s, vec3<T> const &v)
	{
	    return vec3<T>(T(s) + v.x, T(s) + v.y, T(s) + v.z);
	}
	template <typename T>
	inline vec3<T> operator+ (vec3<T> const &v, T const &s)
	{
	    return vec3<T>(v.x + T(s), v.y + T(s), v.z + T(s));
	}
	template <typename T>
	inline vec3<T> operator+ (vec3<T> const &v1, vec3<T> const &v2)
	{
	    return vec3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}

    // subtraction: scalar-vector, vector-scalar, vector-vector
	template <typename T>
	inline vec3<T> operator- (T const &s, vec3<T> const &v)
	{
	    return vec3<T>(T(s) - v.x, T(s) - v.y, T(s) - v.z);
	}
	template <typename T>
	inline vec3<T> operator- (vec3<T> const &v, T const &s)
	{
	    return vec3<T>(v.x - T(s), v.y - T(s), v.z - T(s));
	}
	template <typename T>
	inline vec3<T> operator- (vec3<T> const &v1, vec3<T> const &v2)
	{
	    return vec3<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}

    // multiplication: scalar*vector, vector*scalar, vector*vector
	template <typename T>
	inline vec3<T> operator* (T const &s, vec3<T> const &v)
	{
	    return vec3<T>(T(s) * v.x, T(s) * v.y, T(s) * v.z);
	}
	template <typename T>
	inline vec3<T> operator* (vec3<T> const &v, T const &s)
	{
	    return vec3<T>(v.x * T(s), v.y * T(s), v.z * T(s));
	}
	template <typename T>
	inline vec3<T> operator* (vec3<T> const &v1, vec3<T> const &v2)
	{
	    return vec3<T>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
	}

    // division: vector/scalar
	template <typename T>
	inline vec3<T> operator/ (vec3<T> const &v, T const &s)
	{
	    return (T(1.0)/s) * v;
	}

      // unary negation
	template <typename T>
	inline vec3<T> operator- (vec3<T> const &v)
	{
	    return vec3<T>(-v.x, -v.y, -v.z);
	}

      /********** Other functions **********/

      // length of a vector
	template <typename T>
	inline T length (__detail::vec3<T> const &v)
	{
	    return v.length();
	}

      // return the unit vector in the direction of v
	template <typename T>
	inline vec3<T> normalize (vec3<T> const &v)
	{
	    return v / v.length();
	}

      // distance between two points
	template <typename T>
	inline T distance (vec3<T> const &p1, vec3<T> const &p2)
	{
	    return length(p2 - p1);
	}

      // dot product (inner product)
	template <typename T>
	inline T dot (vec3<T> const &v1, vec3<T> const &v2)
	{
	    return (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z);
	}

      // cross product
	template <typename T>
	inline vec3<T> cross (vec3<T> const &v1, vec3<T> const &v2)
	{
	    return vec3<T> (
		v1.y*v2.z - v1.z*v2.y,
		v1.z*v2.x - v1.x*v2.z,
		v1.x*v2.y - v1.y*v2.x);
	}

	template <typename T>
	inline T const *valuePtr (vec3<T> const &v)
	{
	    return &(v.x);
	}

      //! \brief linear interpolation of 3D vectors
      //! \param a value for t = 0
      //! \param t interpolation factor
      //! \param b value for t = 1
	template <typename T>
	inline vec3<T> mix (vec3<T> const &a, T t, vec3<T> const &b)
	{
	    return (a + t * (b - a));
	}

    } /* namespace __detail */

} /* namespace cs237 */

#endif /* !_CS237_VEC3_INL_ */
