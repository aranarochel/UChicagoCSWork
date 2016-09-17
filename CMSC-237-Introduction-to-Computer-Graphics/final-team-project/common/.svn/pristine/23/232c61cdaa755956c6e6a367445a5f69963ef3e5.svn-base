/*! \file cs237-mat2x2.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_MAT2X2_HXX_
#define _CS237_MAT2X2_HXX_

#ifndef _CS237_HXX_
#error "c237-mat2x2.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      //! template class for 2x2 matrices
	template <typename T>
	struct mat2x2 {
	    vec2<T> cv[2];	// column vectors

	  //! create an uninitialized matrix
	    mat2x2 () {}
	  //! copy constructor
	    mat2x2 (mat2x2 const &m);
	  //! construct a matrix from individual elements given in column-major order
	    explicit mat2x2 (
		T m00, T m10,	// first column
		T m01, T m11);	// second column
	  //! construct a matrix from column vectors
	    explicit mat2x2 (vec2<T> const &c0, vec2<T> const &c1);
	  //! extract the upper-left 2x2 matrix from a 3x3 matrix
	    explicit mat2x2 (mat3x3<T> const &mm);
	  //! extract the upper-left 2x2 matrix from a 2x3 matrix
	    explicit mat2x2 (mat2x3<T> const &mm);
	  //! extract the upper-left 2x2 matrix from a 3x2 matrix
	    explicit mat2x2 (mat3x2<T> const &mm);
	  //! construct a diagonal matrix with diagonal elements initialized to x
	    explicit mat2x2 (T const &x);

	  ///@{
	  //! return the i'th column vector of the matrix
	    vec2<T> & operator[] (unsigned int const &i);
	    vec2<T> const & operator[] (unsigned int const &i) const;
	  ///@}

	    mat2x2 & operator= (mat2x2 const &mm);

	    mat2x2 & operator+= (T const &s);
	    mat2x2 & operator+= (mat2x2 const &v);

	    mat2x2 & operator-= (T const &s);
	    mat2x2 & operator-= (mat2x2 const &v);

	    mat2x2 & operator*= (T const &s);
	    mat2x2 & operator*= (mat2x2 const &m);

	  //! return the inverse of this matrix
	    mat2x2 inverse() const;

	  //! return the transpose of this matrix
	    mat2x2 transpose() const;

	  //! return the determiniant of this matrix
	    T determinant() const;
	};

	//! \brief output the matrix to a stream as a sequence of column vectors
	//! \param s the stream to print to
	//! \param m the matrix to print
	template <typename T>
	std::ostream& operator<< (std::ostream& s, mat2x2<T> const &m);

    } /* _namespace __detail */

  //! Single-precision 2x2 matrices
    typedef __detail::mat2x2<float> mat2x2f;
  //! Double-precision 2x2 matrices
    typedef __detail::mat2x2<double> mat2x2d;
  //! Single-precision 2x2 matrices (the same as \ref mat2x2f)
    typedef mat2x2f mat2f;
  //! Double-precision 2x2 matrices (the same as \ref mat2x2d)
    typedef mat2x2d mat2d;

} /* namespace cs237 */

#endif /* !_CS237_MAT2X2_HXX_ */
