/*! \file cs237-mat3x2.inl
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Inline operations on mat3x2 values.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_MAT3X2_INL_
#define _CS237_MAT3X2_INL_

#ifndef _CS237_HXX_
#error "c237-mat3x2.inl should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      /********** Class mat3x2<T> member functions **********/

      // copy constructor
	template <typename T>
	inline mat3x2<T>::mat3x2 (mat3x2 const &mm)
	    : cv {mm.cv[0], mm.cv[1], mm.cv[2]}
	{ }

      // construct a matrix from individual elements given in column-major order
	template <typename T>
	inline mat3x2<T>::mat3x2 (
	    T m00, T m10,	// first column
	    T m01, T m11,	// second column
	    T m02, T m12)	// third column
	    : cv {vec2<T>(m00, m10), vec2<T>(m01, m11), vec2<T>(m02, m12)}
	{ }

      // construct a matrix from column vectors
	template <typename T>
	inline mat3x2<T>::mat3x2 (vec2<T> const &c0, vec2<T> const &c1, vec2<T> const &c2)
	    : cv {c0, c1, c2}
	{ }

      // extract the upper 3x2 matrix from a 3x3 matrix
	template <typename T>
	inline mat3x2<T>::mat3x2 (mat3x3<T> const &mm)
	    : cv {vec2<T>(mm.cv[0]), vec2<T>(mm.cv[1]), vec2<T>(mm.cv[2])}
	{ }

      //! return the i'th column vector
	template <typename T>
	inline vec2<T> & mat3x2<T>::operator[] (unsigned int const &i)
	{
	    assert (i < 3);
	    return this->cv[i];
	}

	template <typename T>
	inline vec2<T> const & mat3x2<T>::operator[] (unsigned int const &i) const
	{
	    assert (i < 3);
	    return this->cv[i];
	}

	template <typename T>
	inline mat3x2<T> & mat3x2<T>::operator= (mat3x2<T> const &mm)
	{
	    this->cv[0] = mm.cv[0];
	    this->cv[1] = mm.cv[1];
	    this->cv[2] = mm.cv[2];
	    return *this;
	}

	template <typename T>
	inline mat3x2<T> & mat3x2<T>::operator+= (T const &s)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    this->cv[0] += sv;
	    this->cv[1] += sv;
	    this->cv[2] += sv;
	    return *this;
	}

	template <typename T>
	inline mat3x2<T> & mat3x2<T>::operator+= (mat3x2 const &mm)
	{
	    this->cv[0] += mm.cv[0];
	    this->cv[1] += mm.cv[1];
	    this->cv[2] += mm.cv[1];
	    return *this;
	}

	template <typename T>
	inline mat3x2<T> & mat3x2<T>::operator-= (T const &s)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    this->cv[0] -= sv;
	    this->cv[1] -= sv;
	    this->cv[2] -= sv;
	    return *this;
	}

	template <typename T>
	inline mat3x2<T> & mat3x2<T>::operator-= (mat3x2 const &mm)
	{
	    this->cv[0] -= mm.cv[0];
	    this->cv[1] -= mm.cv[1];
	    this->cv[2] -= mm.cv[1];
	    return *this;
	}

	template <typename T>
	inline mat3x2<T> & mat3x2<T>::operator*= (T const &s)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    this->cv[0] *= sv;
	    this->cv[1] *= sv;
	    this->cv[2] *= sv;
	    return *this;
	}

	template <typename T>
	inline mat2x3<T> mat3x2<T>::transpose() const
	{
	    return mat2x3<T>(
		vec3<T>(this->cv[0][0], this->cv[1][0], this->cv[2][0]),
		vec3<T>(this->cv[0][1], this->cv[1][1], this->cv[2][1]));
	}

    /********** Binary operations **********/

    // addition: scalar+matrix, matrix+scalar, matrix+matrix
	template <typename T>
	inline mat3x2<T> operator+ (T const &s, mat3x2<T> const &m)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    return mat3x2<T>(sv + m.cv[0], sv + m.cv[1], sv + m.cv[2]);
	}

	template <typename T>
	inline mat3x2<T> operator+ (mat3x2<T> const &m, T const &s)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    return mat3x2<T>(m.cv[0] + sv, m.cv[1] + sv, m.cv[2] + sv);
	}

	template <typename T>
	inline mat3x2<T> operator+ (mat3x2<T> const &m1, mat3x2<T> const &m2)
	{
	    return mat3x2<T>(m1.cv[0] + m2.cv[0], m1.cv[1] + m2.cv[1], m1.cv[2] + m2.cv[2]);
	}

    // subtraction: scalar-matrix, matrix-scalar, matrix-matrix
	template <typename T>
	inline mat3x2<T> operator- (T const &s, mat3x2<T> const &m)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    return mat3x2<T>(sv - m.cv[0], sv - m.cv[1], sv - m.cv[2]);
	}

	template <typename T>
	inline mat3x2<T> operator- (mat3x2<T> const &m, T const &s)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    return mat3x2<T>(m.cv[0] - sv, m.cv[1] - sv, m.cv[2] - sv);
	}

	template <typename T>
	inline mat3x2<T> operator- (mat3x2<T> const &m1, mat3x2<T> const &m2)
	{
	    return mat3x2<T>(m1.cv[0] - m2.cv[0], m1.cv[1] - m2.cv[1]), m1.cv[2] - m2.cv[2];
	}

    // scaling: scalar*matrix, matrix*scalar
	template <typename T>
	inline mat3x2<T> operator* (T const &s, mat3x2<T> const &m)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    return mat3x2<T>(sv * m.cv[0], sv * m.cv[1], sv * m.cv[2]);
	}

	template <typename T>
	inline mat3x2<T> operator* (mat3x2<T> const &m, T const &s)
	{
	    vec2<T> sv = vec2<T>(T(s), T(s));
	    return mat3x2<T>(m.cv[0] * sv, m.cv[1] * sv, m.cv[2] * sv);
	}

    // inner product: vector*matrix, matrix*vector, matrix*matrix
	template <typename T>
	inline vec3<T> operator* (vec2<T> const &v, mat3x2<T> const &m)
	{
	    return vec2<T>(dot(v, m.cv[0]), dot(v, m.cv[1]), dot(v, m.cv[2]));
	}

	template <typename T>
	inline vec2<T> operator* (mat3x2<T> const &m, vec3<T> const &v)
	{
	    return vec2<T>(
		m.cv[0][0]*v.x + m.cv[1][0]*v.y + m.cv[2][0]*v.z,
		m.cv[0][1]*v.x + m.cv[1][1]*v.y + m.cv[2][1]*v.z);
	}

    //! multiply a 2x2 matrix times a 3x2 matrix, producing a 3x2 matrix
	template <typename T>
	inline mat3x2<T> operator* (mat2x2<T> const &m1, mat3x2<T> const &m2)
	{
	    vec2<T> r1(m1[0][0], m1[1][0]); // first row of m1
	    vec2<T> r2(m1[0][1], m1[1][1]); // second row of m1
	    return mat3x2<T>(
		dot(r1, m2.cv[0]), dot(r2, m2.cv[0]),  // first column
		dot(r1, m2.cv[1]), dot(r2, m2.cv[1]),  // second column
		dot(r1, m2.cv[2]), dot(r2, m2.cv[2])); // third column
	}

    //! multiply a 3x2 matrix times a 3x3 matrix, producing a 3x2 matrix
	template <typename T>
	inline mat3x2<T> operator* (mat3x2<T> const &m1, mat3x3<T> const &m2)
	{
	    vec3<T> r1(m1[0][0], m1[1][0], m1[2][0]); // first row of m1
	    vec3<T> r2(m1[0][1], m1[1][1], m1[2][1]); // second row of m1
	    return mat3x2<T>(
		dot(r1, m2.cv[0]), dot(r2, m2.cv[0]),  // first column
		dot(r1, m2.cv[1]), dot(r2, m2.cv[1]),  // second column
		dot(r1, m2.cv[2]), dot(r2, m2.cv[2])); // third column
	}

      /***** Other functions *****/

      //! unary negation
	template <typename T>
	inline mat3x2<T> operator- (mat3x2<T> const &m)
	{
	    return mat3x2<T>(-m.cv[0], -m.cv[1], -m.cv[2]);
	}

      //! outer product of column vector * row vector
      //! \param cv a row vector
      //! \param rv a column vector
	template <typename T>
	inline mat3x2<T> outerProduct (vec2<T> const &cv, vec3<T> const &rv)
	{
	    return mat3x2<T>(
		cv[0]*rv[0], cv[1]*rv[0],  // first column
		cv[0]*rv[1], cv[1]*rv[1],  // second column
		cv[0]*rv[2], cv[1]*rv[2]); // third column
	}

      //! get a pointer to the matrix's data (in column-major order)
	template <typename T>
	inline T const *valuePtr (mat3x2<T> const &m)
	{
	    return &(m.cv[0].x);
	}

    } /* namespace __detail */

} /* namespace cs237 */

#endif /* !_CS237_MAT3X2_INL_ */
