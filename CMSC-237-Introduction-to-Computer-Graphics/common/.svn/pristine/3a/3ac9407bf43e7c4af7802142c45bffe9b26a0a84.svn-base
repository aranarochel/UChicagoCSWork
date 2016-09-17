/*! \file cs237-image.cxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Operations on image values.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "png.h"

namespace cs237 {

  //! \brief helper function to convert texture format to number of channels per pixel
    static uint32_t numChannels (GLenum fmt)
    {
	switch (fmt) {
	  case GL_RED: return 1;
	  case GL_RG: return 2;
	  case GL_RGB:
	  case GL_BGR: return 3;
	  case GL_RGBA:
	  case GL_BGRA: return 4;
	  default:
	    std::cerr << "Error: invalid format " << fmt << " specified for image" << std::endl;
	    exit (1);
	}

    } /* numChannels */

  //! \brief helper function to convert element type to size in bytes
    static size_t sizeOfType (GLenum ty)
    {
	switch (ty) {
	  case GL_UNSIGNED_BYTE:
	  case GL_BYTE:
	    return 1;
	  case GL_UNSIGNED_SHORT:
	  case GL_SHORT:
	    return 2;
	  case GL_UNSIGNED_INT:
	  case GL_INT:
	  case GL_FLOAT:
	    return 4;
	  case GL_UNSIGNED_BYTE_3_3_2:
	  case GL_UNSIGNED_BYTE_2_3_3_REV:
	    return 1;
	  case GL_UNSIGNED_SHORT_5_6_5:
	  case GL_UNSIGNED_SHORT_5_6_5_REV:
	  case GL_UNSIGNED_SHORT_4_4_4_4:
	  case GL_UNSIGNED_SHORT_4_4_4_4_REV:
	  case GL_UNSIGNED_SHORT_5_5_5_1:
	  case GL_UNSIGNED_SHORT_1_5_5_5_REV:
	    return 2;
	  case GL_UNSIGNED_INT_8_8_8_8:
	  case GL_UNSIGNED_INT_8_8_8_8_REV:
	  case GL_UNSIGNED_INT_10_10_10_2:
	  case GL_UNSIGNED_INT_2_10_10_10_REV:
	    return 4;
	  default:
	    std::cerr << "Error: invalid type " << ty << " specified for image" << std::endl;
	    exit (1);
	}
    } /* sizeOfType */

  //! \brief read function wrapper around an istream.
    static void readData (png_struct *pngPtr, png_bytep data, png_size_t length)
    {
	std::istream *inS = reinterpret_cast<std::istream*>(png_get_io_ptr(pngPtr));
	inS->read(reinterpret_cast<char *>(data), length);
	if (! inS->good()) {
#if ((PNG_LIBPNG_VER_MAJOR == 1) && (PNG_LIBPNG_VER_MINOR < 5))
	    longjmp(pngPtr->jmpbuf, 1);
#else
	    png_longjmp (pngPtr, 1);
#endif
	}
    }

  //! \brief helper function to read a PNG image from an input stream
  //! \param inS the imput stream
  //! \param flip true if the rows of the image should be flipped to match OpenGL coordinates
  //! \param widOut output variable for the image width
  //! \param htOut output variable for the image height (nullptr for 1D images)
  //! \param fmtOut output variable for the OpenGL pixel format
  //! \param tyOut output variable for the OpenGL pixel type
  //! \return a pointer to the image data, or nullptr on error
    void *readPNG (std::ifstream &inS, bool flip, GLsizei *widOut, GLsizei *htOut, GLenum *fmtOut, GLenum *tyOut)
    {
      /* check PNG signature */
	unsigned char sig[8];
	inS.read (reinterpret_cast<char *>(sig), sizeof(sig));
	if (! inS.good()) {
#ifndef NDEBUG
	    std::cerr << "readPNG: I/O error reading header" << std::endl;
#endif
	    inS.close();
	    return nullptr;
	}
	if (png_sig_cmp(sig, 0, 8)) {
#ifndef NDEBUG
	    std::cerr << "readPNG: bogus header" << std::endl;
#endif
	    inS.close();
	    return nullptr;
	}

      /* setup read structures */
	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if (pngPtr == nullptr) {
#ifndef NDEBUG
	    std::cerr << "readPNG: error creating read_struct" << std::endl;
#endif
	    inS.close();
	    return nullptr;
	}
	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (infoPtr == nullptr) {
#ifndef NDEBUG
	    std::cerr << "readPNG: error creating info_struct" << std::endl;
#endif
	    png_destroy_read_struct(&pngPtr, nullptr, nullptr);
	    inS.close();
	    return nullptr;
	}
	png_infop endPtr = png_create_info_struct(pngPtr);
	if (!endPtr) {
#ifndef NDEBUG
	    std::cerr << "readPNG: error creating info_struct" << std::endl;
#endif
	    png_destroy_read_struct (&pngPtr, &infoPtr, nullptr);
	    inS.close();
	    return nullptr;
	}

      /* error handler */
	if (setjmp (png_jmpbuf(pngPtr))) {
#ifndef NDEBUG
	    std::cerr << "readPNG: I/O error" << std::endl;
#endif
	    png_destroy_read_struct (&pngPtr, &infoPtr, &endPtr);
	    inS.close();
	    return nullptr;
	}

      /* set up input */
	png_set_read_fn (pngPtr, reinterpret_cast<void *>(&inS), readData);

      /* let the PNG library know that we already checked the signature */
	png_set_sig_bytes (pngPtr, 8);

      /* get file info */
	png_uint_32 width, height, bytesPerPixel;
	int bitDepth, colorType;
	png_read_info (pngPtr, infoPtr);
	png_get_IHDR (pngPtr, infoPtr, &width, &height,
	    &bitDepth, &colorType, 0 /* interlace type */,
	    0 /* compression type */, 0 /* filter method */);

	GLenum fmt, ty;
	switch (colorType) {
	  case PNG_COLOR_TYPE_GRAY:
	    fmt = GL_RED;
	    ty = GL_UNSIGNED_BYTE;
	    bytesPerPixel = 1;
	    if (bitDepth < 8) {
		png_set_expand_gray_1_2_4_to_8(pngPtr);
	    }
	    else if (bitDepth == 16) {
	      // PNG files store date in network byte order (big-endian), but the x86 is little-endian
		png_set_swap (pngPtr);
		ty = GL_UNSIGNED_SHORT;
		bytesPerPixel = 2;
	    }
	    break;
	  case PNG_COLOR_TYPE_GRAY_ALPHA:
	    fmt = GL_RG;
	    ty = GL_UNSIGNED_BYTE;
	    bytesPerPixel = 2;
	    if (bitDepth == 16) {
	      // PNG files store date in network byte order (big-endian), but the x86 is little-endian
		png_set_swap (pngPtr);
		bytesPerPixel = 4;
		ty = GL_UNSIGNED_SHORT;
	    }
	    break;
	  case PNG_COLOR_TYPE_PALETTE:
	    fmt = GL_RGB;
	    ty = GL_UNSIGNED_BYTE;
	    bytesPerPixel = 3;
	    png_set_palette_to_rgb (pngPtr);
	    break;
	  case PNG_COLOR_TYPE_RGB:
	    fmt = GL_RGB;
	    ty = GL_UNSIGNED_BYTE;
	    bytesPerPixel = 3;
	    if (bitDepth == 16) {
	      // PNG files store date in network byte order (big-endian), but the x86 is little-endian
		png_set_swap (pngPtr);
		bytesPerPixel = 12;
		ty = GL_UNSIGNED_SHORT;
	    }
	    break;
	  case PNG_COLOR_TYPE_RGB_ALPHA:
	    fmt = GL_RGBA;
	    ty = GL_UNSIGNED_BYTE;
	    bytesPerPixel = 4;
	    if (bitDepth == 16) {
	      // PNG files store date in network byte order (big-endian), but the x86 is little-endian
		png_set_swap (pngPtr);
		bytesPerPixel = 8;
		ty = GL_UNSIGNED_SHORT;
	    }
	    break;
	  default:
#ifndef NDEBUG
	    std::cerr << "unknown color type " << colorType << std::endl;
#endif
	    png_destroy_read_struct (&pngPtr, &infoPtr, (png_infopp)0);
	    inS.close();
	    return nullptr;
	}

      /* sanity check the image dimensions: max size is 20k x 20k */
	if ((20*1024 < width) || (20*1024 < height)) {
#ifndef NDEBUG
	    std::cerr << "readPNG: image too large" << std::endl;
#endif
	    inS.close();
	    return nullptr;
	}

      /* allocate image data */
	int bytesPerRow = bytesPerPixel * width;
	png_byte *img = (png_bytep) std::malloc (height * bytesPerRow);
	if (img == nullptr) {
#ifndef NDEBUG
	    std::cerr << "readPNG: unable to allocate image" << std::endl;
#endif
	    png_destroy_read_struct (&pngPtr, &infoPtr, &endPtr);
	    inS.close();
	    return nullptr;
	}

	png_bytepp rowPtrs = new png_bytep[height];
	if (flip) {
	  /* setup row pointers so that the texture has OpenGL orientation */
	    for (png_uint_32 i = 1;  i <= height;  i++)
		rowPtrs[height - i] = img + (i-1)*bytesPerRow;
	}
	else {
	    for (png_uint_32 i = 0;  i < height;  i++)
		rowPtrs[i] = img + i*bytesPerRow;
	}

      /* read the image */
	png_read_image(pngPtr, rowPtrs);

      /* Clean up. */
	png_destroy_read_struct (&pngPtr, &infoPtr, &endPtr);
	delete rowPtrs;

	if ((htOut == nullptr) && (height > 1)) {
	    width *= height;
	}

	*widOut = width;
	if (htOut != nullptr) *htOut = height;
	*fmtOut = fmt;
	*tyOut = ty;

	return img;

    } /* readPNG */

  //! \brief write function wrapper around an ostream.
    static void writeData (png_struct *pngPtr, png_bytep data, png_size_t length)
    {
	std::ostream *outS = reinterpret_cast<std::ostream*>(png_get_io_ptr(pngPtr));
	outS->write(reinterpret_cast<char *>(data), length);
	if (! outS->good()) {
#if ((PNG_LIBPNG_VER_MAJOR == 1) && (PNG_LIBPNG_VER_MINOR < 5))
	    longjmp(pngPtr->jmpbuf, 1);
#else
	    png_longjmp (pngPtr, 1);
#endif
	}
    }

  //! \brief flush function wrapper around an ostream.
    static void flushData (png_struct *pngPtr)
    {
	std::ostream *outS = reinterpret_cast<std::ostream*>(png_get_io_ptr(pngPtr));
	outS->flush();
	if (! outS->good()) {
#if ((PNG_LIBPNG_VER_MAJOR == 1) && (PNG_LIBPNG_VER_MINOR < 5))
	    longjmp(pngPtr->jmpbuf, 1);
#else
	    png_longjmp (pngPtr, 1);
#endif
	}
    }

  //! \brief helper function to write a PNG file to an output stream
  //! \param outS the output stream
  //! \param flip true if the rows of the image should be flipped to match OpenGL
  //! \param wid the image width
  //! \param ht the image height (1 for 1D images)
  //! \param fmt the OpenGL pixel format
  //! \param ty the OpenGL pixel type
  //! \param data a pointer to the image data
  //! \return true if the write is successful; otherwise false on error.
    bool writePNG (std::ofstream &outS, bool flip, GLsizei wid, GLsizei ht, GLenum fmt, GLenum ty, void *data)
    {
      /* setup write structures */
	png_structp pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if (pngPtr == nullptr) {
#ifndef NDEBUG
	    std::cerr << "writePNG: error creating write_struct" << std::endl;
#endif
	    return false;
	}
	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (infoPtr == nullptr) {
#ifndef NDEBUG
	    std::cerr << "writePNG: error creating info_struct" << std::endl;
#endif
	    png_destroy_write_struct(&pngPtr, nullptr);
	    return false;
	}

      /* error handler */
	if (setjmp (png_jmpbuf(pngPtr))) {
#ifndef NDEBUG
	    std::cerr << "writePNG: I/O error" << std::endl;
#endif
	    png_destroy_write_struct (&pngPtr, &infoPtr);
	    return false;
	}

      /* set up output functions */
	png_set_write_fn (pngPtr, reinterpret_cast<void *>(&outS), writeData, flushData);

      /* determine image properties */
	int nChannels, bitDepth, colorTy, bytesPerRow;
	int transforms = PNG_TRANSFORM_IDENTITY;
	switch (fmt) {
	  case GL_RED:
	    nChannels = 1;
	    colorTy = PNG_COLOR_TYPE_GRAY;
	    break;
	  case GL_RG:
	    nChannels = 2;
	    colorTy = PNG_COLOR_TYPE_GRAY_ALPHA;
	    break;
	  case GL_RGB:
	    nChannels = 3;
	    colorTy = PNG_COLOR_TYPE_RGB;
	    break;
	  case GL_BGR:
	    nChannels = 3;
	    colorTy = PNG_COLOR_TYPE_RGB;
	    transforms |= PNG_TRANSFORM_BGR;
	    break;
	  case GL_RGBA:
	    nChannels = 4;
	    colorTy = PNG_COLOR_TYPE_RGB_ALPHA;
	    break;
	  case GL_BGRA:
	    nChannels = 4;
	    colorTy = PNG_COLOR_TYPE_RGB_ALPHA;
	    transforms |= PNG_TRANSFORM_BGR;
	    break;
	  default:
	    std::cerr << "writePNG: invalid format " << fmt << std::endl;
	    return false;
	}
	switch (ty) {
	  case GL_UNSIGNED_BYTE:
	    bytesPerRow = wid * nChannels;
	    bitDepth = 8;
	    break;
	  case GL_UNSIGNED_SHORT:
	  // PNG files store date in network byte order (big-endian), but the x86 is little-endian
	    transforms |= PNG_TRANSFORM_SWAP_ENDIAN;
//	    png_set_swap (pngPtr);
	    bytesPerRow = 2 * wid * nChannels;
	    bitDepth = 16;
	    break;
/* FIXME: support for other pixel types */
	  case GL_BYTE:
	  case GL_SHORT:
	  case GL_UNSIGNED_INT:
	  case GL_INT:
	  case GL_FLOAT:
	  case GL_UNSIGNED_BYTE_3_3_2:
	  case GL_UNSIGNED_BYTE_2_3_3_REV:
	  case GL_UNSIGNED_SHORT_5_6_5:
	  case GL_UNSIGNED_SHORT_5_6_5_REV:
	  case GL_UNSIGNED_SHORT_4_4_4_4:
	  case GL_UNSIGNED_SHORT_4_4_4_4_REV:
	  case GL_UNSIGNED_SHORT_5_5_5_1:
	  case GL_UNSIGNED_SHORT_1_5_5_5_REV:
	  case GL_UNSIGNED_INT_8_8_8_8:
	  case GL_UNSIGNED_INT_8_8_8_8_REV:
	  case GL_UNSIGNED_INT_10_10_10_2:
	  case GL_UNSIGNED_INT_2_10_10_10_REV:
	  default:
	    std::cerr << "writePNG: invalid/unsupported pixel type " << ty << std::endl;
	    return false;
	}

	png_set_IHDR (pngPtr, infoPtr, wid, ht, bitDepth, colorTy,
	    PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	png_bytep img = reinterpret_cast<png_bytep>(data);
	png_bytepp rowPtrs = new png_bytep[ht];
	if (flip) {
	  /* setup row pointers so that the texture has OpenGL orientation */
	    for (int i = 1;  i <= ht;  i++)
		rowPtrs[ht - i] = img + (i-1)*bytesPerRow;
	}
	else {
	    for (int i = 0;  i < ht;  i++)
		rowPtrs[i] = img + i*bytesPerRow;
	}
	png_set_rows (pngPtr, infoPtr, rowPtrs);

	png_write_png (pngPtr, infoPtr, transforms, nullptr);

      /* Clean up. */
	png_destroy_write_struct (&pngPtr, &infoPtr);
	delete rowPtrs;
	outS.flush();

	return true;

    } /* writePNG */

  /***** virtual base class image_base member functions *****/

    namespace __detail {

	image_base::image_base (uint32_t nd, GLenum fmt, GLenum ty, size_t npixels)
	    : _nDims(nd), _format(fmt), _type(ty), _nBytes(numChannels(fmt) * npixels * sizeOfType(ty))
	{
	    this->_data = std::malloc(this->_nBytes);
	}

	image_base::~image_base () {
	    if (this->_data != nullptr)
		std::free(this->_data);
	}

	unsigned int image_base::nChannels () const
	{
	    return numChannels (this->_format);
	}

	size_t image_base::nBytesPerPixel () const
	{
	    return sizeOfType (this->_type);
	}

    } /* namespace __detail */


  /***** class image1d member functions *****/

    image1d::image1d (uint32_t wid, GLenum fmt, GLenum ty)
	: __detail::image_base (1, fmt, ty, wid), _wid(wid)
    { }

    image1d::image1d (std::string const &file)
	: __detail::image_base (1)
    {
      // open the image file for reading
	std::ifstream inS(file, std::ifstream::in | std::ifstream::binary);
	if (inS.fail()) {
#ifndef NDEBUG
	    std::cerr << "image2d::image1d: unable to open \"" << file << "\"" << std::endl;
#endif
	    exit (1);
	}

	this->_data = readPNG(inS, false, &this->_wid, nullptr, &this->_format, &this->_type);
	if (this->_data == nullptr) {
	    std::cerr << "image2d::image1d: unable to load image file \"" << file << "\"" << std::endl;
	    exit (1);
	}
	this->_nBytes = numChannels(this->_format) * this->_wid * sizeOfType(this->_type);

	inS.close();
    }

  // Load the image into a 1D GL texture
    void image1d::texImage (GLuint texId)
    {
	CS237_CHECK( glBindTexture (GL_TEXTURE_1D, texId) );
	CS237_CHECK( glTexImage1D (
	    GL_TEXTURE_1D,	// target
	    0,			// level
	    this->_format,	// internal format (same as image format)
	    this->_wid,		// width
	    0,			// border
	    this->_format,	// image format
	    this->_type,	// image type
	    this->_data) );	// image data

    }

  // write the image
    bool image1d::write (const char *file)
    {
      // open the image file for writing
	std::ofstream outS(file, std::ofstream::out | std::ifstream::binary);
	if (outS.fail()) {
#ifndef NDEBUG
	    std::cerr << "image1d::write: unable to open \"" << file << "\"" << std::endl;
#endif
	    return false;
	}

	bool sts = writePNG (outS, false, this->_wid, 1, this->_format, this->_type, this->_data);
	
	outS.close();

	return sts;
    }


  /***** class image2d member functions *****/

    image2d::image2d (uint32_t wid, uint32_t ht, GLenum fmt, GLenum ty)
	: __detail::image_base (2, fmt, ty, wid * ht), _wid(wid), _ht(ht)
    { }

    image2d::image2d (std::string const &file, bool flip)
	: __detail::image_base (2)
    {
      // open the image file for reading
	std::ifstream inS(file, std::ifstream::in | std::ifstream::binary);
	if (inS.fail()) {
#ifndef NDEBUG
	    std::cerr << "image2d::image2d: unable to open \"" << file << "\"" << std::endl;
#endif
	    exit (1);
	}

	this->_data = readPNG(inS, flip, &this->_wid, &this->_ht, &this->_format, &this->_type);
	if (this->_data == nullptr) {
	    std::cerr << "image2d::image2d: unable to load image file \"" << file << "\"" << std::endl;
	    exit (1);
	}
	this->_nBytes = numChannels(this->_format) * this->_wid * this->_ht * sizeOfType(this->_type);

	inS.close();
    }

    image2d::image2d (std::ifstream &inS, bool flip)
	: __detail::image_base (2)
    {
	this->_data = readPNG(inS, flip, &this->_wid, &this->_ht, &this->_format, &this->_type);
	if (this->_data == nullptr) {
	    std::cerr << "image2d::image2d: unable to load 2D image" << std::endl;
	    exit (1);
	}
	this->_nBytes = numChannels(this->_format) * this->_wid * this->_ht * sizeOfType(this->_type);
    }

  // Load the image into a GL texture
    void image2d::texImage (GLenum target, GLuint texId)
    {
	assert ((target = GL_TEXTURE_2D) || (target == GL_TEXTURE_RECTANGLE));

	CS237_CHECK( glBindTexture (target, texId) );
	CS237_CHECK( glTexImage2D (
	    target,		// target
	    0,			// level
	    this->_format,	// internal format (same as image format)
	    this->_wid,		// width
	    this->_ht,		// height
	    0,			// border
	    this->_format,	// image format
	    this->_type,	// image type
	    this->_data) );	// image data
    }

  // write the image to a file
    bool image2d::write (const char *file, bool flip)
    {
      // open the image file for writing
	std::ofstream outS(file, std::ofstream::out | std::ifstream::binary);
	if (outS.fail()) {
#ifndef NDEBUG
	    std::cerr << "image2d::write: unable to open \"" << file << "\"" << std::endl;
#endif
	    return false;
	}

	bool sts = writePNG (outS, flip, this->_wid, this->_ht, this->_format, this->_type, this->_data);
	
	outS.close();

	return sts;
    }

  // write the image to an output stream
    bool image2d::write (std::ofstream &outS, bool flip)
    {
	bool sts = writePNG (outS, flip, this->_wid, this->_ht, this->_format, this->_type, this->_data);

	return sts;
    }

} /* namespace cs237 */
