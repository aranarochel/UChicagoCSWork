/*! \file cs237-quat.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_QUAT_HXX_
#define _CS237_QUAT_HXX_

#ifndef _CS237_HXX_
#error "c237-quat.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      //! template class for quaternions
	template <typename T>
	struct quat {
	    T x, y, z, w;

	    quat () : x(T(0)), y(T(0)), z(T(0)), w(T(0)) { }
	    quat (quat const &q) : x(q.x), y(q.y), z(q.z), w(q.w) { }
	    explicit quat (T xx, T yy, T zz) : x(xx), y(yy), z(zz), w(T(0)) { }
	    explicit quat (T xx, T yy, T zz, T ww) : x(xx), y(yy), z(zz), w(ww) { }
	    explicit quat (vec3<T> const &v) : x(v.x), y(v.y), z(v.z), w(T(0)) { }
	    explicit quat (vec3<T> const &v, T ww) : x(v.x), y(v.y), z(v.z), w(ww) { }

	  //! create a quaternion from an angle (in degrees) and an axis vector
	    explicit quat (T angle, vec3<T> const &axis);

	    T & operator[] (unsigned int const &i);
	    T const & operator[] (unsigned int const &i) const;

	    quat & operator= (quat const &v);

	    quat & operator+= (quat const &v);

	    quat & operator-= (quat const &v);

	    quat & operator*= (T const &s);
	    quat & operator*= (quat const &q);
	    quat & operator*= (vec3<T> const &v);

	    quat & operator/= (T const &s);

	    quat conjugate () const;
	    quat inverse () const;

	    T length () const;

	  //! normalize this quaternion to unit length
	    quat & normalize ();

	  //! apply the rotation represented by this quaternion to a vector; this operation
	  //! is equivalent to (*this) * v * this->inverse().
	    vec3<T> rotate (vec3<T> const &v) const;

	  //! return the 3x3 linear rotation matrix represented by this quaternion
	    mat3x3<T> toMat3x3 () const;

	  //! return the 3x3 affine rotation matrix represented by this quaternion
	    mat4x4<T> toMat4x4 () const;
	};

      //! \brief output the vector to a stream
      //! \param s the stream to print to
      //! \param v the vector to print
	template <typename T>
	std::ostream& operator<< (std::ostream& s, quat<T> const &v);

    } /* _namespace __detail */

  //! Single-precision quaternions
    typedef __detail::quat<float> quatf;
  //! Double-precision quaternions
    typedef __detail::quat<double> quatd;

} /* namespace cs237 */

#endif /* !_CS237_QUAT_HXX_ */
