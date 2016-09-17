/*! \file height-field.hxx
 *
 * \author John Reppy
 *
 * This file defines the HeightField class, which is a wrapper around the
 * height-field information in a scene.
 */

/* CMSC23700 Project 3 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _HEIGHT_FIELD_HXX_
#define _HEIGHT_FIELD_HXX_

#include "cs237.hxx"

class HeightField {
  public:

  //! construct a HeightField object
  //! \param file the name of the height-field file
  //! \param width the width (X dimension) covered by the ground in world-space coordinates
  //! \param height the height (Z dimension) covered by the ground in world-space coordinates
  //! \param vScale the vertical scaling (Y dimension) factor
  //! \param color the color for non-texturing modes
  //! \param cmap the color texture image
  //! \param nmap the normal-map texture image
    HeightField (
	std::string const file,
	float width, float height, float vScale,
	cs237::color3f const &color,
	cs237::image2d *cmap,
	cs237::image2d *nmap
	);

  //! the width of the ground object in world-space
    float Width () const { return 2.0f * this->_halfWid; }

  //! the height of the ground object in world-space
    float Height () const { return 2.0f * this->_halfHt; }

  //! the number of rows of data in the height-field
    uint32_t NumRows () const { return this->_img->height(); }

  //! the number of rows of data in the height-field
    uint32_t NumCols () const { return this->_img->width(); }

  //! the number of vertices in the mesh represented by the height field
    uint32_t NumVerts () const { return this->NumRows() * this->NumCols(); }

  //! return the vertex ID (i.e., the index into the image data) for the given
  //! row and column
    uint32_t IndexOf (uint32_t row, uint32_t col) const
    {
	assert ((row < this->NumRows()) && (col < this->NumCols()));
	return row * this->NumCols() + col;
    }

  //! return the height-field value at the given row and column
    uint16_t ValueAt (uint32_t row, uint32_t col) const;

  //! return the world-space vertex for the given row and column of the heightfield
    cs237::vec3f const VertexAt (int row, int col) const
    {
	return cs237::vec3f(
	    this->_scaleX * float(col) - this->_halfWid,
	    this->_scaleY * float(this->ValueAt(row, col)),
	    this->_scaleZ * float(row) - this->_halfHt);
    }

  //! return the world-space AABB for the height field
    cs237::AABBf BBox () const {
	return cs237::AABBf(
	    cs237::vec3f(-this->_halfWid, this->_minHt, -this->_halfHt),
	    cs237::vec3f( this->_halfWid, this->_maxHt,  this->_halfHt));
    }

  //! return the color for the ground in wireframe and flat-shading rendering modes
    cs237::color3f const &Color () const { return this->_color; }

  //! return the color map image for the ground
    cs237::image2d *ColorMap () const { return this->_colorMap; }

  //! return the normal map image for the ground
    cs237::image2d *NormalMap () const { return this->_normMap; }

  private:
    const cs237::image2d *_img;		// the underlying image data
    const float		_halfWid;	// half the width in world space
    const float		_halfHt;	// half the height in world space
    float		_minHt;		// the minimum elevation
    float		_maxHt;		// the maximum elevation
    const float		_scaleX;	// horizontal scaling factor in X dimension
    const float		_scaleY;	// vertical scaling factor (Y dimension)
    const float		_scaleZ;	// horizontal scaling factor in Z dimension
    const cs237::color3f _color;	//!< the color of the ground in wireframe, flat-shading,
					//!  or diffuse mode
    cs237::image2d	*_colorMap;	//!< the color texture image for the ground in texturing and
					//!  normal-mapping modes
    cs237::image2d	*_normMap;	//!< the normal-map texture image for the ground in
					//!  normal-mapping mode.

};

#endif // !_HEIGHT_FIELD_HXX_
