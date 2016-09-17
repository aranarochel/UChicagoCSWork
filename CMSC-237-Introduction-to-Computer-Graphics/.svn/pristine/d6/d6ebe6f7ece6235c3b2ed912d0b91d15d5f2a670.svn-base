/*! \file g-buffer.cxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "g-buffer.hxx"


 GBuffer::GBuffer (uint32_t wid, uint32_t ht)
 	: _fbo(0), _posBuf(nullptr), _colBuf(nullptr), _normBuf(nullptr),_depthBuf(nullptr)
 {

	this->_wid = wid;
	this->_ht = ht;


 	// Creates a framebuffer object and assigns the id to fboId
	glGenFramebuffers(1, & this->_fbo); 

	// Make the the framebuffer in "fboId" the current framebuffer (GL_FRAMEBUFFER).  
	glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);

	// create textures
	this->_posBuf = new cs237::texture2D (
		GL_TEXTURE_2D, GL_RGB32F, this->_wid, this->_ht, GL_RGB, GL_FLOAT);
	this->_posBuf->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    this->_posBuf->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    this->_posBuf->Parameter (GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    this->_posBuf->Parameter (GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	this->_colBuf = new cs237::texture2D (
		GL_TEXTURE_2D, GL_RGB32F, this->_wid, this->_ht, GL_RGB, GL_FLOAT);
	this->_colBuf->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    this->_colBuf->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    this->_colBuf->Parameter (GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    this->_colBuf->Parameter (GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	this->_normBuf = new cs237::texture2D (
		GL_TEXTURE_2D, GL_RGB32F, this->_wid, this->_ht, GL_RGB, GL_FLOAT);
	this->_normBuf->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    this->_normBuf->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    this->_normBuf->Parameter (GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    this->_normBuf->Parameter (GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    this->_specBuf = new cs237::texture2D (
    	GL_TEXTURE_2D, GL_RGB32F, this->_wid, this->_ht, GL_RGB, GL_FLOAT);
    this->_specBuf->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    this->_specBuf->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    this->_specBuf->Parameter (GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    this->_specBuf->Parameter (GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   
	this->_depthBuf = new cs237::texture2D (
		GL_TEXTURE_2D, GL_DEPTH_COMPONENT, this->_wid, this->_ht, GL_DEPTH_COMPONENT, GL_FLOAT);

	// attach textures to framebuffer
	CS237_CHECK( glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->_posBuf->Id(), 0) );
	CS237_CHECK( glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 1, this->_colBuf->Id(), 0) );
	CS237_CHECK( glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 2, this->_normBuf->Id(), 0) );
	CS237_CHECK( glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 3, this->_specBuf->Id(), 0) );
	CS237_CHECK( glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->_depthBuf->Id(), 0) );

	// enable writing to all textures (except the depth texture)
	GLenum attachments[] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT0+1,GL_COLOR_ATTACHMENT0+2,GL_COLOR_ATTACHMENT0+3};
	CS237_CHECK(glDrawBuffers(4, attachments));
 	
	//check status and unbind fbo
	GLenum status = glCheckFramebufferStatus (GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "FBO  error, status = " << status << std::endl;
    exit (EXIT_FAILURE); 
	}

	// restore default FBO
	CS237_CHECK( glBindTexture (GL_TEXTURE_2D, 0) );
	CS237_CHECK (glBindFramebuffer(GL_FRAMEBUFFER, 0));
 }

 GBuffer::~GBuffer ()
{
    delete this->_posBuf;
    delete this->_normBuf;
    delete this->_colBuf;
    delete this->_depthBuf;
    delete this->_specBuf;
}

 // Bind the depth buffer for writing (i.e., rendering)
//
void GBuffer::BindForWriting ()
{
    CS237_CHECK( glBindFramebuffer (GL_FRAMEBUFFER, this->_fbo) );
    CS237_CHECK( glViewport (0, 0, this->_wid, this->_ht) );
}

// Bind the depth buffer for reading
//
void GBuffer::BindForReading (GLenum texUnit, int tex)
{
    CS237_CHECK( glActiveTexture (texUnit) );


    if (tex == 0){
    	this->_posBuf->Bind();
    }
    else if (tex == 1) {
    	this->_colBuf->Bind();
    }
    else if (tex == 2) {
    	this->_normBuf->Bind();
    }
    else if (tex ==3) {
    	this->_specBuf->Bind();
    }
    else {
    	this->_depthBuf->Bind();
    }

 
}

