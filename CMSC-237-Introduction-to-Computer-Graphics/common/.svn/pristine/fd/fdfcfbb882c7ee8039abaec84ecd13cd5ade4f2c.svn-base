/*! \file cs237-mat4x4.inl
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Inline operations on mat4x4 values.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_MAT4X4_INL_
#define _CS237_MAT4X4_INL_

#ifndef _CS237_HXX_
#error "c237-mat4x4.inl should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      /********** Class mat4x4<T> member functions **********/

      //! copy constructor
	template <typename T>
	inline mat4x4<T>::mat4x4 (mat4x4 const &mm)
	    : cv {mm.cv[0], mm.cv[1], mm.cv[2], mm.cv[3]}
	{ }

      //! construct a matrix from individual elements given in column-major order
	template <typename T>
	inline mat4x4<T>::mat4x4 (
	    T m00, T m10, T m20, T m30,	// first column
	    T m01, T m11, T m21, T m31,	// second column
	    T m02, T m12, T m22, T m32,	// third column
	    T m03, T m13, T m23, T m33)	// fourth column
	    : cv {
		vec4<T>(m00, m10, m20, m30),
		vec4<T>(m01, m11, m21, m31),
		vec4<T>(m02, m12, m22, m32),
		vec4<T>(m03, m13, m23, m33)}
	{ }

      //! construct a matrix from column vectors
	template <typename T>
	inline mat4x4<T>::mat4x4 (vec4<T> const &c0, vec4<T> const &c1, vec4<T> const &c2, vec4<T> const &c3)
	    : cv {c0, c1, c2, c3}
	{ }

      //! construct a diagonal matrix with diagonal elements initialized to x
	template <typename T>
	inline mat4x4<T>::mat4x4 (T const &x)
	    : cv {
		vec4<T>(x, T(0), T(0), T(0)),
		vec4<T>(T(0), x, T(0), T(0)),
		vec4<T>(T(0), T(0), x, T(0)),
		vec4<T>(T(0), T(0), T(0), x)}
	{ }

      //! return the i'th column vector
	template <typename T>
	inline vec4<T> & mat4x4<T>::operator[] (unsigned int const &i)
	{
	    assert (i < 4);
	    return this->cv[i];
	}

	template <typename T>
	inline vec4<T> const & mat4x4<T>::operator[] (unsigned int const &i) const
	{
	    assert (i < 4);
	    return this->cv[i];
	}

	template <typename T>
	inline mat4x4<T> & mat4x4<T>::operator= (mat4x4<T> const &mm)
	{
	    this->cv[0] = mm.cv[0];
	    this->cv[1] = mm.cv[1];
	    this->cv[2] = mm.cv[2];
	    this->cv[3] = mm.cv[3];
	    return *this;
	}

	template <typename T>
	inline mat4x4<T> & mat4x4<T>::operator+= (T const &s)
	{
	    vec4<T> sv = vec4<T>(T(s), T(s), T(s), T(s));
	    this->cv[0] += sv;
	    this->cv[1] += sv;
	    this->cv[2] += sv;
	    this->cv[3] += sv;
	    return *this;
	}

	template <typename T>
	inline mat4x4<T> & mat4x4<T>::operator+= (mat4x4 const &mm)
	{
	    this->cv[0] += mm.cv[0];
	    this->cv[1] += mm.cv[1];
	    this->cv[2] += mm.cv[2];
	    this->cv[3] += mm.cv[3];
	    return *this;
	}

	template <typename T>
	inline mat4x4<T> & mat4x4<T>::operator-= (T const &s)
	{
	    vec4<T> sv = vec4<T>(T(s), T(s), T(s), T(s));
	    this->cv[0] -= sv;
	    this->cv[1] -= sv;
	    this->cv[2] -= sv;
	    this->cv[3] -= sv;
	    return *this;
	}

	template <typename T>
	inline mat4x4<T> & mat4x4<T>::operator-= (mat4x4 const &mm)
	{
	    this->cv[0] -= mm.cv[0];
	    this->cv[1] -= mm.cv[1];
	    this->cv[2] -= mm.cv[2];
	    this->cv[3] -= mm.cv[3];
	    return *this;
	}

	template <typename T>
	inline mat4x4<T> & mat4x4<T>::operator*= (T const &s)
	{
	    vec4<T> sv = vec4<T>(T(s), T(s), T(s), T(s));
	    this->cv[0] *= sv;
	    this->cv[1] *= sv;
	    this->cv[2] *= sv;
	    this->cv[3] *= sv;
	    return *this;
	}

	template <typename T>
	inline mat4x4<T> & mat4x4<T>::operator*= (mat4x4 const &mm)
	{
	    vec4<T> r0 = vec4<T>(this->cv[0][0], this->cv[1][0], this->cv[2][0], this->cv[3][0]);
	    this->cv[0][0] = dot(r0, mm.cv[0]);
	    this->cv[1][0] = dot(r0, mm.cv[1]);
	    this->cv[2][0] = dot(r0, mm.cv[2]);
	    this->cv[3][0] = dot(r0, mm.cv[3]);

	    vec4<T> r1 = vec4<T>(this->cv[0][1], this->cv[1][1], this->cv[2][1], this->cv[3][1]);
	    this->cv[0][1] = dot(r1, mm.cv[0]);
	    this->cv[1][1] = dot(r1, mm.cv[1]);
	    this->cv[2][1] = dot(r1, mm.cv[2]);
	    this->cv[3][1] = dot(r1, mm.cv[3]);

	    vec4<T> r2 = vec4<T>(this->cv[0][2], this->cv[1][2], this->cv[2][2], this->cv[3][2]);
	    this->cv[0][2] = dot(r2, mm.cv[0]);
	    this->cv[1][2] = dot(r2, mm.cv[1]);
	    this->cv[2][2] = dot(r2, mm.cv[2]);
	    this->cv[3][2] = dot(r2, mm.cv[3]);

	    vec4<T> r3 = vec4<T>(this->cv[0][3], this->cv[1][3], this->cv[2][3], this->cv[3][3]);
	    this->cv[0][3] = dot(r3, mm.cv[0]);
	    this->cv[1][3] = dot(r3, mm.cv[1]);
	    this->cv[2][3] = dot(r3, mm.cv[2]);
	    this->cv[3][3] = dot(r3, mm.cv[3]);

	    return *this;
	}

	template <typename T>
	inline mat4x4<T> mat4x4<T>::transpose() const
	{
	    return mat4x4<T>(
		vec4<T>(this->cv[0][0], this->cv[1][0], this->cv[2][0], this->cv[3][0]),
		vec4<T>(this->cv[0][1], this->cv[1][1], this->cv[2][1], this->cv[3][1]),
		vec4<T>(this->cv[0][2], this->cv[1][2], this->cv[2][2], this->cv[3][2]),
		vec4<T>(this->cv[0][3], this->cv[1][3], this->cv[2][3], this->cv[3][3]));
	}


    /********** Binary operations **********/

    // addition: scalar+matrix, matrix+scalar, matrix+matrix
	template <typename T>
	inline mat4x4<T> operator+ (T const &s, mat4x4<T> const &m)
	{
	    vec4<T> sv = vec4<T>(T(s), T(s), T(s), T(s));
	    return mat4x4<T>(sv + m.cv[0], sv + m.cv[1], sv + m.cv[2], sv + m.cv[3]);
	}

	template <typename T>
	inline mat4x4<T> operator+ (mat4x4<T> const &m, T const &s)
	{
	    vec4<T> sv = vec4<T>(T(s), T(s), T(s), T(s));
	    return mat4x4<T>(m.cv[0] + sv, m.cv[1] + sv, m.cv[2] + sv, m.cv[3] + sv);
	}

	template <typename T>
	inline mat4x4<T> operator+ (mat4x4<T> const &m1, mat4x4<T> const &m2)
	{
	    return mat4x4<T>(m1.cv[0] + m2.cv[0], m1.cv[1] + m2.cv[1], m1.cv[2] + m2.cv[2], m1.cv[3] + m2.cv[3]);
	}

    // subtraction: scalar-matrix, matrix-scalar, matrix-matrix
	template <typename T>
	inline mat4x4<T> operator- (T const &s, mat4x4<T> const &m)
	{
	    vec4<T> sv = vec4<T>(T(s), T(s), T(s), T(s));
	    return mat4x4<T>(sv - m.cv[0], sv - m.cv[1], sv - m.cv[2], sv - m.cv[3]);
	}

	template <typename T>
	inline mat4x4<T> operator- (mat4x4<T> const &m, T const &s)
	{
	    vec4<T> sv = vec4<T>(T(s), T(s), T(s), T(s));
	    return mat4x4<T>(m.cv[0] - sv, m.cv[1] - sv, m.cv[2] - sv, m.cv[3] - sv);
	}

	template <typename T>
	inline mat4x4<T> operator- (mat4x4<T> const &m1, mat4x4<T> const &m2)
	{
	    return mat4x4<T>(m1.cv[0] - m2.cv[0], m1.cv[1] - m2.cv[1], m1.cv[2] - m2.cv[2], m1.cv[3] - m2.cv[3]);
	}

    // scaling: scalar*matrix, matrix*scalar
	template <typename T>
	inline mat4x4<T> operator* (T const &s, mat4x4<T> const &m)
	{
	    vec4<T> sv = vec4<T>(T(s), T(s), T(s), T(s));
	    return mat4x4<T>(sv * m.cv[0], sv * m.cv[1], sv * m.cv[2], sv * m.cv[3]);
	}
	template <typename T>
	inline mat4x4<T> operator* (mat4x4<T> const &m, T const &s)
	{
	    vec4<T> sv = vec4<T>(T(s), T(s), T(s), T(s));
	    return mat4x4<T>(m.cv[0] * sv, m.cv[1] * sv, m.cv[2] * sv, m.cv[3] * sv);
	}

    // inner product: vector*matrix, matrix*vector, matrix*matrix
	template <typename T>
	inline vec4<T> operator* (vec4<T> const &v, mat4x4<T> const &m)
	{
	    return vec4<T>(dot(v, m.cv[0]), dot(v, m.cv[1]), dot(v, m.cv[2]), dot(v, m.cv[3]));
	}

	template <typename T>
	inline vec4<T> operator* (mat4x4<T> const &m, vec4<T> const &v)
	{
	    return vec4<T>(
		m.cv[0][0]*v.x + m.cv[1][0]*v.y + m.cv[2][0]*v.z + m.cv[3][0]*v.w,
		m.cv[0][1]*v.x + m.cv[1][1]*v.y + m.cv[2][1]*v.z + m.cv[3][1]*v.w,
		m.cv[0][2]*v.x + m.cv[1][2]*v.y + m.cv[2][2]*v.z + m.cv[3][2]*v.w,
		m.cv[0][3]*v.x + m.cv[1][3]*v.y + m.cv[2][3]*v.z + m.cv[3][3]*v.w);
	}

	template <typename T>
	inline mat4x4<T> operator* (mat4x4<T> const &m1, mat4x4<T> const &m2)
	{
	    mat4x4<T> result(m1);
	    result *= m2;
	    return result;
	}

      /********** Other functions **********/

      //! unary negation
	template <typename T>
	inline mat4x4<T> operator- (mat4x4<T> const &m)
	{
	    return mat4x4<T>(-m.cv[0], -m.cv[1], -m.cv[2], -m.cv[3]);
	}

      //! outer product of column vector * row vector
      //! \param cv a row vector
      //! \param rv a column vector
	template <typename T>
	inline mat4x4<T> outerProduct (vec4<T> const &cv, vec4<T> const &rv)
	{
	    return mat4x4<T>(cv * rv.x, cv * rv.y, cv * rv.z, cv * rv.w);
	}

      //! get a pointer to the matrix's data (in column-major order)
	template <typename T>
	inline T const *valuePtr (mat4x4<T> const &m)
	{
	    return &(m.cv[0].x);
	}

    } /* namespace __detail */

} /* namespace cs237 */

#endif /* !_CS237_MAT4X4_INL_ */
