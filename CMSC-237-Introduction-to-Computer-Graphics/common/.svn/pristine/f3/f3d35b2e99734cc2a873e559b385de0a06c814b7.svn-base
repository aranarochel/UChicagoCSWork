/*! \file cs237-vec3.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_VEC3_HXX_
#define _CS237_VEC3_HXX_

#ifndef _CS237_HXX_
#error "c237-vec3.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      //! template class for three-element vectors
	template <typename T>
	struct vec3 {
	    T x, y, z;

	    vec3 () : x(T(0)), y(T(0)), z(T(0)) { }
	    vec3 (vec3 const &v) : x(v.x), y(v.y), z(v.z) { }
	    explicit vec3 (T xx) : x(xx), y(T(0)), z(T(0)) { }
	    explicit vec3 (T xx, T yy) : x(xx), y(yy), z(T(0)) { }
	    explicit vec3 (T xx, T yy, T zz) : x(xx), y(yy), z(zz) { }
	    explicit vec3 (vec2<T> const &v) : x(v.x), y(v.y), z(T(0)) { }
	    explicit vec3 (vec2<T> const &v, T zz) : x(v.x), y(v.y), z(zz) { }
	    explicit vec3 (vec4<T> const &v);  // first three components of a 4-vector

	  ///@{
	  //! return the ith element of the vector
	    T const & operator[] (unsigned int const &i) const;
	    T & operator[] (unsigned int const &i);
	  ///@}

	    vec3 & operator= (vec3 const &v);

	  //! add a scalar to this vector
	    vec3 & operator+= (T const &s);
	  //! add a vector to this vector
	    vec3 & operator+= (vec3 const &v);

	  //! subtract a scalar from this vector
	    vec3 & operator-= (T const &s);
	  //! subtract a vector from this vector
	    vec3 & operator-= (vec3 const &v);

	  //! multiply this vector by a scalar
	    vec3 & operator*= (T const &s);
	  //! component-wise vector multiplication
	    vec3 & operator*= (vec3 const &v);

	  //! divide this vector by a scalar
	    vec3 & operator/= (T const &s);

	  //! the length (magnitude) of the vector
	    T length () const;
	  //! normalize the vector
	    vec3 & normalize ();
	};

      //! \brief output the vector to a stream
      //! \param s the stream to print to
      //! \param v the vector to print
	template <typename T>
	std::ostream& operator<< (std::ostream& s, vec3<T> const &v);

    } /* _namespace __detail */

  //! Three-element, single-precision floating-point vectors
    typedef __detail::vec3<float> vec3f;
  //! Three-element, double-precision floating-point vectors
    typedef __detail::vec3<double> vec3d;

} /* namespace cs237 */

#endif /* !_CS237_VEC3_HXX_ */
