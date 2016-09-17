/*! \file depth-buffer.hxx
 *
 * \author John Reppy
 *
 * \brief Wrapper code around OpenGL frame-buffer objects to support
 * rendering to a depth buffer.
 */

/* CMSC23700 Project 3 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _DEPTH_BUFFER_HXX_
#define _DEPTH_BUFFER_HXX_

#include "cs237.hxx"


class DepthBuffer {
  public:

  //! \brief construct and initialize a new depth buffer of the give size
  //! \param wid the width of the buffer (should be power of 2)
  //! \param ht the height of the buffer (should be power of 2)
  //! \return the new buffer
  //!
  //! This function allocates and initializes a frame-buffer object
  //! and associated depth texture.  The texture is initialized with the
  //! following parameters:
  //!   - the filter parameters are set to \c GL_LINEAR
  //!   - the wrap parameters are set to \c GL_CLAMP_TO_EDGE
  //!   - the texture mode is set to \c GL_INTENSITY
  //!   - the texture compare mode is set to \c GL_COMPARE_R_TO_TEXTURE
  //!   - the texture compare function is set to \c GL_LEQUAL
  //! .
    DepthBuffer (uint32_t winWidth, uint32_t winHeight);

  //! destructor; this will free the underlying texture(s)
    ~DepthBuffer ();

  //! return the width of the buffer
    uint32_t Width() const { return this->_wid; }

  //! return the height of the buffer
    uint32_t Height() const { return this->_ht; }

  //! \brief make the depth buffer be the current render target
  //!
  //! This function makes the depth buffer be the current render target.
    void BindForWriting ();

  //! \brief binds the depth texture to the provided texture unit
  //! \param texUnit the binding texture unit 
    void BindForReading (GLenum texUnit);

  private:
    GLuint		_fboId;		//!< the frame-buffer's ID
    cs237::texture2D	*_depthBuf;	//!< the depth renderbuffer
    uint32_t		_wid;		//!< the width of the buffer
    uint32_t		_ht;		//!< the height of the buffer

};

#endif // !_DEPTH_BUFFER_HXX_
