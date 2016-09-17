/*! \file cs237-mat4x4.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_MAT4X4_HXX_
#define _CS237_MAT4X4_HXX_

#ifndef _CS237_HXX_
#error "c237-mat4x4.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      //! template class for 4x4 matrices
	template <typename T>
	struct mat4x4 {
	    vec4<T> cv[4];	// column vectors

	  //! create an uninitialized matrix
	    mat4x4 () {}
	  //! copy constructor
	    mat4x4 (mat4x4 const &m);
	  //! construct a matrix from individual elements given in column-major order
	    explicit mat4x4 (
		T m00, T m10, T m20, T m30,	// first column
		T m01, T m11, T m21, T m31,	// second column
		T m02, T m12, T m22, T m32,	// third column
		T m03, T m13, T m23, T m33);	// fourth column
	  //! construct a matrix from column vectors
	    explicit mat4x4 (vec4<T> const &c0, vec4<T> const &c1, vec4<T> const &c2, vec4<T> const &c3);
	  //! construct a diagonal matrix with diagonal elements initialized to x
	    explicit mat4x4 (T const &x);

	  ///@{
	  //! return the i'th column vector of the matrix
	    vec4<T> & operator[] (unsigned int const &i);
	    vec4<T> const & operator[] (unsigned int const &i) const;
	  ///@}

	    mat4x4 & operator= (mat4x4 const &mm);

	    mat4x4 & operator+= (T const &s);
	    mat4x4 & operator+= (mat4x4 const &mm);

	    mat4x4 & operator-= (T const &s);
	    mat4x4 & operator-= (mat4x4 const &mm);

	    mat4x4 & operator*= (T const &s);
	    mat4x4 & operator*= (mat4x4 const &mm);

	  //! return the inverse of this matrix
	    mat4x4 inverse() const;

	  //! return the transpose of this matrix
	    mat4x4 transpose() const;

	  //! return the determiniant of this matrix
	    T determinant() const;

	  //! extract a 3x3 "normal" transform matrix from a 4x4 affine transform matrix, which
	  //! is the inverse transpose of the upper-left 3x3 submatrix.
	    mat3x3<T> normalMatrix() const;
	};

	//! \brief output the matrix to a stream as a sequence of column vectors
	//! \param s the stream to print to
	//! \param m the matrix to print
	template <typename T>
	std::ostream& operator<< (std::ostream& s, mat4x4<T> const &m);

    } /* _namespace __detail */

  //! Single-precision 4x4 matrices
    typedef __detail::mat4x4<float> mat4x4f;
  //! Double-precision 4x4 matrices
    typedef __detail::mat4x4<double> mat4x4d;
  //! Single-precision 4x4 matrices (the same as \ref mat4x4f)
    typedef mat4x4f mat4f;
  //! Double-precision 4x4 matrices (the same as \ref mat4x4d)
    typedef mat4x4d mat4d;

} /* namespace cs237 */

#endif /* !_CS237_MAT4X4_HXX_ */
