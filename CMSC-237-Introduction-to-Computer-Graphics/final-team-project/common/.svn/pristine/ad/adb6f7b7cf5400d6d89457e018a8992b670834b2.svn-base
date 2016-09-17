/*! \file cs237-vec4.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_VEC4_HXX_
#define _CS237_VEC4_HXX_

#ifndef _CS237_HXX_
#error "c237-vec4.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      //! template class for four-element vectors
	template <typename T>
	struct vec4 {
	    T x, y, z, w;

	    vec4 () : x(T(0)), y(T(0)), z(T(0)), w(T(0)) { }
	    vec4 (vec4 const &v) : x(v.x), y(v.y), z(v.z), w(v.w) { }
	    explicit vec4 (T xx) : x(xx), y(T(0)), z(T(0)), w(T(0)) { }
	    explicit vec4 (T xx, T yy) : x(xx), y(yy), z(T(0)), w(T(0)) { }
	    explicit vec4 (T xx, T yy, T zz) : x(xx), y(yy), z(zz), w(T(0)) { }
	    explicit vec4 (T xx, T yy, T zz, T ww) : x(xx), y(yy), z(zz), w(ww) { }
	    explicit vec4 (vec2<T> const &v) : x(v.x), y(v.y), z(T(0)), w(T(0)) { }
	    explicit vec4 (vec2<T> const &v, T zz) : x(v.x), y(v.y), z(zz), w(T(0)) { }
	    explicit vec4 (vec2<T> const &v, T zz, T ww) : x(v.x), y(v.y), z(zz), w(ww) { }
	    explicit vec4 (vec3<T> const &v) : x(v.x), y(v.y), z(v.z), w(T(0)) { }
	    explicit vec4 (vec3<T> const &v, T ww) : x(v.x), y(v.y), z(v.z), w(ww) { }

	  ///@{
	  //! return the ith element of the vector
	    T & operator[] (unsigned int const &i);
	    T const & operator[] (unsigned int const &i) const;
	  ///@}

	    vec4 & operator= (vec4 const &v);

	  //! add a scalar to this vector
	    vec4 & operator+= (T const &s);
	  //! add a vector to this vector
	    vec4 & operator+= (vec4 const &v);

	  //! subtract a scalar from this vector
	    vec4 & operator-= (T const &s);
	  //! subtract a vector from this vector
	    vec4 & operator-= (vec4 const &v);

	  //! multiply this vector by a scalar
	    vec4 & operator*= (T const &s);
	  //! component-wise vector multiplication
	    vec4 & operator*= (vec4 const &v);

	  //! divide this vector by a scalar
	    vec4 & operator/= (T const &s);

	  //! the length (magnitude) of the vector
	    T length () const;
	  //! normalize the vector
	    vec4 & normalize ();
	};

      //! \brief output the vector to a stream
      //! \param s the stream to print to
      //! \param v the vector to print
	template <typename T>
	std::ostream& operator<< (std::ostream& s, vec4<T> const &v);

    } /* _namespace __detail */

  //! Four-element, single-precision floating-point vectors
    typedef __detail::vec4<float> vec4f;
  //! Four-element, double-precision floating-point vectors
    typedef __detail::vec4<double> vec4d;

} /* namespace cs237 */

#endif /* !_CS237_VEC4_HXX_ */
