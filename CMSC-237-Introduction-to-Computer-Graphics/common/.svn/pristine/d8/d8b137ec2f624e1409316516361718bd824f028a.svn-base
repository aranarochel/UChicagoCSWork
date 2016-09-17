/*! \file cs237-mat.cxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Operations on matrix values, including creating and applying transformation matrices.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"

namespace cs237 {

    namespace __detail {

      // 4x4 matrix determinant
	template <typename T>
	T mat4x4<T>::determinant() const
	{
#define M(r,c)	this->cv[c][r]
	  // 2x2 subdeterminants
	    T tbt23C01 = M(2,0)*M(3,1) - M(2,1)*M(3,0);
	    T tbt23C02 = M(2,0)*M(3,2) - M(2,2)*M(3,0);
	    T tbt23C03 = M(2,0)*M(3,3) - M(2,3)*M(3,0);
	    T tbt23C12 = M(2,1)*M(3,2) - M(2,2)*M(3,1);
	    T tbt23C13 = M(2,1)*M(3,3) - M(2,3)*M(3,1);
	    T tbt23C23 = M(2,2)*M(3,3) - M(2,3)*M(3,2);
	  // 3x3 subdeterminants
	    T sd00 = M(1,1)*tbt23C23 - M(1,2)*tbt23C13 + M(1,3)*tbt23C12;
	    T sd01 = M(1,0)*tbt23C23 - M(1,2)*tbt23C03 + M(1,3)*tbt23C02;
	    T sd02 = M(1,0)*tbt23C13 - M(1,1)*tbt23C03 + M(1,3)*tbt23C01;
	    T sd03 = M(1,0)*tbt23C12 - M(1,1)*tbt23C02 + M(1,2)*tbt23C01;

	    return M(0,0)*sd00 - M(0,1)*sd01 + M(0,2)*sd02 - M(0,3)*sd03;
#undef M
	}

      // 4x4 matrix inverse
	template <typename T>
	mat4x4<T> mat4x4<T>::inverse() const
	{
#define M(r,c)	this->cv[c][r]
	  // 2x2 subdeterminants
	    T tbt23C01 = M(2,0)*M(3,1) - M(2,1)*M(3,0);
	    T tbt23C02 = M(2,0)*M(3,2) - M(2,2)*M(3,0);
	    T tbt23C03 = M(2,0)*M(3,3) - M(2,3)*M(3,0);
	    T tbt23C12 = M(2,1)*M(3,2) - M(2,2)*M(3,1);
	    T tbt23C13 = M(2,1)*M(3,3) - M(2,3)*M(3,1);
	    T tbt23C23 = M(2,2)*M(3,3) - M(2,3)*M(3,2);
	  // 2x2 subdeterminants
	    T tbt13C01 = M(1,0)*M(3,1) - M(1,1)*M(3,0);
	    T tbt13C02 = M(1,0)*M(3,2) - M(1,2)*M(3,0);
	    T tbt13C03 = M(1,0)*M(3,3) - M(1,3)*M(3,0);
	    T tbt13C12 = M(1,1)*M(3,2) - M(1,2)*M(3,1);
	    T tbt13C13 = M(1,1)*M(3,3) - M(1,3)*M(3,1);
	    T tbt13C23 = M(1,2)*M(3,3) - M(1,3)*M(3,2);
	  // 2x2 subdeterminants
	    T tbt12C01 = M(1,0)*M(2,1) - M(1,1)*M(2,0);
	    T tbt12C02 = M(1,0)*M(2,2) - M(1,2)*M(2,0);
	    T tbt12C03 = M(1,0)*M(2,3) - M(1,3)*M(2,0);
	    T tbt12C12 = M(1,1)*M(2,2) - M(1,2)*M(2,1);
	    T tbt12C13 = M(1,1)*M(2,3) - M(1,3)*M(2,1);
	    T tbt12C23 = M(1,2)*M(2,3) - M(1,3)*M(2,2);
	  // 3x3 subdeterminants
	    T sd00 = M(1,1)*tbt23C23 - M(1,2)*tbt23C13 + M(1,3)*tbt23C12;
	    T sd01 = M(1,0)*tbt23C23 - M(1,2)*tbt23C03 + M(1,3)*tbt23C02;
	    T sd02 = M(1,0)*tbt23C13 - M(1,1)*tbt23C03 + M(1,3)*tbt23C01;
	    T sd03 = M(1,0)*tbt23C12 - M(1,1)*tbt23C02 + M(1,2)*tbt23C01;
	  // 3x3 subdeterminants
	    T sd10 = M(0,1)*tbt23C23 - M(0,2)*tbt23C13 + M(0,3)*tbt23C12;
	    T sd11 = M(0,0)*tbt23C23 - M(0,2)*tbt23C03 + M(0,3)*tbt23C02;
	    T sd12 = M(0,0)*tbt23C13 - M(0,1)*tbt23C03 + M(0,3)*tbt23C01;
	    T sd13 = M(0,0)*tbt23C12 - M(0,1)*tbt23C02 + M(0,2)*tbt23C01;
	  // 3x3 subdeterminants
	    T sd20 = M(0,1)*tbt13C23 - M(0,2)*tbt13C13 + M(0,3)*tbt13C12;
	    T sd21 = M(0,0)*tbt13C23 - M(0,2)*tbt13C03 + M(0,3)*tbt13C02;
	    T sd22 = M(0,0)*tbt13C13 - M(0,1)*tbt13C03 + M(0,3)*tbt13C01;
	    T sd23 = M(0,0)*tbt13C12 - M(0,1)*tbt13C02 + M(0,2)*tbt13C01;
	  // 3x3 subdeterminants
	    T sd30 = M(0,1)*tbt12C23 - M(0,2)*tbt12C13 + M(0,3)*tbt12C12;
	    T sd31 = M(0,0)*tbt12C23 - M(0,2)*tbt12C03 + M(0,3)*tbt12C02;
	    T sd32 = M(0,0)*tbt12C13 - M(0,1)*tbt12C03 + M(0,3)*tbt12C01;
	    T sd33 = M(0,0)*tbt12C12 - M(0,1)*tbt12C02 + M(0,2)*tbt12C01;
	  // determinate
	    T det = M(0,0)*sd00 - M(0,1)*sd01 + M(0,2)*sd02 - M(0,3)*sd03;
	  // check for invertability
	    if ((det < T(-0.00001)) || (T(0.0001) < det)) {
		T detInv = T(1) / det;
		return mat4x4<T> (
		     sd00*detInv, -sd01*detInv,  sd02*detInv, -sd03*detInv,
		    -sd10*detInv,  sd11*detInv, -sd12*detInv,  sd13*detInv,
		     sd20*detInv, -sd21*detInv,  sd22*detInv, -sd23*detInv,
		    -sd30*detInv,  sd31*detInv, -sd32*detInv,  sd33*detInv);
	    }
	    else
		return *this;
#undef M
	}

      // extract a 3x3 "normal" transform matrix from a 4x4 affine transform matrix, which
      // is the inverse transpose of the upper-left 3x3 submatrix.
	template <typename T>
	mat3x3<T> mat4x4<T>::normalMatrix() const
	{
#define M(r,c)	this->cv[c][r]
	  // scale by 1/determinate
	    T det = M(0,0) * (M(1,1)*M(2,2) - M(1,2)*M(2,1))
		  - M(0,1) * (M(1,0)*M(2,2) - M(1,2)*M(2,0))
		  + M(0,2) * (M(1,0)*M(2,1) - M(1,1)*M(2,0));
	    T s = T(1) / det;
	  // we swap rows and columns in the construction of the result
	    return mat3x3<T> (
		// first column
		   s * (M(1,1)*M(2,2) - M(1,2)*M(2,1)),
		   s * (M(0,2)*M(2,1) - M(0,1)*M(2,2)),
		   s * (M(0,1)*M(1,2) - M(0,2)*M(1,1)),
		// second column
		   s * (M(1,2)*M(2,0) - M(1,0)*M(2,2)),
		   s * (M(0,0)*M(2,2) - M(0,2)*M(2,0)),
		   s * (M(0,2)*M(1,0) - M(0,0)*M(1,2)),
		// third column
		   s * (M(1,0)*M(2,1) - M(1,1)*M(2,0)),
		   s * (M(0,1)*M(2,0) - M(0,0)*M(2,1)),
		   s * (M(0,0)*M(1,1) - M(0,1)*M(1,0))
		);
#undef M
	}

      // instantiate the mat4x4 class to generate the functions
	template struct mat4x4<float>;
	template struct mat4x4<double>;

    } /* namespace __detail */

  /***** Transforms *****/

  // isotropic (uniform) scaling
