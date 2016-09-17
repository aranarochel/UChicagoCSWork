/*! \file cs237-mat2x3.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_MAT2X3_HXX_
#define _CS237_MAT2X3_HXX_

#ifndef _CS237_HXX_
#error "c237-mat2x3.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      //! template class for 2x3 matrices
	template <typename T>
	struct mat2x3 {
	    vec3<T> cv[2];	// column vectors

	  //! create an uninitialized matrix
	    mat2x3 () {}
	  //! copy constructor
	    mat2x3 (mat2x3 const &m);
	  //! construct a matrix from individual elements given in column-major order
	    explicit mat2x3 (
		T m00, T m10, T m20,	// first column
		T m01, T m11, T m21);	// second column
	  //! construct a matrix from column vectors
	    explicit mat2x3 (vec3<T> const &c0, vec3<T> const &c1);
	  //! extract the left 2x3 matrix from a 3x3 matrix
	    explicit mat2x3 (mat3x3<T> const &mm);

	  ///@{
	  //! return the i'th column vector of the matrix
	    vec3<T> & operator[] (unsigned int const &i);
	    vec3<T> const & operator[] (unsigned int const &i) const;
	  ///@}

	    mat2x3 & operator= (mat2x3 const &mm);

	    mat2x3 & operator+= (T const &s);
	    mat2x3 & operator+= (mat2x3 const &v);

	    mat2x3 & operator-= (T const &s);
	    mat2x3 & operator-= (mat2x3 const &v);

	    mat2x3 & operator*= (T const &s);

	  //! return the transpose of this matrix
	    mat3x2<T> transpose() const;
	};

	//! \brief output the matrix to a stream as a sequence of column vectors
	//! \param s the stream to print to
	//! \param m the matrix to print
	template <typename T>
	std::ostream& operator<< (std::ostream& s, mat2x3<T> const &m);

    } /* _namespace __detail */

  //! Single-precision 2x3 matrices
    typedef __detail::mat2x3<float> mat2x3f;
  //! Double-precision 2x3 matrices
    typedef __detail::mat2x3<double> mat2x3d;

} /* namespace cs237 */

#endif /* !_CS237_MAT2X3_HXX_ */
