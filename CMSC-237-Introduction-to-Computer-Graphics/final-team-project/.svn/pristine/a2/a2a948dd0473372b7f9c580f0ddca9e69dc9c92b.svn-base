/*! \file buffer-cache.hxx
 *
 * The BufferCache provides a cache for VAO objects.
 *
 * \author John Reppy
 */

/* CMSC23700 Final Project sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _BUFFER_CACHE_HXX_
#define _BUFFER_CACHE_HXX_

#include "cs237.hxx"
#include <vector>

//! A wrapper for an OpenGL VAO.  Note that we only store vertex data (see struct Vertex)
//! from the chunk.
struct VAO {
    GLuint	_id;		//!< the OpenGL VAO ID
    GLuint	_vBuf;		//!< the OpenGL vertex buffer ID
    GLuint	_iBuf;		//!< the OpenGL element buffer ID
    GLuint	_nIndices;	//!< the number of indices in the element buffer
    bool	_inUse;		//!< true when this VAO is assigned to a chunk

    VAO();
    ~VAO();

  //! load the contents of a VAO with the data for a chunk
  //! \param[in] chunk the source of data to load in the VAO
    void Load (struct Chunk const &chunk);

  //! render the contents of the VAO using the current OpenGL state.  The VAO is rendered as
  //! a triangle strip.
    void Render ()
    {
	CS237_CHECK( glBindVertexArray (this->_id) );
	CS237_CHECK( glDrawElements (GL_TRIANGLE_STRIP, this->_nIndices, GL_UNSIGNED_SHORT, 0) );
	CS237_CHECK( glBindVertexArray (0) );
    }

};

//! A cache of VAO objects
class BufferCache {
  public:

  //! constructor
    BufferCache ();
  //! destructor
    ~BufferCache ();

  //! acquire a VAO object from the cache for use.
    VAO *Acquire ();
  //! release an unused VAO back to the cache
    void Release (VAO *vao);

  private:
    std::vector<VAO *>	_freeList;	//!< the list of free VAO objects
};

#endif // !_BUFFER_CACHE_HXX_
