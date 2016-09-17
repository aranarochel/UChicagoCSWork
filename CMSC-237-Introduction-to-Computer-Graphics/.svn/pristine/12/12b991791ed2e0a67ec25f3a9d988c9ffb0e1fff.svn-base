/*! \file axes.hxx
 *
 * These classes support drawing directional vectors and axes as lines to help with
 * debugging.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _AXES_HXX_
#define _AXES_HXX_

#include "cs237.hxx"

//! a class for drawing the XYZ axes
class Axes {
  public:
    Axes (float scale = 1.0f);
    ~Axes ();

  //! draw the axes
  //! \param projMat the projection matrix
  //! \param viewMat the world-space to eye-space view transform
    void Draw (cs237::mat4f projMat, cs237::mat4f viewMat);

  private:
    GLuint		_vaoId;		//! VAO for the axes data

};

//! A class for drawing a directional line segment
class Line {
  public:
    Line (cs237::vec3f dir, cs237::color3f color, float length = 1.0f);
    Line (cs237::vec3f p1, cs237::vec3f p2, cs237::color3f color);
    ~Line ();

  //! draw the line
  //! \param projMat the projection matrix
  //! \param viewMat the world-space to eye-space view transform
    void Draw (cs237::mat4f projMat, cs237::mat4f viewMat);

  private:
    GLuint		_vaoId;		//! VAO for the axes data

};

#endif // !_AXES_HXX_