#define CS237_SCALE(T) __detail::mat4x4<T> scale (T s)
    template <typename T> CS237_SCALE(T)
    {
	return __detail::mat4x4<T>(
	      s,  T(0), T(0), T(0),	// first column
	    T(0),   s,  T(0), T(0),	// second column
	    T(0), T(0),   s,  T(0),	// third column
	    T(0), T(0), T(0), T(1));	// fourth column
    } /* end of scale */
  template CS237_SCALE(float);
  template CS237_SCALE(double);
#undef CS237_SCALE

  // isotropic (uniform) scaling
#define CS237_SCALE(T) __detail::mat4x4<T> scale (__detail::mat4x4<T> const &m, T s)
    template <typename T> CS237_SCALE(T)
    {
#define M(r,c)	m.cv[c][r]
	return __detail::mat4x4<T>(
	    M(0,0)*s, M(1,0)*s, M(2,0)*s, M(3,0)*s,
	    M(0,1)*s, M(1,1)*s, M(2,1)*s, M(3,1)*s,
	    M(0,2)*s, M(1,2)*s, M(2,2)*s, M(3,2)*s,
	    M(0,3), M(1,3), M(2,3), M(3,3));
#undef M
    }
  template CS237_SCALE(float);
  template CS237_SCALE(double);
