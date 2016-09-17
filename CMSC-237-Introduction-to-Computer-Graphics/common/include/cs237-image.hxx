/*! \file cs237-image.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_IMAGE_HXX_
#define _CS237_IMAGE_HXX_

#ifndef _CS237_HXX_
#error "c237-image.hxx should not be included directly"
#endif

#include <fstream>

namespace cs237 {

    namespace __detail {

	class image_base {
	  public:
	  //! the number of dimensions (1, 2, or 3)
	    unsigned int nDims () const { return this->_nDims; }
	  //! the OpenGL format of the pixels.
	  //! \return the OpenGL format, which will be one of GL_RED, GL_RG, GL_RGB, GL_BGR
	  //! GL_RGBA, or GL_BGRA.
	    unsigned int format () const { return this->_format; }
	  //! the OpenGL type of the pixels
	  //! \return the OpenGL element type of the pixels, which should be one of
	  //!         GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT,
	  //!         GL_INT, GL_FLOAT, GL_UNSIGNED_BYTE_3_3_2, GL_UNSIGNED_BYTE_2_3_3_REV,
	  //!         GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_6_5_REV,
	  //!         GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_4_4_4_4_REV,
	  //!         GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_SHORT_1_5_5_5_REV,
	  //!         GL_UNSIGNED_INT_8_8_8_8, GL_UNSIGNED_INT_8_8_8_8_REV,
	  //!         GL_UNSIGNED_INT_10_10_10_2, or GL_UNSIGNED_INT_2_10_10_10_REV
	    unsigned int type () const { return this->_type; }
	  //! the data pointer
	    void *data () const { return this->_data; }
	  //! the total number of bytes of image data
	    size_t nBytes () const { return this->_nBytes; }

	  //! the number of channels (1, 2, 3, or 4)
	    unsigned int nChannels () const;

	  //! the number of bytes per pixel
	    size_t nBytesPerPixel () const;

	  protected:
	    uint32_t	_nDims;		//!< the number of dimensions (1 or 2)
	    GLenum	_format;	//!< the texture format; will be one of GL_RED (1 chan),
					//!< GL_RG (2 chan), GL_RGB (3 chan), GL_BGR (3 chan),
					//!< GL_RGBA (4 chan), or GL_BGRA (4 chan).
	    GLenum	_type;		//!< the representation type of the data; will be one of
					//!< GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT,
					//!< GL_UNSIGNED_INT, GL_INT, GL_FLOAT, etc.
	    size_t	_nBytes;	//!< size in bytes of image data
	    void	*_data;		//!< the raw image data

	    explicit image_base ()
		:  _nDims(0), _format(0), _type(0), _nBytes(0), _data(nullptr)
	    { }
	    explicit image_base (uint32_t nd)
		: _nDims(nd), _format(0), _type(0), _nBytes(0), _data(nullptr)
	    { }
	    explicit image_base (uint32_t nd, GLenum fmt, GLenum ty, size_t nPixels);

	    virtual ~image_base ();

	};

    } /* namespace __detail */

  /* 1D images */
    class image1d : public __detail::image_base {
      public:
      //! create and allocate space for an uninitialized image
      //! \param wid the width of the image
      //! \param fmt the image format, which should be one of GL_RED, GL_RG, GL_RGB, GL_BGR
      //!            GL_RGBA, or GL_BGRA.
      //! \param ty the type of the elements, which should be one of GL_UNSIGNED_BYTE
      //!           GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT,
      //!           GL_INT, GL_FLOAT, GL_UNSIGNED_BYTE_3_3_2, GL_UNSIGNED_BYTE_2_3_3_REV,
      //!           GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_6_5_REV,
      //!           GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_4_4_4_4_REV,
      //!           GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_SHORT_1_5_5_5_REV,
      //!           GL_UNSIGNED_INT_8_8_8_8, GL_UNSIGNED_INT_8_8_8_8_REV,
      //!           GL_UNSIGNED_INT_10_10_10_2, or GL_UNSIGNED_INT_2_10_10_10_REV
	image1d (uint32_t wid, GLenum fmt, GLenum ty);

      //! create and initialize an image from a PNG file.
      //! \param file the name of the PNG file
	image1d (std::string const &file);

      //! return the width of the image
	GLsizei width () const { return this->_wid; }

      //! Load the image into a 1D GL texture; this function also binds the current texture to texId
      //! \param[in] texId the GL texture ID for this image
	void texImage (GLuint texId);

      //! write the texture to a file in PNG format
      //! \param file the name of the PNG file
      //! \return true if successful, false otherwise
	bool write (const char *file);

      private:
	GLsizei _wid;		//!< the width of the image in pixels
    };

  /* 2D images */
    class image2d : public __detail::image_base {
      public:
      //! create and allocate space for an uninitialized image
      //! \param wid the width of the image
      //! \param ht the height of the image
      //! \param fmt the image format, which should be one of GL_RED, GL_RG, GL_RGB, GL_BGR
      //!            GL_RGBA, or GL_BGRA.
      //! \param ty the type of the elements, which should be one of GL_UNSIGNED_BYTE
      //!           GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT,
      //!           GL_INT, GL_FLOAT, GL_UNSIGNED_BYTE_3_3_2, GL_UNSIGNED_BYTE_2_3_3_REV,
      //!           GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_6_5_REV,
      //!           GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_4_4_4_4_REV,
      //!           GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_SHORT_1_5_5_5_REV,
      //!           GL_UNSIGNED_INT_8_8_8_8, GL_UNSIGNED_INT_8_8_8_8_REV,
      //!           GL_UNSIGNED_INT_10_10_10_2, or GL_UNSIGNED_INT_2_10_10_10_REV
	image2d (uint32_t wid, uint32_t ht, GLenum fmt, GLenum ty);

      //! create and initialize an image from a PNG file.
      //! \param file the name of the PNG file
      //! \param flip set to true if the image should be flipped vertically to match OpenGL
      //!        texture coordinates (default true)
	image2d (std::string const &file, bool flip = true);

      //! create and initialize an image from a PNG-format input stream
      //! \param inS the input stream
      //! \param flip set to true if the image should be flipped vertically to match OpenGL
      //!        texture coordinates (default true)
	image2d (std::ifstream &inS, bool flip = true);

      //! return the width of the image
	GLsizei width () const { return this->_wid; }

      //! return the height of the image
	GLsizei height () const { return this->_ht; }

      //! Load the image into a 2D GL texture; this function also binds the current texture to texId
      //! \param[in] target specifies the target texture. Must be GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE
      //! \param[in] texId the GL texture ID for this image
	void texImage (GLenum target, GLuint texId);

      //! write the texture to a file in PNG format
      //! \param file the name of the PNG file
      //! \param flip set to true if the image should be flipped vertically to match standard
      //!        image-file coordinates (default true)
      //! \return true if successful, false otherwise
      //!
      //! Note that the type of the image must be either GL_UNSIGNED_BYTE,
      //! or GL_UNSIGNED_SHORT to write it as a PNG file.
	bool write (const char *file, bool flip = true);

      //! write the texture to an output stream in PNG format
      //! \param outS the output stream to write the PNG image to
      //! \param flip set to true if the image should be flipped vertically to match standard
      //!        image-file coordinates (default true)
      //! \return true if successful, false otherwise
      //!
      //! Note that the type of the image must be either GL_UNSIGNED_BYTE,
      //! or GL_UNSIGNED_SHORT to write it to an output stream.
	bool write (std::ofstream &outS, bool flip = true);

      private:
	GLsizei _wid;		//!< the width of the image in pixels
	GLsizei _ht;		//!< the height of the image in pixels
    };

} /* namespace cs237 */

#endif /* !_CS237_IMAGE_HXX_ */
