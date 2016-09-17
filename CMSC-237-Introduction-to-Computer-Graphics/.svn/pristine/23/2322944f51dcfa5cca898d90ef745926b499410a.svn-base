/*! \file buffer-cache.cxx
 *
 * \author John Reppy
 */

/* CMSC23700 Final Project sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "buffer-cache.hxx"
#include "map-cell.hxx"

#define VERT_LOC	0

/**** struct VAO member functions *****/

VAO::VAO()
{
    CS237_CHECK( glGenVertexArrays (1, &this->_id) );
    GLuint buf[2];
    CS237_CHECK( glGenBuffers (2, buf) );

    this->_vBuf = buf[0];
    this->_iBuf = buf[1];
    this->_nIndices = 0;
    this->_inUse = false;

}

VAO::~VAO()
{
}

// load data from the chunk
void VAO::Load (struct Chunk const &chunk)
{
    assert (this->_inUse);

    CS237_CHECK( glBindVertexArray (this->_id) );

  // setup the vertex array (4 shorts per vertex)
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, this->_vBuf) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, chunk.vSize(), chunk._vertices, GL_DYNAMIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (VERT_LOC, 4, GL_SHORT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (VERT_LOC) );

  // setup index array
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->_iBuf) );
    CS237_CHECK( glBufferData (GL_ELEMENT_ARRAY_BUFFER, chunk.iSize(), chunk._indices, GL_DYNAMIC_DRAW) );
    this->_nIndices = chunk._nIndices;

    CS237_CHECK( glBindVertexArray (0) );

}

/**** class BufferCache member functions *****/

BufferCache::BufferCache ()
    : _freeList()
{
    this->_freeList.reserve(256);
}

BufferCache::~BufferCache ()
{
}

VAO *BufferCache::Acquire ()
{
    VAO *vao;
    if (this->_freeList.size() == 0) {
      // we need to allocate a new VAO
	vao = new VAO();
    }
    else {
	vao = this->_freeList.back();
	this->_freeList.pop_back();
    }
    assert (! vao->_inUse);
    vao->_inUse = true;

    return vao;

}

void BufferCache::Release (VAO *vao)
{
    assert (vao->_inUse);
    vao->_inUse = false;
    this->_freeList.push_back(vao);
}