#undef CS237_SCALE

  // anisotropic (non-uniform) scaling
#define CS237_SCALE(T) __detail::mat4x4<T> scale (__detail::vec3<T> const &s)
    template <typename T> CS237_SCALE(T)
    {
	return __detail::mat4x4<T>(
	    s[0], T(0), T(0), T(0),	// first column
	    T(0), s[1], T(0), T(0),	// second column
	    T(0), T(0), s[2], T(0),	// third column
	    T(0), T(0), T(0), T(1));	// fourth column
    }
  template CS237_SCALE(float);
  template CS237_SCALE(double);
#undef CS237_SCALE

#define CS237_SCALE(T) __detail::mat4x4<T> scale (__detail::mat4x4<T> const &m, __detail::vec3<T> const &s)
    template <typename T> CS237_SCALE(T)
    {
#define M(r,c)	m.cv[c][r]
	return __detail::mat4x4<T>(
	    M(0,0)*s[0], M(1,0)*s[0], M(2,0)*s[0], M(3,0)*s[0],	// first column
	    M(0,1)*s[1], M(1,1)*s[1], M(2,1)*s[1], M(3,1)*s[1],	// second column
	    M(0,2)*s[2], M(1,2)*s[2], M(2,2)*s[2], M(3,2)*s[2],	// third column
	    M(0,3),      M(1,3),      M(2,3),      M(3,3));	// fourth column
#undef M
    }
  template CS237_SCALE(float);
  template CS237_SCALE(double);
#undef CS237_SCALE

  // rotation about X, Y, and Z axes
#define CS237_ROTATEX(T) __detail::mat4x4<T> rotateX (T theta)
    template <typename T> CS237_ROTATEX(T)
    {
	T angle = radians(theta);
	T c = cos(angle);
	T s = sin(angle);
	return __detail::mat4x4<T>(
	    T(1), T(0), T(0), T(0),	// first column
	    T(0),   c,   -s,  T(0),	// second column
	    T(0),   s,    c,  T(0),	// third column
	    T(0), T(0), T(0), T(1));	// fourth column
    } /* end of rotateX */
    template CS237_ROTATEX(float);
    template CS237_ROTATEX(double);
#undef CS237_ROTATEX

#define CS237_ROTATEX(T) __detail::mat4x4<T> rotateX (__detail::mat4x4<T> const &m, T theta)
    template <typename T> CS237_ROTATEX(T)
    {
#define M(r,c)	m.cv[c][r]
	T angle = radians(theta);
	T c = cos(angle);
	T s = sin(angle);
	return __detail::mat4x4<T>(
	    M(0,0), M(1,0), M(2,0), M(3,0),
	    M(0,1)*c - M(0,2)*s, M(1,1)*c - M(1,2)*s, M(2,1)*c - M(2,2)*s, M(3,1)*c - M(3,2)*s,
	    M(0,1)*s + M(0,2)*c, M(1,1)*s + M(1,2)*c, M(2,1)*s + M(2,2)*c, M(3,1)*s + M(3,2)*c,
	    M(0,3), M(1,3), M(2,3), M(3,3));
#undef M
    } /* end of rotateX */
    template CS237_ROTATEX(float);
    template CS237_ROTATEX(double);
