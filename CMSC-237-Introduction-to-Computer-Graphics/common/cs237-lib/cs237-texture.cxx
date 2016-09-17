/*! \file cs237-texture.cxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Operations on texture values.
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

	inline GLuint genTexture ()
	{
	    GLuint id;
	    CS237_CHECK( glGenTextures (1, &id) );
	    return id;
	}

      /***** class texture_base member functions *****/

	texture_base::Obj::~Obj ()
	{
	    glDeleteTextures (1, &this->_id);
	}

	texture_base::texture_base (GLenum tgt)
	    : _target(tgt), _obj(std::make_shared<texture_base::Obj>(genTexture()))
	{
	    if (this->Id() == 0) {
		std::cerr << "Error creating texture: target = " << tgt << ", error = "
		    << glGetError() << std::endl;
		exit (1);
	    }
	}

	void texture_base::Bind ()
	{
	    CS237_CHECK( glBindTexture (this->_target, this->Id()) );
	}

	void texture_base::Parameter (GLenum param, GLenum value)
	{
	    CS237_CHECK( glTexParameteri (this->_target, param, value) );
	}

	void texture_base::Parameter (GLenum param, GLint value)
	{
	    CS237_CHECK( glTexParameteri (this->_target, param, value) );
	}

	void texture_base::Parameter (GLenum param, color4f value)
	{
	    CS237_CHECK( glTexParameterfv (this->_target, param, valuePtr(value)) );
	}

    } /* namespace __detail */

  /***** class texture1D member functions *****/

    texture1D::texture1D (image1d *img)
	: texture_base (GL_TEXTURE_1D)
    {
	assert (img != nullptr);
	img->texImage (this->Id());
    }

    texture1D::texture1D (GLenum target, GLenum ifmt, GLenum wid, GLenum fmt, GLenum ty)
	: texture_base (target)
    {
	this->Bind();
	CS237_CHECK( glTexImage1D (
	    target,		// target
	    0,			// level
	    ifmt,		// internal format (same as image format)
	    wid,		// width
	    0,			// border
	    fmt,		// image format
	    ty,			// image type
	    nullptr) );		// image data (no image)
    }

  /***** class texture2D member functions *****/

    texture2D::texture2D (GLenum target, image2d *img)
	: texture_base (target)
    {
	assert (img != nullptr);
      // Note: if the number of bytes in a row is not a multiple of 4, then we need to adjust the
      // unpacking alignment
	size_t bytesPerRow = img->width() * img->nBytesPerPixel();
	if ((bytesPerRow & 3) != 0) {
	    GLint alignment = (bytesPerRow & 1) ? 1 : 2;
	    CS237_CHECK (glPixelStorei (GL_UNPACK_ALIGNMENT, alignment) );
	    img->texImage (target, this->Id());
	    CS237_CHECK (glPixelStorei (GL_UNPACK_ALIGNMENT, 4) );  // restore default
	} else {
	    img->texImage (target, this->Id());
	}
    }

    texture2D::texture2D (GLenum target, GLenum ifmt, GLenum wid, GLenum ht, GLenum fmt, GLenum ty)
	: texture_base (target)
    {
	this->Bind();
	CS237_CHECK( glTexImage2D (
	    target,		// target
	    0,			// level
	    ifmt,		// internal format (same as image format)
	    wid,		// width
	    ht,			// height
	    0,			// border
	    fmt,		// image format
	    ty,			// image type
	    nullptr) );		// image data (no image)
    }

} /* namespace cs237 */
