/*! \file mesh.cxx
 *
 * \author John Reppy
 */

/* CMSC23700 Lab 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "mesh.hxx"

/*! The locations of the standard mesh attributes.  The layout directives in the shaders
 * should match these values.
 */
const GLint CoordAttrLoc = 0; //!< location of vertex coordinates attribute
const GLint NormAttrLoc = 1;  //!< location of vertex normal attribute
const GLint TexCoordAttrLoc = 2;  //!< location of texture coordniate attribute

//! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
//! loaded separately.
Mesh::Mesh(GLenum p)
    : vbufId(0), ebufId(0), prim(p), nIndices(0)
{
  // generate the vertex array object for the mesh
    CS237_CHECK( glGenVertexArrays (1, &(this->vaoId)));
}

//! initialize the vertex data buffers for the mesh
void Mesh::LoadVertices (int nVerts, const cs237::vec3f *verts)
{
    GLuint id;

  // generate buffer IDs for the buffer
    CS237_CHECK( glGenBuffers (1, &id) );

    CS237_CHECK( glBindVertexArray (this->vaoId) );

  // vertex buffer initialization
    this->vbufId = id;
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, this->vbufId) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, nVerts*sizeof(cs237::vec3f), verts, GL_STREAM_DRAW));
    CS237_CHECK( glVertexAttribPointer (CoordAttrLoc, 3, GL_FLOAT, GL_FALSE, sizeof(cs237::vec3f), (GLvoid*)0) );
    CS237_CHECK( glEnableVertexAttribArray (CoordAttrLoc) );
  
  //Unbind the VBO & VAO
    CS237_CHECK( glBindVertexArray (0) );
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, 0) );

}

//! initialize the element array for the mesh
void Mesh::LoadIndices (int n, const uint32_t *indices)
{
    this->nIndices = n;
    CS237_CHECK( glBindVertexArray (this->vaoId) );
    CS237_CHECK( glGenBuffers (1, &this->ebufId) );
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->ebufId) );
    CS237_CHECK( glBufferData (GL_ELEMENT_ARRAY_BUFFER, n*sizeof(uint32_t), indices, GL_STATIC_DRAW) );
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0) );

    //Unbind VBO
    CS237_CHECK( glBindVertexArray (0) );
}
//! initialize the texture-coordinate data buffer for the mesh
void Mesh::LoadTexCoords (int nCoords, cs237::vec2f *tcoords){

    CS237_CHECK( glBindVertexArray (this->vaoId) );
    CS237_CHECK( glGenBuffers (1, &this->tcbufId) );
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, this->tcbufId) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, nCoords*sizeof(cs237::vec2f), tcoords, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (TexCoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (TexCoordAttrLoc) );
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindVertexArray (0) );
}
//! initialize the normals data buffer for the mesh
void Mesh::LoadNormals (int nVerts, cs237::vec3f *norms){

    GLuint id;
    
    // generate buffer IDs for the buffer
    CS237_CHECK( glGenBuffers (1, &id) );
    // normal buffer initialization 
    this->nbufId = id; 
    CS237_CHECK( glBindVertexArray (this->vaoId) );
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, this->nbufId) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, nVerts*sizeof(cs237::vec3f), norms, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (NormAttrLoc, 3, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (NormAttrLoc) );
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindVertexArray (0) );
}
void Mesh::Draw ()
{
  //Bind to the VAIO
    CS237_CHECK( glBindVertexArray (this->vaoId) );

  // bind the indices array
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->ebufId) );

  // render
    CS237_CHECK( glDrawElements (this->prim, this->nIndices, GL_UNSIGNED_INT, 0));
    
  // cleanup
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindVertexArray (0) );

}