#undef CS237_ROTATEX

#define CS237_ROTATEY(T) __detail::mat4x4<T> rotateY (T theta)
    template <typename T> CS237_ROTATEY(T)
    {
	T angle = radians(theta);
	T c = cos(angle);
	T s = sin(angle);
	return __detail::mat4x4<T>(
	      c,  T(0),   s,  T(0),	// first column
	    T(0), T(1), T(0), T(0),	// second column
	     -s,  T(0),   c,  T(0),	// third column
	    T(0), T(0), T(0), T(1));	// fourth column
    } /* end of rotateY */
    template CS237_ROTATEY(float);
    template CS237_ROTATEY(double);
#undef CS237_ROTATEY

#define CS237_ROTATEY(T) __detail::mat4x4<T> rotateY (__detail::mat4x4<T> const &m, T theta)
    template <typename T> CS237_ROTATEY(T)
    {
	T angle = radians(theta);
	T c = cos(angle);
	T s = sin(angle);
#define M(r,c)	m.cv[c][r]
	return __detail::mat4x4<T>(
	    M(0,0)*c + M(0,2)*s, M(1,0)*c + M(1,2)*s, M(2,0)*c + M(2,2)*s, M(3,0)*c + M(3,2)*s,
	    M(0,1), M(1,1), M(2,1), M(3,1),
	    -M(0,0)*s + M(0,2)*c, -M(1,0)*s + M(1,2)*c, -M(2,0)*s + M(2,2)*c, -M(3,0)*s + M(3,2)*c,
	    M(0,3), M(1,3), M(2,3), M(3,3));
#undef M
    } /* end of rotateY */
    template CS237_ROTATEY(float);
    template CS237_ROTATEY(double);
#undef CS237_ROTATEY

#define CS237_ROTATEZ(T) __detail::mat4x4<T> rotateZ (T theta)
    template <typename T> CS237_ROTATEZ(T)
    {
	T angle = radians(theta);
	T c = cos(angle);
	T s = sin(angle);
	return __detail::mat4x4<T>(
	      c,   -s,  T(0), T(0),	// first column
	      s,    c,  T(0), T(0),	// second column
	    T(0), T(0), T(1), T(0),	// third column
	    T(0), T(0), T(0), T(1));	// fourth column
    } /* end of rotateZ */
    template CS237_ROTATEZ(float);
    template CS237_ROTATEZ(double);
#undef CS237_ROTATEZ

#define CS237_ROTATEZ(T) __detail::mat4x4<T> rotateZ (__detail::mat4x4<T> const &m, T theta)
    template <typename T> CS237_ROTATEZ(T)
    {
	T angle = radians(theta);
	T c = cos(angle);
	T s = sin(angle);
#define M(r,c)	m.cv[c][r]
	return __detail::mat4x4<T>(
	    M(0,0)*c - M(0,1)*s, M(1,0)*c - M(1,1)*s, M(2,0)*c - M(2,1)*s, M(3,0)*c - M(3,1)*s,
	    M(0,0)*s + M(0,1)*c, M(1,0)*s + M(1,1)*c, M(2,0)*s + M(2,1)*c, M(3,0)*s + M(3,1)*c,
	    M(0,2), M(1,2), M(2,2), M(3,2),
	    M(0,3), M(1,3), M(2,3), M(3,3));
#undef M
    } /* end of rotateZ */
    template CS237_ROTATEZ(float);
    template CS237_ROTATEZ(double);
#undef CS237_ROTATEZ

  // rotation about an arbitrary axis
#define CS237_ROTATE(T) __detail::mat4x4<T> rotate (T theta, __detail::vec3<T> const &axis)
    template <typename T> CS237_ROTATE(T)
    {
	T len = axis.length();
	if (len <= T(0.0001))
	    return __detail::mat4x4<T>(T(1));  // no rotation, since axis is degenerate, so return I
	__detail::vec3<T> a = axis / len;
	T angle = radians(theta);
	T c = cos(angle);
	T s = sin(angle);
	__detail::vec3<T> asq = a*a;

	return __detail::mat4x4<T>(
	    asq[0] + c*(T(1) - asq[0]), a[0]*a[1]*(T(1) - c) + a[2]*s, a[0]*a[2]*(T(1) - c) - a[1]*s, T(0),
	    a[0]*a[1]*(T(1) - c) - a[2]*s, asq[1] + c*(T(1) - asq[1]), a[1]*a[2]*(T(1) - c) + a[0]*s, T(0),
	    a[0]*a[2]*(T(1) - c) + a[1]*s, a[1]*a[2]*(T(1) - c) - a[0]*s, asq[2] + c*(T(1) - asq[2]), T(0),
	    T(0), T(0), T(0), T(1));
    }
    template CS237_ROTATE(float);
    template CS237_ROTATE(double);
