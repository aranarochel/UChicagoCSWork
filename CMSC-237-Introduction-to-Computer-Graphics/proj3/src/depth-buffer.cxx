/*! \file depth-buffer.cxx
 *
 * \author John Reppy
 *
 * This implementation is based on code written by Lamont Samuels for
 * the Winter 2014 version of this course.
 */

/* CMSC23700 Project 3 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "depth-buffer.hxx"

DepthBuffer::DepthBuffer (uint32_t winWidth, uint32_t winHeight)
    : _fboId(0), _depthBuf(nullptr), _wid(winWidth), _ht(winHeight)
{
  // create the framebuffer
    CS237_CHECK( glGenFramebuffers(1, &this->_fboId) );    
    CS237_CHECK( glBindFramebuffer(GL_FRAMEBUFFER, this->_fboId) );

  // Generate a texture to represent the depth texture
    this->_depthBuf = new cs237::texture2D (
	GL_TEXTURE_2D, GL_DEPTH_COMPONENT, this->_wid, this->_ht,
	GL_DEPTH_COMPONENT, GL_FLOAT);
    this->_depthBuf->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    this->_depthBuf->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    this->_depthBuf->Parameter (GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    this->_depthBuf->Parameter (GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    this->_depthBuf->Parameter (GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    this->_depthBuf->Parameter (GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    CS237_CHECK( glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->_depthBuf->Id(), 0) );

  // check that the FBO is complete
    GLenum status = glCheckFramebufferStatus (GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
	std::cerr << "Depth Buffer error, status = " << status << std::endl;
        exit (EXIT_FAILURE); 
    }

  // cleanup
    CS237_CHECK( glBindTexture (GL_TEXTURE_2D, 0) );
    CS237_CHECK( glBindFramebuffer (GL_FRAMEBUFFER, 0) );

}

DepthBuffer::~DepthBuffer ()
{
    delete this->_depthBuf;
    /* what about _fbo? */
}

// Bind the depth buffer for writing (i.e., rendering)
//
void DepthBuffer::BindForWriting ()
{
    CS237_CHECK( glBindFramebuffer (GL_FRAMEBUFFER, this->_fboId) );
    CS237_CHECK( glViewport (0, 0, this->_wid, this->_ht) );
}

// Bind the depth buffer for reading
//
void DepthBuffer::BindForReading (GLenum texUnit)
{
    CS237_CHECK( glActiveTexture (texUnit) );
    this->_depthBuf->Bind();
}
