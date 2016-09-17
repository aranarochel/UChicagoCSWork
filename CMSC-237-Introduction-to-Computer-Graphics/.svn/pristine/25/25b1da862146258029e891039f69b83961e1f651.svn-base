/*! \file camera.cxx
 *
 * \author John Reppy
 *
 * The camera class encapsulates the current view and projection matrices.
 */

/* CMSC23700 Final Project sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "camera.hxx"

/***** class Camera member functions *****/

Camera::Camera ()
    : _errorFactor(-1)
{ }

// translate a point to the coordinate system that has the camera as the origin, but is
// oriented and scaled the same as the world coordinates.
cs237::vec3d Camera::translate (cs237::vec3d const &p) const
{
    return p - this->_pos;
}

// a transformation matrix that assumes that the camera is at the origin.
cs237::mat4x4f Camera::viewTransform () const
{
    return cs237::lookAt (
	cs237::vec3f(0.0f, 0.0f, 0.0f),
	this->_dir,
	this->_up);
}

// the projection transform for the camera
cs237::mat4x4f Camera::projTransform () const
{
    float n_e = this->_nearZ * tanf (this->_halfFOV);  // n/e
    return cs237::frustum(
	-n_e, n_e,
	-this->_aspect * n_e, this->_aspect * n_e,
	this->_nearZ, this->_farZ);
}

// update the camera for the aspect ratio of the given viewport.  This operations will change
// the aspect ratio, but not the field of view.
void Camera::setViewport (int wid, int ht)
{
    this->_errorFactor = -1.0f;  // mark the error factor as invalid
    this->_aspect = float(ht) / float(wid);
    this->_wid = wid;
}

// set the horizontal field of view in degrees
void Camera::setFOV (float angle)
{
    this->_errorFactor = -1.0f;
    this->_halfFOV = cs237::radians(0.5 * angle);
}

// set the near and far planes
void Camera::setNearFar (double nearZ, double farZ)
{
    assert ((0.0 < nearZ) && (nearZ < farZ));
    this->_nearZ = nearZ;
    this->_farZ = farZ;
}

// move the camera to a new position while maintaining its heading
void Camera::move (cs237::vec3d const &pos)
{
    this->_pos = pos;
}

// move the camera to a new position and heading, while maintaining
// its up vector
void Camera::move (cs237::vec3d const &pos, cs237::vec3d const &at)
{
    this->_pos = pos;
    this->_dir = normalize(cs237::toFloat(at - pos));
}

// move the camera to a new position, heading, and up vector
void Camera::move (cs237::vec3d const &pos, cs237::vec3d const &at, cs237::vec3d const &up)
{
    this->_pos = pos;
    this->_dir = cs237::toFloat(normalize(at - pos));
    this->_up = cs237::toFloat(normalize(up));
}

// change the direction of the camera
void Camera::look (cs237::vec3f const &dir)
{
    this->_dir = normalize(dir);
}

// change the direction of the camera
void Camera::look (cs237::vec3f const &dir, cs237::vec3f const &up)
{
    this->_dir = normalize(dir);
    this->_up = up;
}

// compute the screen-space error for a geometric error of size err at distance dist.
float Camera::screenError (float dist, float err) const
{
    if (this->_errorFactor < 0.0f) {
	this->_errorFactor = float(this->_wid) / (2.0 * tanf(this->_halfFOV));
    }
    return this->_errorFactor * (err / dist);

}

/***** Output *****/

std::ostream& operator<< (std::ostream& s, Camera const &cam)
{
    s << "Camera {"
	<< "\n  position =  " << cam.position()
	<< "\n  direction = " << cam.direction()
	<< "\n  up =        " << cam.up()
	<< "\n  near Z =    " << cam.near()
	<< "\n  far Z =     " << cam.far()
	<< "\n  aspect =    " << cam.aspect()
	<< "\n  fov =       " << cam.fov()
	<< "\n}";

    return s;
}

