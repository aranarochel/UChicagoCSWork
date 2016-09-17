/*! \file cs237-vec2.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_VEC2_HXX_
#define _CS237_VEC2_HXX_

#ifndef _CS237_HXX_
#error "c237-vec2.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      //! template class for two-element vectors
	template <typename T>
	struct vec2 {
	    T x, y;

	    vec2 () : x(T(0)), y(T(0)) { }
	    vec2 (vec2 const &v) : x(v.x), y(v.y) { }
	    explicit vec2 (T xx) : x(xx), y(T(0)) { }
	    explicit vec2 (T xx, T yy) : x(xx), y(yy) { }
	    explicit vec2 (vec3<T> const &v);  // first two components of 3-vector
	    explicit vec2 (vec4<T> const &v);  // first two components of 4-vector

	  ///@{
	  //! return the i'th element of the vector
	    T & operator[] (unsigned int const &i);
	    T const & operator[] (unsigned int const &i) const;
	  ///@}

	    vec2 & operator= (vec2 const &v);

	  //! add a scalar to this vector
	    vec2 & operator+= (T const &s);
	  //! add a vector to this vector
	    vec2 & operator+= (vec2 const &v);

	  //! subtract a scalar from this vector
	    vec2 & operator-= (T const &s);
	  //! subtract a vector from this vector
	    vec2 & operator-= (vec2 const &v);

	  //! multiply this vector by a scalar
	    vec2 & operator*= (T const &s);
	  //! component-wise vector multiplication
	    vec2 & operator*= (vec2 const &v);

	  //! divide this vector by a scalar
	    vec2 & operator/= (T const &s);

	  //! the length (magnitude) of the vector
	    T length () const;
	  //! normalize the vector
	    vec2 & normalize ();
	};

      //! \brief output the vector to a stream
      //! \param s the stream to print to
      //! \param v the vector to print
	template <typename T>
	std::ostream& operator<< (std::ostream& s, vec2<T> const &v);

    } /* _namespace __detail */

  //! Two-element, single-precision floating-point vectors
    typedef __detail::vec2<float> vec2f;
  //! Two-element, double-precision floating-point vectors
    typedef __detail::vec2<double> vec2d;

} /* namespace cs237 */

#endif /* !_CS237_VEC2_HXX_ */
