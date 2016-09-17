/*! \file cs237-mat3x2.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_MAT3X2_HXX_
#define _CS237_MAT3X2_HXX_

#ifndef _CS237_HXX_
#error "c237-mat3x2.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      //! template class for 3x2 matrices
	template <typename T>
	struct mat3x2 {
	    vec2<T> cv[3];	// column vectors

	  //! create an uninitialized matrix
	    mat3x2 () {}
	  //! copy constructor
	    mat3x2 (mat3x2 const &m);
	  //! construct a matrix from individual elements given in column-major order
	    explicit mat3x2 (
		T m00, T m10,	// first column
		T m01, T m11,	// second column
		T m02, T m12);	// third column
	  //! construct a matrix from column vectors
	    explicit mat3x2 (vec2<T> const &c0, vec2<T> const &c1, vec2<T> const &c2);
	  //! extract the upper 3x2 matrix from a 3x3 matrix
	    explicit mat3x2 (mat3x3<T> const &mm);

	  ///@{
	  //! return the i'th column vector of the matrix
	    vec2<T> & operator[] (unsigned int const &i);
	    vec2<T> const & operator[] (unsigned int const &i) const;
	  ///@}

	    mat3x2 & operator= (mat3x2 const &mm);

	    mat3x2 & operator+= (T const &s);
	    mat3x2 & operator+= (mat3x2 const &v);

	    mat3x2 & operator-= (T const &s);
	    mat3x2 & operator-= (mat3x2 const &v);

	  //! scale the elements of the matrix
	    mat3x2 & operator*= (T const &s);

	  //! return the transpose of this matrix
	    mat2x3<T> transpose() const;
	};

	//! \brief output the matrix to a stream as a sequence of column vectors
	//! \param s the stream to print to
	//! \param m the matrix to print
	template <typename T>
	std::ostream& operator<< (std::ostream& s, mat3x2<T> const &m);

    } /* _namespace __detail */

  //! Single-precision 3x2 matrices
    typedef __detail::mat3x2<float> mat3x2f;
  //! Double-precision 3x2 matrices
    typedef __detail::mat3x2<double> mat3x2d;

} /* namespace cs237 */

#endif /* !_CS237_MAT3X2_HXX_ */
