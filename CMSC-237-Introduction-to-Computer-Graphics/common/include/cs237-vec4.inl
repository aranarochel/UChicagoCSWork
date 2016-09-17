/*! \file cs237-vec4.inl
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Inline operations on vec4 values.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_VEC4_INL_
#define _CS237_VEC4_INL_

#ifndef _CS237_HXX_
#error "c237-vec4.inl should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      // assignment
	template <typename T>
	inline vec4<T> & vec4<T>::operator= (vec4<T> const &v)
	{
	    this->x = v.x;
	    this->y = v.y;
	    this->z = v.z;
	    this->w = v.w;
	    return *this;
	}

      // subscript
	template <typename T>
	inline T & vec4<T>::operator[] (unsigned int const &i)
	{
	    assert (i < 4);
	    return (&this->x)[i];
	}
	template <typename T>
	T const & vec4<T>::operator[] (unsigned int const &i) const
	{
	    assert (i < 4);
	    return (&this->x)[i];
	}

      // + assignment: +scalar and +vector
	template <typename T>
	inline vec4<T> & vec4<T>::operator+= (T const &s)
	{
	    this->x += s;
	    this->y += s;
	    this->z += s;
	    this->w += s;
	    return *this;
	}
	template <typename T>
	inline vec4<T> & vec4<T>::operator+= (vec4<T> const &v)
	{
	    this->x += v.x;
	    this->y += v.y;
	    this->z += v.z;
	    this->w += v.w;
	    return *this;
	}

      // - assignment: -scalar and -vector
	template <typename T>
	inline vec4<T> & vec4<T>::operator-= (T const &s)
	{
	    this->x -= s;
	    this->y -= s;
	    this->z -= s;
	    this->w -= s;
	    return *this;
	}
	template <typename T>
	inline vec4<T> & vec4<T>::operator-= (vec4<T> const &v)
	{
	    this->x -= v.x;
	    this->y -= v.y;
	    this->z -= v.z;
	    this->w -= v.w;
	    return *this;
	}

      // * assignment: *scalar and *vector
	template <typename T>
	inline vec4<T> & vec4<T>::operator*= (T const &s)
	{
	    this->x *= s;
	    this->y *= s;
	    this->z *= s;
	    this->w *= s;
	    return *this;
	}
	template <typename T>
	inline vec4<T> & vec4<T>::operator*= (vec4<T> const &v)
	{
	    this->x *= v.x;
	    this->y *= v.y;
	    this->z *= v.z;
	    this->w *= v.w;
	    return *this;
	}

      // / assignment: /scalar
	template <typename T>
	inline vec4<T> & vec4<T>::operator/= (T const &s)
	{
	    T one_s = T(1.0)/s;
	    this->x *= one_s;
	    this->y *= one_s;
	    this->z *= one_s;
	    this->w *= one_s;
	    return *this;
	}

	template <typename T>
	inline T vec4<T>::length () const
	{
	    return sqrt(this->x*this->x + this->y*this->y + this->z*this->z + this->w*this->w);
	}

	template <typename T>
	inline vec4<T> & vec4<T>::normalize ()
	{
	    return (*this /= this->length());
	}

    // addition: scalar+vector, vector+scalar, vector+vector
	template <typename T>
	inline vec4<T> operator+ (T const &s, vec4<T> const &v)
	{
	    return vec4<T>(T(s) + v.x, T(s) + v.y, T(s) + v.z, T(s) + v.w);
	}
	template <typename T>
	inline vec4<T> operator+ (vec4<T> const &v, T const &s)
	{
	    return vec4<T>(v.x + T(s), v.y + T(s), v.z + T(s), v.w + T(s));
	}
	template <typename T>
	inline vec4<T> operator+ (vec4<T> const &v1, vec4<T> const &v2)
	{
	    return vec4<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
	}

    // subtraction: scalar-vector, vector-scalar, vector-vector
	template <typename T>
	inline vec4<T> operator- (T const &s, vec4<T> const &v)
	{
	    return vec4<T>(T(s) - v.x, T(s) - v.y, T(s) - v.z, T(s) - v.w);
	}
	template <typename T>
	inline vec4<T> operator- (vec4<T> const &v, T const &s)
	{
	    return vec4<T>(v.x - T(s), v.y - T(s), v.z - T(s), v.w - T(s));
	}
	template <typename T>
	inline vec4<T> operator- (vec4<T> const &v1, vec4<T> const &v2)
	{
	    return vec4<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
	}

    // multiplication: scalar*vector, vector*scalar, vector*vector
	template <typename T>
	inline vec4<T> operator* (T const &s, vec4<T> const &v)
	{
	    return vec4<T>(T(s) * v.x, T(s) * v.y, T(s) * v.z, T(s) * v.w);
	}
	template <typename T>
	inline vec4<T> operator* (vec4<T> const &v, T const &s)
	{
	    return vec4<T>(v.x * T(s), v.y * T(s), v.z * T(s), v.w * T(s));
	}
	template <typename T>
	inline vec4<T> operator* (vec4<T> const &v1, vec4<T> const &v2)
	{
	    return vec4<T>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
	}

    // division: vector/scalar
	template <typename T>
	inline vec4<T> operator/ (vec4<T> const &v, T const &s)
	{
	    return (T(1.0)/s) * v;
	}

      // unary negation
	template <typename T>
	inline vec4<T> operator- (vec4<T> const &v)
	{
	    return vec4<T>(-v.x, -v.y, -v.z, -v.w);
	}

      /********** Other functions **********/

      // length of a vector
	template <typename T>
	inline T length (vec4<T> const &v)
	{
	    return v.length();
	}

      // return the unit vector in the direction of v
	template <typename T>
	inline vec4<T> normalize (vec4<T> const &v)
	{
	    return v / v.length();
	}

      // distance between two points
	template <typename T>
	inline T distance (vec4<T> const &p1, vec4<T> const &p2)
	{
	    return length(p2 - p1);
	}

      // dot product (inner product)
	template <typename T>
	inline T dot (vec4<T> const &v1, vec4<T> const &v2)
	{
	    return (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z) + (v1.w*v2.w);
	}

	template <typename T>
	inline T const *valuePtr (vec4<T> const &v)
	{
	    return &(v.x);
	}

      //! \brief linear interpolation of 4D vectors
      //! \param a value for t = 0
      //! \param t interpolation factor
      //! \param b value for t = 1
	template <typename T>
	inline vec4<T> mix (vec4<T> const &a, T t, vec4<T> const &b)
	{
	    return (a + t * (b - a));
	}

    } /* namespace __detail */

} /* namespace cs237 */

#endif /* !_CS237_VEC4_INL_ */
