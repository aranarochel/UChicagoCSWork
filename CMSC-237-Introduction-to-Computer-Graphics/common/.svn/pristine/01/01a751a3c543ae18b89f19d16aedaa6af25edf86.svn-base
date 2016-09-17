/*! \file cs237-transform.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Standard OpenGL transformation matrices.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_TRANSFORM_HXX_
#define _CS237_TRANSFORM_HXX_

#ifndef _CS237_HXX_
#error "cs237-transform.hxx should not be included directly"
#endif

namespace cs237 {

  //! create an isotropic (uniform) scaling matrix
  //! \param s the scaling factor
    template <typename T>
    __detail::mat4x4<T> scale (T s);

  //! apply isotropic (uniform) scaling to a matrix
  //! \param m the matrix to transform
  //! \param s the scaling factor
  //!
  //! The expression cs237::scale(m, s) is equivalent to m * cs237::scale(s)
    template <typename T>
    __detail::mat4x4<T> scale (__detail::mat4x4<T> const &m, T s);

  //! create an anisotropic (non-uniform) scaling matrix
  //! \param s the vector of scaling factors
    template <typename T>
    __detail::mat4x4<T> scale (__detail::vec3<T> const &s);

  //! apply anisotropic (non-uniform) scaling to a matrix
  //! \param m the matrix to transform
  //! \param s the vector of scaling factors
  //!
  //! The expression cs237::scale(m, s) is equivalent to m * cs237::scale(s)
    template <typename T>
    __detail::mat4x4<T> scale (__detail::mat4x4<T> const &m, __detail::vec3<T> const &s);

  //! create a rotation about the X axis matrix
  //! \param theta the angle of rotation in degrees
    template <typename T>
    __detail::mat4x4<T> rotateX (T theta);

  //! apply rotation about the X axis matrix to a matrix
  //! \param m the matrix to transform
  //! \param theta the angle of rotation in degrees
  //!
  //! The expression cs237::rotateX(m, theta) is equivalent to m * cs237::rotateX(theta)
    template <typename T>
    __detail::mat4x4<T> rotateX (__detail::mat4x4<T> const &m, T theta);

  //! create a rotation about the Y axis matrix
  //! \param theta the angle of rotation in degrees
    template <typename T>
    __detail::mat4x4<T> rotateY (T theta);

  //! apply rotation about the Y axis matrix to a matrix
  //! \param m the matrix to transform
  //! \param theta the angle of rotation in degrees
  //!
  //! The expression cs237::rotateY(m, theta) is equivalent to m * cs237::rotateY(theta)
    template <typename T>
    __detail::mat4x4<T> rotateY (__detail::mat4x4<T> const &m, T theta);

  //! create a rotation about the Z axis matrix
  //! \param theta the angle of rotation in degrees
    template <typename T>
    __detail::mat4x4<T> rotateZ (T theta);

  //! apply rotation about the Z axis matrix to a matrix
  //! \param m the matrix to transform
  //! \param theta the angle of rotation in degrees
  //!
  //! The expression cs237::rotateZ(m, theta) is equivalent to m * cs237::rotateZ(theta)
    template <typename T>
    __detail::mat4x4<T> rotateZ (__detail::mat4x4<T> const &m, T theta);

  //! create a rotation about an arbitrary axis matrix
  //! \param theta the angle of rotation in degrees
  //! \param axis the axis of rotation
    template <typename T>
    __detail::mat4x4<T> rotate (T theta, __detail::vec3<T> const &axis);

  //! apply a rotation about an arbitrary axis to a matrix
  //! \param m the matrix to transform
  //! \param theta the angle of rotation in degrees
  //! \param axis the axis of rotation
    template <typename T>
    __detail::mat4x4<T> rotate (__detail::mat4x4<T> const &m, T theta, __detail::vec3<T> const &axis);

  //! create a translation by the given offset vector
  //! \param offset the vector that defines the translation
    template <typename T>
    __detail::mat4x4<T> translate (__detail::vec3<T> const &offset);

  //! apply a translation by an offset vector to a matrix
  //! \param m the matrix to transform
  //! \param offset the vector that defines the translation
  //!
  //! The expression cs237::translate(m, offset) is equivalent to m * cs237::translate(offset)
    template <typename T>
    __detail::mat4x4<T> translate (__detail::mat4x4<T> const &m, __detail::vec3<T> const &offset);

  //! \brief build a a viewing matrix derived from an eye point, a reference point indicating the
  //!        center of the scene, and an up vector.
  //! \param[in] eye specifies the position of the eye point
  //! \param[in] center specifies the point that is being looked at
  //! \param[in] up specifies the up direction of the viewer
    template <typename T>
    __detail::mat4x4<T> lookAt (
	__detail::vec3<T> const &eye,
	__detail::vec3<T> const &center,
	__detail::vec3<T> const &up);

  //! set up a transformation matrix that produces a parallel projection
  //! \param left    specify the coordinates for the left vertical clipping plane
  //! \param right   specify the coordinates for the right vertical clipping plane
  //! \param bottom  specify the coordinates for the bottom horizontal clipping plane
  //! \param top     specify the coordinates for the top horizontal clipping plane
  //! \param nearVal specify the distance to the nearer depth clipping plane
  //! \param farVal  specify the distance to the farther depth clipping plane
  //!
  //! The \p nearVal and \p farVal values are negative if the plane is to be behind the viewer.
    template <typename T>
    __detail::mat4x4<T> ortho (
	T const &left, T const &right,
	T const &bottom, T const &top,
	T const &nearVal, T const &farVal);

  //! set up a transformation matrix that produces two-dimensional orthographic viewing region
    template <typename T>
    __detail::mat4x4<T> ortho2D (T const &left, T const &right, T const &bottom, T const &top);

  //! \brief specifies a perspective projection matrix
  //! \param[in] fov the vertical field of view; i.e., the angle in degrees between the XZ plane
  //!        and the top of the vew frustum.
  //! \param[in] aspect the aspect ratio (width/height)
  //! \param[in] zNear the distance from the viewer to the near clipping plane
  //! \param[in] zFar the distance from the viewer to the far clipping plane
    template <typename T>
    __detail::mat4x4<T> perspective (T const &fov, T const &aspect, T const &zNear, T const &zFar);

  //! specifies a viewing frustum into the world coordinate system
    template <typename T>
    __detail::mat4x4<T> frustum (T const &left, T const &right, T const &bottom, T const &top, T const &nearVal, T const &farVal);

} /* namespace cs237 */


#endif /* !_CS237_TRANSFORM_HXX_ */
