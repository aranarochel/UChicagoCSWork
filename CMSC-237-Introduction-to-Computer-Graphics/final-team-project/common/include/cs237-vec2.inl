/*! \file cs237-vec2.inl
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Inline operations on vec2 values.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_VEC2_INL_
#define _CS237_VEC2_INL_

#ifndef _CS237_HXX_
#error "c237-vec2.inl should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      // first two components of a 3-vector
	template <typename T>
	inline vec2<T>::vec2 (vec3<T> const &v) : x(v.x), y(v.y) { }

      // first two components of a 4-vector
	template <typename T>
	inline vec2<T>::vec2 (vec4<T> const &v) : x(v.x), y(v.y) { }

      // subscript
	template <typename T>
	inline T & vec2<T>::operator[] (unsigned int const &i)
	{
	    assert (i < 2);
	    return (&this->x)[i];
	}
	template <typename T>
	inline T const & vec2<T>::operator[] (unsigned int const &i) const
	{
	    assert (i < 2);
	    return (&this->x)[i];
	}

      // assignment
	template <typename T>
	inline vec2<T> & vec2<T>::operator= (vec2<T> const &v)
	{
	    this->x = v.x;
	    this->y = v.y;
	    return *this;
	}

      // + assignment: +scalar and +vector
	template <typename T>
	inline vec2<T> & vec2<T>::operator+= (T const &s)
	{
	    this->x += s;
	    this->y += s;
	    return *this;
	}
	template <typename T>
	inline vec2<T> & vec2<T>::operator+= (vec2<T> const &v)
	{
	    this->x += v.x;
	    this->y += v.y;
	    return *this;
	}

      // - assignment: -scalar and -vector
	template <typename T>
	inline vec2<T> & vec2<T>::operator-= (T const &s)
	{
	    this->x -= s;
	    this->y -= s;
	    return *this;
	}
	template <typename T>
	inline vec2<T> & vec2<T>::operator-= (vec2<T> const &v)
	{
	    this->x -= v.x;
	    this->y -= v.y;
	    return *this;
	}

      // * assignment: *scalar and *vector
	template <typename T>
	inline vec2<T> & vec2<T>::operator*= (T const &s)
	{
	    this->x *= s;
	    this->y *= s;
	    return *this;
	}
	template <typename T>
	inline vec2<T> & vec2<T>::operator*= (vec2<T> const &v)
	{
	    this->x *= v.x;
	    this->y *= v.y;
	    return *this;
	}

      // / assignment: /scalar
	template <typename T>
	inline vec2<T> & vec2<T>::operator/= (T const &s)
	{
	    T one_s = T(1.0)/s;
	    this->x *= one_s;
	    this->y *= one_s;
	    return *this;
	}

	template <typename T>
	inline T vec2<T>::length () const
	{
	    return sqrt(this->x*this->x + this->y*this->y);
	}

	template <typename T>
	inline vec2<T> & vec2<T>::normalize ()
	{
	    return (*this /= this->length());
	}

    // addition: scalar+vector, vector+scalar, vector+vector
	template <typename T>
	inline vec2<T> operator+ (T const &s, vec2<T> const &v)
	{
	    return vec2<T>(T(s) + v.x, T(s) + v.y);
	}
	template <typename T>
	inline vec2<T> operator+ (vec2<T> const &v, T const &s)
	{
	    return vec2<T>(v.x + T(s), v.y + T(s));
	}
	template <typename T>
	inline vec2<T> operator+ (vec2<T> const &v1, vec2<T> const &v2)
	{
	    return vec2<T>(v1.x + v2.x, v1.y + v2.y);
	}

    // subtraction: scalar-vector, vector-scalar, vector-vector
	template <typename T>
	inline vec2<T> operator- (T const &s, vec2<T> const &v)
	{
	    return vec2<T>(T(s) - v.x, T(s) - v.y);
	}
	template <typename T>
	inline vec2<T> operator- (vec2<T> const &v, T const &s)
	{
	    return vec2<T>(v.x - T(s), v.y - T(s));
	}
	template <typename T>
	inline vec2<T> operator- (vec2<T> const &v1, vec2<T> const &v2)
	{
	    return vec2<T>(v1.x - v2.x, v1.y - v2.y);
	}

    // multiplication: scalar*vector, vector*scalar, vector*vector
	template <typename T>
	inline vec2<T> operator* (T const &s, vec2<T> const &v)
	{
	    return vec2<T>(T(s) * v.x, T(s) * v.y);
	}
	template <typename T>
	inline vec2<T> operator* (vec2<T> const &v, T const &s)
	{
	    return vec2<T>(v.x * T(s), v.y * T(s));
	}
	template <typename T>
	inline vec2<T> operator* (vec2<T> const &v1, vec2<T> const &v2)
	{
	    return vec2<T>(v1.x * v2.x, v1.y * v2.y);
	}

    // division: vector/scalar
	template <typename T>
	inline vec2<T> operator/ (vec2<T> const &v, T const &s)
	{
	    return (T(1.0)/s) * v;
	}

      // unary negation
	template <typename T>
	inline vec2<T> operator- (vec2<T> const &v)
	{
	    return vec2<T>(-v.x, -v.y);
	}

      /********** Other functions **********/

      // length of a vector
	template <typename T>
	inline T length (vec2<T> const &v)
	{
	    return v.length();
	}

      // return the unit vector in the direction of v
	template <typename T>
	inline vec2<T> normalize (vec2<T> const &v)
	{
	    return v / v.length();
	}

      // distance between two points
	template <typename T>
	inline T distance (vec2<T> const &p1, vec2<T> const &p2)
	{
	    return length(p2 - p1);
	}

      // dot product (inner product)
	template <typename T>
	inline T dot (vec2<T> const &v1, vec2<T> const &v2)
	{
	    return (v1.x*v2.x) + (v1.y*v2.y);
	}

	template <typename T>
	inline T const *valuePtr (vec2<T> const &v)
	{
	    return &(v.x);
	}

      //! \brief linear interpolation of 2D vectors
      //! \param a value for t = 0
      //! \param t interpolation factor
      //! \param b value for t = 1
	template <typename T>
	inline vec2<T> mix (vec2<T> const &a, T t, vec2<T> const &b)
	{
	    return (a + t * (b - a));
	}

    } /* namespace __detail */

} /* namespace cs237 */

#endif /* !_CS237_VEC2_INL_ */
