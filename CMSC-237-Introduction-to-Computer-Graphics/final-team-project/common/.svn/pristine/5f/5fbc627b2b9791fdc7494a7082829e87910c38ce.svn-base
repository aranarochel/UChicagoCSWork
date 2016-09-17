/*! \file cs237-mat3x3.inl
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Inline operations on mat3x3 values.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_MAT3X3_INL_
#define _CS237_MAT3X3_INL_

#ifndef _CS237_HXX_
#error "c237-mat3x3.inl should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      /********** Class mat3x3<T> member functions **********/

      //! copy constructor
	template <typename T>
	inline mat3x3<T>::mat3x3 (mat3x3 const &mm)
	    : cv {mm.cv[0], mm.cv[1], mm.cv[2]}
	{ }

      //! construct a matrix from individual elements given in column-major order
	template <typename T>
	inline mat3x3<T>::mat3x3 (
	    T m00, T m10, T m20,	// first column
	    T m01, T m11, T m21,	// second column
	    T m02, T m12, T m22)	// third column
	    : cv {
		vec3<T>(m00, m10, m20),
		vec3<T>(m01, m11, m21),
		vec3<T>(m02, m12, m22)}
	{ }

      //! construct a matrix from column vectors
	template <typename T>
	inline mat3x3<T>::mat3x3 (vec3<T> const &c0, vec3<T> const &c1, vec3<T> const &c2)
	    : cv {c0, c1, c2}
	{ }

      //! extract the upper-left 3x3 matrix from a 4x4 matrix
	template <typename T>
	inline mat3x3<T>::mat3x3 (mat4x4<T> const &mm)
	    : cv {vec3<T>(mm.cv[0]), vec3<T>(mm.cv[1]), vec3<T>(mm.cv[2])}
	{ }

      //! extract the upper-left 3x3 matrix from a 3x4 matrix
	template <typename T>
	inline mat3x3<T>::mat3x3 (mat3x4<T> const &mm)
	    : cv {vec3<T>(mm.cv[0]), vec3<T>(mm.cv[1]), vec3<T>(mm.cv[2])}
	{ }

      //! extract the upper-left 3x3 matrix from a 4x3 matrix
	template <typename T>
	inline mat3x3<T>::mat3x3 (mat4x3<T> const &mm)
	    : cv {mm.cv[0], mm.cv[1], mm.cv[2]}
	{ }

      //! construct a diagonal matrix with diagonal elements initialized to x
	template <typename T>
	inline mat3x3<T>::mat3x3 (T const &x)
	    : cv {
		vec3<T>(x, T(0), T(0)),
		vec3<T>(T(0), x, T(0)),
		vec3<T>(T(0), T(0), x)}
	{ }

      //! return the i'th column vector
	template <typename T>
	inline vec3<T> & mat3x3<T>::operator[] (unsigned int const &i)
	{
	    assert (i < 3);
	    return this->cv[i];
	}

	template <typename T>
	inline vec3<T> const & mat3x3<T>::operator[] (unsigned int const &i) const
	{
	    assert (i < 3);
	    return this->cv[i];
	}

	template <typename T>
	inline mat3x3<T> & mat3x3<T>::operator= (mat3x3<T> const &mm)
	{
	    this->cv[0] = mm.cv[0];
	    this->cv[1] = mm.cv[1];
	    this->cv[2] = mm.cv[2];
	    return *this;
	}

	template <typename T>
	inline mat3x3<T> & mat3x3<T>::operator+= (T const &s)
	{
	    vec3<T> sv = vec3<T>(T(s), T(s), T(s));
	    this->cv[0] += sv;
	    this->cv[1] += sv;
	    this->cv[2] += sv;
	    return *this;
	}

	template <typename T>
	inline mat3x3<T> & mat3x3<T>::operator+= (mat3x3 const &mm)
	{
	    this->cv[0] += mm.cv[0];
	    this->cv[1] += mm.cv[1];
	    this->cv[2] += mm.cv[2];
	    return *this;
	}

	template <typename T>
	inline mat3x3<T> & mat3x3<T>::operator-= (T const &s)
	{
	    vec3<T> sv = vec3<T>(T(s), T(s), T(s));
	    this->cv[0] -= sv;
	    this->cv[1] -= sv;
	    this->cv[2] -= sv;
	    return *this;
	}

	template <typename T>
	inline mat3x3<T> & mat3x3<T>::operator-= (mat3x3 const &mm)
	{
	    this->cv[0] -= mm.cv[0];
	    this->cv[1] -= mm.cv[1];
	    this->cv[2] -= mm.cv[2];
	    return *this;
	}

	template <typename T>
	inline mat3x3<T> & mat3x3<T>::operator*= (T const &s)
	{
	    vec3<T> sv = vec3<T>(T(s), T(s), T(s));
	    this->cv[0] *= sv;
	    this->cv[1] *= sv;
	    this->cv[2] *= sv;
	    return *this;
	}

	template <typename T>
	inline mat3x3<T> & mat3x3<T>::operator*= (mat3x3 const &mm)
	{
	    vec3<T> r0 = vec3<T>(this->cv[0][0], this->cv[1][0], this->cv[2][0]);
	    this->cv[0][0] = dot(r0, mm.cv[0]);
	    this->cv[1][0] = dot(r0, mm.cv[1]);
	    this->cv[2][0] = dot(r0, mm.cv[2]);

	    vec3<T> r1 = vec3<T>(this->cv[0][1], this->cv[1][1], this->cv[2][1]);
	    this->cv[0][1] = dot(r1, mm.cv[0]);
	    this->cv[1][1] = dot(r1, mm.cv[1]);
	    this->cv[2][1] = dot(r1, mm.cv[2]);

	    vec3<T> r2 = vec3<T>(this->cv[0][2], this->cv[1][2], this->cv[2][2]);
	    this->cv[0][2] = dot(r2, mm.cv[0]);
	    this->cv[1][2] = dot(r2, mm.cv[1]);
	    this->cv[2][2] = dot(r2, mm.cv[2]);

	    return *this;
	}

	template <typename T>
	inline mat3x3<T> mat3x3<T>::transpose() const
	{
	    return mat3x3<T>(
		vec3<T>(this->cv[0][0], this->cv[1][0], this->cv[2][0]),
		vec3<T>(this->cv[0][1], this->cv[1][1], this->cv[2][1]),
		vec3<T>(this->cv[0][2], this->cv[1][2], this->cv[2][2]));
	}

	template <typename T>
	inline T mat3x3<T>::determinant() const
	{
	  // det M = M[0] . (M[1] x M[2]), where M[i] are column vectors
	    return dot (this->cv[0], cross(this->cv[1], this->cv[2]));
	}

	template <typename T>
	inline mat3x3<T> mat3x3<T>::inverse() const
	{
	    T s = (T(1.0) / this->determinant());
	    return mat3x3<T>(
		s * vec3<T>(this->cv[0][0], this->cv[1][0], this->cv[2][0]),
		s * vec3<T>(this->cv[0][1], this->cv[1][1], this->cv[2][1]),
		s * vec3<T>(this->cv[0][2], this->cv[1][2], this->cv[2][2]));
	}

    /********** Binary operations **********/

    // addition: scalar+matrix, matrix+scalar, matrix+matrix
	template <typename T>
	inline mat3x3<T> operator+ (T const &s, mat3x3<T> const &m)
	{
	    vec3<T> sv = vec3<T>(T(s), T(s), T(s));
	    return mat3x3<T>(sv + m.cv[0], sv + m.cv[1], sv + m.cv[2]);
	}

	template <typename T>
	inline mat3x3<T> operator+ (mat3x3<T> const &m, T const &s)
	{
	    vec3<T> sv = vec3<T>(T(s), T(s), T(s));
	    return mat3x3<T>(m.cv[0] + sv, m.cv[1] + sv, m.cv[2] + sv);
	}

	template <typename T>
	inline mat3x3<T> operator+ (mat3x3<T> const &m1, mat3x3<T> const &m2)
	{
	    return mat3x3<T>(m1.cv[0] + m2.cv[0], m1.cv[1] + m2.cv[1], m1.cv[2] + m2.cv[2]);
	}

    // subtraction: scalar-matrix, matrix-scalar, matrix-matrix
	template <typename T>
	inline mat3x3<T> operator- (T const &s, mat3x3<T> const &m)
	{
	    vec3<T> sv = vec3<T>(T(s), T(s), T(s));
	    return mat3x3<T>(sv - m.cv[0], sv - m.cv[1], sv - m.cv[2]);
	}

	template <typename T>
	inline mat3x3<T> operator- (mat3x3<T> const &m, T const &s)
	{
	    vec3<T> sv = vec3<T>(T(s), T(s), T(s));
	    return mat3x3<T>(m.cv[0] - sv, m.cv[1] - sv, m.cv[2] - sv);
	}

	template <typename T>
	inline mat3x3<T> operator- (mat3x3<T> const &m1, mat3x3<T> const &m2)
	{
	    return mat3x3<T>(m1.cv[0] - m2.cv[0], m1.cv[1] - m2.cv[1], m1.cv[2] - m2.cv[2]);
	}

    // scaling: scalar*matrix, matrix*scalar
	template <typename T>
	inline mat3x3<T> operator* (T const &s, mat3x3<T> const &m)
	{
	    vec3<T> sv = vec3<T>(T(s), T(s), T(s));
	    return mat3x3<T>(sv * m.cv[0], sv * m.cv[1], sv * m.cv[2]);
	}
	template <typename T>
	inline mat3x3<T> operator* (mat3x3<T> const &m, T const &s)
	{
	    vec3<T> sv = vec3<T>(T(s), T(s), T(s));
	    return mat3x3<T>(m.cv[0] * sv, m.cv[1] * sv, m.cv[2] * sv);
	}

    // inner product: vector*matrix, matrix*vector, matrix*matrix
	template <typename T>
	inline vec3<T> operator* (vec3<T> const &v, mat3x3<T> const &m)
	{
	    return vec3<T>(dot(v, m.cv[0]), dot(v, m.cv[1]), dot(v, m.cv[2]));
	}

	template <typename T>
	inline vec3<T> operator* (mat3x3<T> const &m, vec3<T> const &v)
	{
	    return vec3<T>(
		m.cv[0][0]*v.x + m.cv[1][0]*v.y + m.cv[2][0]*v.z,
		m.cv[0][1]*v.x + m.cv[1][1]*v.y + m.cv[2][1]*v.z,
		m.cv[0][2]*v.x + m.cv[1][2]*v.y + m.cv[2][2]*v.z);
	}

	template <typename T>
	inline mat3x3<T> operator* (mat3x3<T> const &m1, mat3x3<T> const &m2)
	{
	    mat3x3<T> result(m1);
	    result *= m2;
	    return result;
	}

      /********** Other functions **********/

      //! unary negation
	template <typename T>
	inline __detail::mat3x3<T> operator- (__detail::mat3x3<T> const &m)
	{
	    return __detail::mat3x3<T>(-m.cv[0], -m.cv[1], -m.cv[2]);
	}

      //! outer product of column vector * row vector
      //! \param cv a row vector
      //! \param rv a column vector
	template <typename T>
	inline __detail::mat3x3<T> outerProduct (__detail::vec3<T> const &cv, __detail::vec3<T> const &rv)
	{
	    return __detail::mat3x3<T>(cv * rv.x, cv * rv.y, cv * rv.z);
	}

      //! get a pointer to the matrix's data (in column-major order)
	template <typename T>
	inline T const *valuePtr (mat3x3<T> const &m)
	{
	    return &(m.cv[0].x);
	}

    } /* namespace __detail */

} /* namespace cs237 */

#endif /* !_CS237_MAT3X3_INL_ */
