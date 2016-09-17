/*! \file cs237-mat2x2.inl
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Inline operations on mat2x2 values.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_MAT2X2_INL_
#define _CS237_MAT2X2_INL_

#ifndef _CS237_HXX_
#error "c237-mat2x2.inl should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      /********** Class mat2x2<T> member functions **********/

      //! copy constructor
	template <typename T>
	inline mat2x2<T>::mat2x2 (mat2x2 const &mm)
	    : cv {mm.cv[0], mm.cv[1]}
	{ }

      //! construct a matrix from individual elements given in column-major order
	template <typename T>
	inline mat2x2<T>::mat2x2 (
	    T m00, T m10,	// first column
	    T m01, T m11)	// second column
	    : cv {vec2<T>(m00, m10), vec2<T>(m01, m11)}
	{ }

      //! construct a matrix from column vectors
	template <typename T>
	inline mat2x2<T>::mat2x2 (vec2<T> const &c0, vec2<T> const &c1)
	    : cv {c0, c1}
	{ }

      //! extract the upper-left 2x2 matrix from a 2x3 matrix
	template <typename T>
	inline mat2x2<T>::mat2x2 (mat2x3<T> const &mm)
	    : cv {vec2<T>(mm.cv[0]), vec2<T>(mm.cv[1])}
	{ }

      //! extract the upper-left 2x2 matrix from a 3x2 matrix
	template <typename T>
	inline mat2x2<T>::mat2x2 (mat3x2<T> const &mm)
	    : cv {mm.cv[0], mm.cv[1]}
	{ }

      //! extract the upper-left 2x2 matrix from a 3x3 matrix
	template <typename T>
	inline mat2x2<T>::mat2x2 (mat3x3<T> const &mm)
	    : cv {vec2<T>(mm.cv[0]), vec2<T>(mm.cv[1])}
	{ }

      //! construct a diagonal matrix with diagonal elements initialized to x
	template <typename T>
	inline mat2x2<T>::mat2x2 (T const &x)
	    : cv {vec2<T>(x, T(0)), vec2<T>(T(0), x)}
	{ }

      //! return the i'th column vector
	template <typename T>
	inline vec2<T> & mat2x2<T>::operator[] (unsigned int const &i)
	{
	    assert (i < 2);
	    return this->cv[i];
	}

	template <typename T>
	inline vec2<T> const & mat2x2<T>::operator[] (unsigned int const &i) const
	{
	    assert (i < 2);
	    return this->cv[i];
	}

	template <typename T>
	inline mat2x2<T> & mat2x2<T>::operator= (mat2x2<T> const &mm)
	{
	    this->cv[0] = mm.cv[0];
	    this->cv[1] = mm.cv[1];
	    return *this;
	}

	template <typename T>
	inline mat2x2<T> & mat2x2<T>::operator+= (T const &s)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    this->cv[0] += sv;
	    this->cv[1] += sv;
	    return *this;
	}

	template <typename T>
	inline mat2x2<T> & mat2x2<T>::operator+= (mat2x2 const &mm)
	{
	    this->cv[0] += mm.cv[0];
	    this->cv[1] += mm.cv[1];
	    return *this;
	}

	template <typename T>
	inline mat2x2<T> & mat2x2<T>::operator-= (T const &s)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    this->cv[0] -= sv;
	    this->cv[1] -= sv;
	    return *this;
	}

	template <typename T>
	inline mat2x2<T> & mat2x2<T>::operator-= (mat2x2 const &mm)
	{
	    this->cv[0] -= mm.cv[0];
	    this->cv[1] -= mm.cv[1];
	    return *this;
	}

	template <typename T>
	inline mat2x2<T> & mat2x2<T>::operator*= (T const &s)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    this->cv[0] *= sv;
	    this->cv[1] *= sv;
	    return *this;
	}

	template <typename T>
	inline mat2x2<T> & mat2x2<T>::operator*= (mat2x2 const &mm)
	{
	    vec2<T> r0 = vec2<T>(this->cv[0][0], this->cv[1][0]);
	    this->cv[0][0] = dot(r0, mm.cv[0]);
	    this->cv[1][0] = dot(r0, mm.cv[1]);

	    vec2<T> r1 = vec2<T>(this->cv[0][1], this->cv[1][1]);
	    this->cv[0][1] = dot(r1, mm.cv[0]);
	    this->cv[1][1] = dot(r1, mm.cv[1]);

	    return *this;
	}

	template <typename T>
	inline mat2x2<T> mat2x2<T>::transpose() const
	{
	    return mat2x2<T>(
		vec2<T>(this->cv[0][0], this->cv[1][0]),
		vec2<T>(this->cv[0][1], this->cv[1][1]));
	}

	template <typename T>
	inline T mat2x2<T>::determinant() const
	{
	    return (this->cv[0][0]*this->cv[1][1] - this->cv[0][1]*this->cv[1][0]);
	}

	template <typename T>
	inline mat2x2<T> mat2x2<T>::inverse() const
	{
	    T s = (T(1.0) / this->determinant());
	    return mat2x2<T>(
		 s*this->cv[1][1], -s*this->cv[0][1],
		-s*this->cv[1][0],  s*this->cv[0][0]);
	}

    /********** Binary operations **********/

    // addition: scalar+matrix, matrix+scalar, matrix+matrix
	template <typename T>
	inline mat2x2<T> operator+ (T const &s, mat2x2<T> const &m)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    return mat2x2<T>(sv + m.cv[0], sv + m.cv[1]);
	}

	template <typename T>
	inline mat2x2<T> operator+ (mat2x2<T> const &m, T const &s)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    return mat2x2<T>(m.cv[0] + sv, m.cv[1] + sv);
	}

	template <typename T>
	inline mat2x2<T> operator+ (mat2x2<T> const &m1, mat2x2<T> const &m2)
	{
	    return mat2x2<T>(m1.cv[0] + m2.cv[0], m1.cv[1] + m2.cv[1]);
	}

    // subtraction: scalar-matrix, matrix-scalar, matrix-matrix
	template <typename T>
	inline mat2x2<T> operator- (T const &s, mat2x2<T> const &m)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    return mat2x2<T>(sv - m.cv[0], sv - m.cv[1]);
	}

	template <typename T>
	inline mat2x2<T> operator- (mat2x2<T> const &m, T const &s)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    return mat2x2<T>(m.cv[0] - sv, m.cv[1] - sv);
	}

	template <typename T>
	inline mat2x2<T> operator- (mat2x2<T> const &m1, mat2x2<T> const &m2)
	{
	    return mat2x2<T>(m1.cv[0] - m2.cv[0], m1.cv[1] - m2.cv[1]);
	}

    // scaling: scalar*matrix, matrix*scalar
	template <typename T>
	inline mat2x2<T> operator* (T const &s, mat2x2<T> const &m)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    return mat2x2<T>(sv * m.cv[0], sv * m.sv[1]);
	}

	template <typename T>
	inline mat2x2<T> operator* (mat2x2<T> const &m, T const &s)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    return mat2x2<T>(m.cv[0] * sv, m.cv[1] * sv);
	}

    // inner product: vector*matrix, matrix*vector, matrix*matrix
	template <typename T>
	inline vec2<T> operator* (vec2<T> const &v, mat2x2<T> const &m)
	{
	    return vec2<T>(dot(v, m.cv[0]), dot(v, m.cv[1]));
	}

	template <typename T>
	inline vec2<T> operator* (mat2x2<T> const &m, vec2<T> const &v)
	{
	    return vec2<T>(
		m.cv[0][0]*v.x + m.cv[1][0]*v.y,
		m.cv[0][1]*v.x + m.cv[1][1]*v.y);
	}

	template <typename T>
	inline mat2x2<T> operator* (mat2x2<T> const &m1, mat2x2<T> const &m2)
	{
	    mat2x2<T> result(m1);
	    result *= m2;
	    return result;
	}

      /***** Other functions *****/

      //! unary negation
	template <typename T>
	inline mat2x2<T> operator- (mat2x2<T> const &m)
	{
	    return mat2x2<T>(-m.cv[0], -m.cv[1]);
	}

      //! outer product of column vector * row vector
      //! \param cv a row vector
      //! \param rv a column vector
	template <typename T>
	inline mat2x2<T> outerProduct (vec2<T> const &cv, vec2<T> const &rv)
	{
	    return mat2x2<T>(cv * rv.x, cv * rv.y);
	}

      //! get a pointer to the matrix's data (in column-major order)
	template <typename T>
	inline T const *valuePtr (mat2x2<T> const &m)
	{
	    return &(m.cv[0].x);
	}

    } /* namespace __detail */

} /* namespace cs237 */

#endif /* !_CS237_MAT2X2_INL_ */
