/*! \file cs237-mat3x3.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_MAT3X3_HXX_
#define _CS237_MAT3X3_HXX_

#ifndef _CS237_HXX_
#error "c237-mat3x3.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      //! template class for 3x3 matrices
	template <typename T>
	struct mat3x3 {
	    vec3<T> cv[3];	// column vectors

	  //! create an uninitialized matrix
	    mat3x3 () {}
	  //! copy constructor
	    mat3x3 (mat3x3 const &m);
	  //! construct a matrix from individual elements given in column-major order
	    explicit mat3x3 (
		T m00, T m10, T m20,	// first column
		T m01, T m11, T m21,	// second column
		T m02, T m12, T m22);	// third column
	  //! construct a matrix from column vectors
	    explicit mat3x3 (vec3<T> const &c0, vec3<T> const &c1, vec3<T> const &c2);
	  //! extract the upper-left 3x3 matrix from a 4x4 matrix
	    explicit mat3x3 (mat4x4<T> const &mm);
	  //! extract the upper-left 3x3 matrix from a 3x4 matrix
	    explicit mat3x3 (mat3x4<T> const &mm);
	  //! extract the upper-left 3x3 matrix from a 4x3 matrix
	    explicit mat3x3 (mat4x3<T> const &mm);
	  //! construct a diagonal matrix with diagonal elements initialized to x
	    explicit mat3x3 (T const &x);

	  ///@{
	  //! return the i'th column vector of the matrix
	    vec3<T> & operator[] (unsigned int const &i);
	    vec3<T> const & operator[] (unsigned int const &i) const;
	  ///@}

	    mat3x3 & operator= (mat3x3 const &mm);

	    mat3x3 & operator+= (T const &s);
	    mat3x3 & operator+= (mat3x3 const &mm);

	    mat3x3 & operator-= (T const &s);
	    mat3x3 & operator-= (mat3x3 const &mm);

	    mat3x3 & operator*= (T const &s);
	    mat3x3 & operator*= (mat3x3 const &mm);

	  //! return the inverse of this matrix
	    mat3x3 inverse() const;

	  //! return the transpose of this matrix
	    mat3x3 transpose() const;

	  //! return the determiniant of this matrix
	    T determinant() const;
	};

	//! \brief output the matrix to a stream as a sequence of column vectors
	//! \param s the stream to print to
	//! \param m the matrix to print
	template <typename T>
	std::ostream& operator<< (std::ostream& s, mat3x3<T> const &m);

    } /* _namespace __detail */

  //! Single-precision 3x3 matrices
    typedef __detail::mat3x3<float> mat3x3f;
  //! Double-precision 3x3 matrices
    typedef __detail::mat3x3<double> mat3x3d;
  //! Single-precision 3x3 matrices (the same as \ref mat3x3f)
    typedef mat3x3f mat3f;
  //! Double-precision 3x3 matrices (the same as \ref mat3x3f)
    typedef mat3x3d mat3d;

} /* namespace cs237 */

#endif /* !_CS237_MAT3X3_HXX_ */