#undef CS237_ROTATE

#define CS237_ROTATE(T) __detail::mat4x4<T> rotate (__detail::mat4x4<T> const &m, T theta, __detail::vec3<T> const &axis)
    template <typename T> CS237_ROTATE(T)
    {
	return m * rotate(theta, axis);
    }
    template CS237_ROTATE(float);
    template CS237_ROTATE(double);
#undef CS237_ROTATE

  // translation
#define CS237_TRANSLATE(T) __detail::mat4x4<T> translate (__detail::vec3<T> const &t)
    template <typename T> CS237_TRANSLATE(T)
    {
	return __detail::mat4x4<T>(
	    T(1), T(0), T(0), T(0),	// first column
	    T(0), T(1), T(0), T(0),	// second column
	    T(0), T(0), T(1), T(0),	// third column
	    t[0], t[1], t[2], T(1));	// fourth column
    }
    template CS237_TRANSLATE(float);
    template CS237_TRANSLATE(double);
#undef CS237_TRANSLATE

#define CS237_TRANSLATE(T) __detail::mat4x4<T> translate (__detail::mat4x4<T> const &m, __detail::vec3<T> const &t)
    template <typename T> CS237_TRANSLATE(T)
    {
#define M(r,c)	m.cv[c][r]
	return __detail::mat4x4<T>(
	    M(0,0), M(1,0), M(2,0), M(3,0),
	    M(0,1), M(1,1), M(2,1), M(3,1),
	    M(0,2), M(1,2), M(2,2), M(3,2),
	    M(0,0)*t[0] + M(0,1)*t[1] + M(0,2)*t[2] + M(0,3),
	    M(1,0)*t[0] + M(1,1)*t[1] + M(1,2)*t[2] + M(1,3),
	    M(2,0)*t[0] + M(2,1)*t[1] + M(2,2)*t[2] + M(2,3),
	    M(3,0)*t[0] + M(3,1)*t[1] + M(3,2)*t[2] + M(3,3));
#undef M
    }
    template CS237_TRANSLATE(float);
    template CS237_TRANSLATE(double);
#undef CS237_TRANSLATE

  //! build a a viewing matrix derived from an eye point, a reference point indicating the
  //! center of the scene, and an UP vector.
#define CS237_LOOKAT(T) __detail::mat4x4<T> lookAt (__detail::vec3<T> const &eye, __detail::vec3<T> const &center, __detail::vec3<T> const &up)
    template <typename T> CS237_LOOKAT(T)
    {
	__detail::vec3<T> f(__detail::normalize(center - eye));
	__detail::vec3<T> upN(__detail::normalize(up));
	__detail::vec3<T> s(__detail::cross<T>(f, upN));
	__detail::vec3<T> u(__detail::cross(s, f));

	__detail::mat4x4<T> M = __detail::mat4x4<T>(
	    s.x, u.x, -f.x, T(0),
	    s.y, u.y, -f.y, T(0),
	    s.z, u.z, -f.z, T(0),
	    T(0), T(0), T(0), T(1));

	return translate(M, -eye);

    } // lookAt
    template CS237_LOOKAT(float);
    template CS237_LOOKAT(double);
#undef CS237_LOOKAT

  //! set up a transformation matrix that produces a parallel projection
#define CS237_ORTHO(T) __detail::mat4x4<T> ortho (T const &left, T const &right, T const &bottom, T const &top, T const &nearVal, T const &farVal)
    template <typename T> CS237_ORTHO(T)
    {
	return __detail::mat4x4<T>(
	    T(2) / (right - left), T(0), T(0), T(0),
	    T(0), T(2) / (top - bottom), T(0), T(0),
	    T(0), T(0), T(-2) / (farVal - nearVal), T(0),
	    -(right + left) / (right - left),
	    -(top + bottom) / (top - bottom),
	    -(farVal + nearVal) / (farVal - nearVal),
	    T(1));

    } // ortho
    template CS237_ORTHO(float);
    template CS237_ORTHO(double);
