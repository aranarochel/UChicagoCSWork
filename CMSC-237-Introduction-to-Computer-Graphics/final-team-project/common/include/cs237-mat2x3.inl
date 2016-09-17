/*! \file cs237-mat2x3.inl
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Inline operations on mat2x3 values.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_MAT2X3_INL_
#define _CS237_MAT2X3_INL_

#ifndef _CS237_HXX_
#error "c237-mat2x3.inl should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      /********** Class mat2x3<T> member functions **********/

      //! copy constructor
	template <typename T>
	inline mat2x3<T>::mat2x3 (mat2x3 const &mm)
	    : cv {mm.cv[0], mm.cv[1]}
	{ }

      //! construct a matrix from individual elements given in column-major order
	template <typename T>
	inline mat2x3<T>::mat2x3 (
	    T m00, T m10, T m20,	// first column
	    T m01, T m11, T m21)	// second column
	    : cv {vec3<T>(m00, m10, m20), vec3<T>(m01, m11, m21)}
	{ }

      //! construct a matrix from column vectors
	template <typename T>
	inline mat2x3<T>::mat2x3 (vec3<T> const &c0, vec3<T> const &c1)
	    : cv {c0, c1}
	{ }

      //! extract the left 2x3 matrix from a 3x3 matrix
	template <typename T>
	inline mat2x3<T>::mat2x3 (mat3x3<T> const &mm)
	    : cv {vec3<T>(mm.cv[0]), vec3<T>(mm.cv[1])}
	{ }

      //! return the i'th column vector
	template <typename T>
	inline vec3<T> & mat2x3<T>::operator[] (unsigned int const &i)
	{
	    assert (i < 2);
	    return this->cv[i];
	}

	template <typename T>
	inline vec3<T> const & mat2x3<T>::operator[] (unsigned int const &i) const
	{
	    assert (i < 2);
	    return this->cv[i];
	}

	template <typename T>
	inline mat2x3<T> & mat2x3<T>::operator= (mat2x3<T> const &mm)
	{
	    this->cv[0] = mm.cv[0];
	    this->cv[1] = mm.cv[1];
	    return *this;
	}

	template <typename T>
	inline mat2x3<T> & mat2x3<T>::operator+= (T const &s)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    this->cv[0] += sv;
	    this->cv[1] += sv;
	    return *this;
	}

	template <typename T>
	inline mat2x3<T> & mat2x3<T>::operator+= (mat2x3 const &mm)
	{
	    this->cv[0] += mm.cv[0];
	    this->cv[1] += mm.cv[1];
	    return *this;
	}

	template <typename T>
	inline mat2x3<T> & mat2x3<T>::operator-= (T const &s)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    this->cv[0] -= sv;
	    this->cv[1] -= sv;
	    return *this;
	}

	template <typename T>
	inline mat2x3<T> & mat2x3<T>::operator-= (mat2x3 const &mm)
	{
	    this->cv[0] -= mm.cv[0];
	    this->cv[1] -= mm.cv[1];
	    return *this;
	}

	template <typename T>
	inline mat2x3<T> & mat2x3<T>::operator*= (T const &s)
	{
	    vec2<T> sv = vec3<T>(T(s), T(s), T(s));
	    this->cv[0] *= sv;
	    this->cv[1] *= sv;
	    return *this;
	}

	template <typename T>
	inline mat3x2<T> mat2x3<T>::transpose() const
	{
	    return mat3x2<T>(
		this->cv[0][0], this->cv[1][0],  // first column
		this->cv[0][1], this->cv[1][1],  // second column
		this->cv[0][2], this->cv[1][2]); // third column
	}

    /********** Binary operations **********/

    // addition: scalar+matrix, matrix+scalar, matrix+matrix
	template <typename T>
	inline mat2x3<T> operator+ (T const &s, mat2x3<T> const &m)
	{
	    vec3<T> sv = vec3<T>(T(s), T(s), T(s));
	    return mat2x3<T>(sv + m.cv[0], sv + m.cv[1]);
	}

	template <typename T>
	inline mat2x3<T> operator+ (mat2x3<T> const &m, T const &s)
	{
	    vec3<T> sv = vec3<T>(T(s), T(s), T(s));
	    return mat2x3<T>(m.cv[0] + sv, m.cv[1] + sv);
	}

	template <typename T>
	inline mat2x3<T> operator+ (mat2x3<T> const &m1, mat2x3<T> const &m2)
	{
	    return mat2x3<T>(m1.cv[0] + m2.cv[0], m1.cv[1] + m2.cv[1]);
	}

    // subtraction: scalar-matrix, matrix-scalar, matrix-matrix
	template <typename T>
	inline mat2x3<T> operator- (T const &s, mat2x3<T> const &m)
	{
	    vec3<T> sv = vec3<T>(T(s), T(s), T(s));
	    return mat2x3<T>(sv - m.cv[0], sv - m.cv[1]);
	}

	template <typename T>
	inline mat2x3<T> operator- (mat2x3<T> const &m, T const &s)
	{
	    vec3<T> sv = vec3<T>(T(s), T(s), T(s));
	    return mat2x3<T>(m.cv[0] - sv, m.cv[1] - sv);
	}

	template <typename T>
	inline mat2x3<T> operator- (mat2x3<T> const &m1, mat2x3<T> const &m2)
	{
	    return mat2x3<T>(m1.cv[0] - m2.cv[0], m1.cv[1] - m2.cv[1]);
	}

    // scaling: scalar*matrix, matrix*scalar
	template <typename T>
	inline mat2x3<T> operator* (T const &s, mat2x3<T> const &m)
	{
	    vec3<T> sv = vec3<T>(T(s), T(s), T(s));
	    return mat2x3<T>(sv * m.cv[0], sv * m.sv[1]);
	}

	template <typename T>
	inline mat2x3<T> operator* (mat2x3<T> const &m, T const &s)
	{
	    vec3<T> sv = vec3<T>(T(s), T(s), T(s));
	    return mat2x3<T>(m.cv[0] * sv, m.cv[1] * sv);
	}

    // inner product: vector*matrix, matrix*vector, matrix*matrix
	template <typename T>
	inline vec2<T> operator* (vec3<T> const &v, mat2x3<T> const &m)
	{
	    return vec2<T>(dot(v, m.cv[0]), dot(v, m.cv[1]));
	}

	template <typename T>
	inline vec3<T> operator* (mat2x3<T> const &m, vec2<T> const &v)
	{
	    return vec2<T>(
		m.cv[0][0]*v.x + m.cv[1][0]*v.y,
		m.cv[0][1]*v.x + m.cv[1][1]*v.y,
		m.cv[0][2]*v.x + m.cv[1][2]*v.y);
	}

	/* TODO: various forms of matrix times matrix multiplication */

      /***** Other functions *****/

      //! unary negation
	template <typename T>
	inline mat2x3<T> operator- (mat2x3<T> const &m)
	{
	    return mat2x3<T>(-m.cv[0], -m.cv[1]);
	}

      //! outer product of column vector * row vector
      //! \param cv a row vector
      //! \param rv a column vector
	template <typename T>
	inline mat2x3<T> outerProduct (vec3<T> const &cv, vec2<T> const &rv)
	{
	    return mat2x3<T>(
		cv[0]*rv[0], cv[1]*rv[0], cv[2]*rv[0],  // first column
		cv[0]*rv[1], cv[1]*rv[1], cv[2]*rv[1]); // second column
	}

      //! get a pointer to the matrix's data (in column-major order)
	template <typename T>
	inline T const *valuePtr (mat2x3<T> const &m)
	{
	    return &(m.cv[0].x);
	}

    } /* namespace __detail */

} /* namespace cs237 */

#endif /* !_CS237_MAT2X3_INL_ */
