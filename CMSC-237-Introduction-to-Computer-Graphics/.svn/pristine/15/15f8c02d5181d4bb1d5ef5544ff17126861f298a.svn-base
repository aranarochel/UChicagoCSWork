/*! \file mesh.cxx
 *
 * \author YOUR_NAME
 */

#include "mesh.hxx"

/*! The locations of the standard mesh attributes.  The layout directives in the shaders
 * should match these values.
 */
//const GLint	CoordAttrLoc = 0;	//!< location of vertex coordinates attribute

//! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
//! loaded separately.
Mesh::Mesh(GLenum p)
{
    this->prim = p;
    this->ebufId = 0;
    this->vbufId = 0;
    this->nIndices = 0;
    this->CoordAttrLoc = 0;

    CS237_CHECK(glGenVertexArrays(1,&(this->vaoId)));
}

//! initialize the vertex data buffers for the mesh
void Mesh::LoadVertices (int nVerts, const cs237::vec3f *verts)
{
 	GLuint id;

	  
	// create buffer
	CS237_CHECK(glGenBuffers(1,&(id)));
	// bind the vao
	CS237_CHECK( glBindVertexArray (this->vaoId) );

	this->vbufId = id;

	//Bind the vertices buffer to the context using the GL_ARRAY_BUFFER target 
	CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER,this->vbufId)); 
	  
	// Specify the amount of storage we want to use for the vertices buffer 
	CS237_CHECK(glBufferData(GL_ARRAY_BUFFER,nVerts*sizeof(cs237::vec3f),verts,GL_STREAM_DRAW));

	//specify the attribute
	CS237_CHECK(glVertexAttribPointer(CoordAttrLoc,3,GL_FLOAT,GL_FALSE,sizeof(cs237::vec3f),(GLvoid*)0));
	CS237_CHECK(glEnableVertexAttribArray(CoordAttrLoc)); 

	// unbind
	CS237_CHECK(glBindVertexArray(0));
	CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER,0)); 

}

//! initialize the element array for the mesh
void Mesh::LoadIndices (int n, const uint32_t *indices)
{

	this->nIndices = n;
    // bind vao, gen indices buffer
    CS237_CHECK( glBindVertexArray (this->vaoId) );
	CS237_CHECK( glGenBuffers (1, &this->ebufId) );
	CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->ebufId) );
	CS237_CHECK( glBufferData (GL_ELEMENT_ARRAY_BUFFER, n*sizeof(uint32_t), indices, GL_STATIC_DRAW) );

	//unbind
	CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0) );
}

void Mesh::Draw ()
{
	CS237_CHECK(glBindVertexArray(this->vaoId));

	// bind both vertices and index buffers
	CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER,this->vbufId)); 
	CS237_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->ebufId)); 

	// draw
    CS237_CHECK( glDrawElements (this->prim, this->nIndices, GL_UNSIGNED_INT, 0));

    // clean up
	CS237_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0)); 
    CS237_CHECK(glBindVertexArray(0));
}