#undef CS237_ORTHO

  //! set up a transformation matrix that produces two-dimensional orthographic viewing region
#define CS237_ORTHO2D(T) __detail::mat4x4<T> ortho2D (T const &left, T const &right, T const &bottom, T const &top)
    template <typename T> CS237_ORTHO2D(T)
    {
	return __detail::mat4x4<T>(
	    T(2) / (right - left), T(0), T(0), T(0),
	    T(0), T(2) / (top - bottom), T(0), T(0),
	    T(0), T(0), T(-1), T(0),
	    -(right + left) / (right - left), -(top + bottom) / (top - bottom), T(0), T(1));

    } // ortho2D
    template CS237_ORTHO2D(float);
    template CS237_ORTHO2D(double);
#undef CS237_ORTHO2D

  //! specifies a perspective projection matrix
#define CS237_PERSPECTIVE(T) __detail::mat4x4<T> perspective (T const &fov, T const &aspect, T const &zNear, T const &zFar)
    template <typename T> CS237_PERSPECTIVE(T)
    {
	assert(aspect != T(0));
	assert(zFar != zNear);

	T rad = radians(fov);
	T f = 1.0 / tan(T(0.5) * rad);

	return __detail::mat4x4<T>(
	    f / aspect, T(0), T(0), T(0),
	    T(0), f, T(0), T(0),
	    T(0), T(0), (zFar + zNear) / (zNear - zFar), T(-1),
	    T(0), T(0), (T(2) * zFar * zNear) / (zNear - zFar), T(0));

    } // perspective
    template CS237_PERSPECTIVE(float);
    template CS237_PERSPECTIVE(double);
#undef CS237_PERSPECTIVE

  //! specifies a viewing frustum into the world coordinate system
#define CS237_FRUSTUM(T) __detail::mat4x4<T> frustum (T const &left, T const &right, T const &bottom, T const &top, T const &nearVal, T const &farVal)
    template <typename T> CS237_FRUSTUM(T)
    {
	T width = right - left;
	T height = top - bottom;
	T depth = farVal - nearVal;
	return __detail::mat4x4<T>(
	    (T(2) * nearVal) / width, T(0), T(0), T(0),
	    T(0), (T(2) * nearVal) / height, T(0), T(0),
	    (right + left) / width, (top + bottom) / height, -(farVal + nearVal) / depth, T(-1),
	    T(0), T(0), (T(-2) * farVal * nearVal) / depth, T(0));

    } // frustum
    template CS237_FRUSTUM(float);
    template CS237_FRUSTUM(double);
#undef CS237_FRUSTUM

/***** Matrix output *****/

    namespace __detail {

	#define PROTO(MAT,T) std::ostream& operator<< (std::ostream& s, MAT<T> const &m)

	// output the matrix to a stream as a sequence of column vectors
	//
	template <typename T> PROTO(mat2x2,T)
	{
	    return (s << "[" << m[0] << "," << m[1] << "]");
	}
	template PROTO(mat2x2,float);
	template PROTO(mat2x2,double);

	// output the matrix to a stream as a sequence of column vectors
	//
	template <typename T> PROTO(mat2x3,T)
	{
	    return (s << "[" << m[0] << "," << m[1] << "]");
	}
	template PROTO(mat2x3,float);
	template PROTO(mat2x3,double);

	// output the matrix to a stream as a sequence of column vectors
	//
	template <typename T> PROTO(mat3x2,T)
	{
	    return (s << "[" << m[0] << "," << m[1] << "," << m[2] << "]");
	}
	template PROTO(mat3x2,float);
	template PROTO(mat3x2,double);

	// output the matrix to a stream as a sequence of column vectors
	//
	template <typename T> PROTO(mat3x3,T)
	{
	    return (s << "[" << m[0] << "," << m[1] << "," << m[2] << "]");
	}
	template PROTO(mat3x3,float);
	template PROTO(mat3x3,double);

	// output the matrix to a stream as a sequence of column vectors
	//
	template <typename T> PROTO(mat4x4,T)
	{
	    return (s << "[" << m[0] << "," << m[1] << "," << m[2] << "," << m[3] << "]");
	}
	template PROTO(mat4x4,float);
	template PROTO(mat4x4,double);

	#undef PROTO
    } // namespace __detail

} /* namespace cs237 */
