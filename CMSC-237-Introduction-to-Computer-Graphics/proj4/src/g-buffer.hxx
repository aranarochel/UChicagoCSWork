/*! \file g-buffer.hxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 3 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _G_BUFFER_HXX_
#define _G_BUFFER_HXX_

#include "cs237.hxx"

enum {
    COLOR_BUF = 0,
    COORD_BUF = 1,
    NORM_BUF = 2,
    DEPTH_BUF = 3
};

class GBuffer {
  public:
  //! constructor
    GBuffer (uint32_t wid, uint32_t ht);
    ~GBuffer ();

    void Resize (uint32_t wid, uint32_t ht);

  //! \brief make the G buffer be the current render target
  //!
  //! This function makes the G buffer be the current render target.
    void BindForWriting ();

  //! \brief binds the G texture to the provided texture unit
  //! \param texUnit the binding texture unit 
  //! \param enum for which texture to activate
    void BindForReading (GLenum texUnit, int tex);

    GLuint FBO () const { return this->_fbo; }


  private:
    GLuint	_fbo;		// the frame-buffer object

    uint32_t    _wid;   //!< the width of the buffer
    uint32_t    _ht;    //!< the height of the buffer


    /* The G-buffer textures */
    cs237::texture2D  *_posBuf;
    cs237::texture2D  *_colBuf;
    cs237::texture2D  *_normBuf;
    cs237::texture2D  *_depthBuf; 
    cs237::texture2D  *_specBuf;

};

#endif //! _G_BUFFER_HXX_
