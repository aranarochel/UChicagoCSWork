/*! \file mesh.cxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "mesh.hxx"

/* the buffer indices */
enum {
    V_ID = 0,
    VN_ID,
    VT_ID,
    TAN_ID,
    IDX_ID,
    NUM_BUFFERS
};

//  Helper function to enable attribute locations
//
static inline void InitAttributeLoc (bool enable, GLuint loc)
{
    if (enable) {
	CS237_CHECK( glEnableVertexAttribArray (loc) );
    }
    else {
	CS237_CHECK( glDisableVertexAttribArray (loc) );
    }

}

//! create a MeshInfo object to be used for some other kind of mesh (not a Group)
//! \param p the primitive use to render the mesh
MeshInfo::MeshInfo (GLenum p)
    : _vaoId(0), _prim(p), _nIndices(0)
{
  // generate the vertex array object for the mesh
    CS237_CHECK( glGenVertexArrays (1, &(this->_vaoId)) );
}

//! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
//! loaded separately.
MeshInfo::MeshInfo (GLenum p, OBJ::Group const &grp,
	cs237::color3f diffuseC,
	cs237::color3f specularC,
	float shininess,
	cs237::texture2D *cmap,
	cs237::texture2D *nmap)
    : _vaoId(0), _prim(p), _nIndices(grp.nIndices),
	_diffuseC(diffuseC), _specularC(specularC), _shininess(shininess),
	_cMap(cmap), _nMap(nmap)
{
  // generate the vertex array object for the mesh
    CS237_CHECK( glGenVertexArrays (1, &(this->_vaoId)) );
    CS237_CHECK( glBindVertexArray (this->_vaoId) );

  // generate buffer IDs for the buffer
    GLuint id[NUM_BUFFERS];
    CS237_CHECK( glGenBuffers (NUM_BUFFERS, id) );

  // vertex buffer initialization
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, id[V_ID]) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, grp.nVerts*sizeof(cs237::vec3f), grp.verts, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (CoordAttrLoc, 3, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (CoordAttrLoc) );

  // normal buffer initialization
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, id[VN_ID]) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, grp.nVerts*sizeof(cs237::vec3f), grp.norms, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (NormAttrLoc, 3, GL_FLOAT, GL_FALSE, 0, 0) );

  // texture-coordinate buffer initialization
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, id[VT_ID]) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, grp.nVerts*sizeof(cs237::vec2f), grp.txtCoords, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (TexCoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, 0) );

  // tangent-vector buffer initialization
    cs237::vec3f *tan = new cs237::vec3f[grp.nVerts];
    cs237::vec3f *bitan = new cs237::vec3f[grp.nVerts];
    for (int i = 0;  i < grp.nVerts;  i++) {  // clear arrays
	tan[i] = cs237::vec3f();
	bitan[i] = cs237::vec3f();
    }
    cs237::vec4f *tangents = new cs237::vec4f[grp.nVerts];
    uint32_t nTris = grp.nIndices / 3;
    assert (nTris * 3 == grp.nIndices);
    for (int tri = 0;  tri < nTris;  tri++) {
      // get the indices for the triangle
	uint32_t i1 = grp.indices[3*tri + 0];
	uint32_t i2 = grp.indices[3*tri + 1];
	uint32_t i3 = grp.indices[3*tri + 2];
      // get the vertices for the triangle
	cs237::vec3f v1 = grp.verts[i1];
	cs237::vec3f v2 = grp.verts[i2];
	cs237::vec3f v3 = grp.verts[i3];
      // get the texture coordinates for the triangle
	cs237::vec2f vt1 = grp.txtCoords[i1];
	cs237::vec2f vt2 = grp.txtCoords[i2];
	cs237::vec2f vt3 = grp.txtCoords[i3];
      // the sides of the triangle as a 3x2 matrix
	cs237::mat3x2f Q = cs237::mat3x2f(
	    v2.x - v1.x, v3.x - v1.x,	// column one
	    v2.y - v1.y, v3.y - v2.y,	// column two
	    v2.z - v1.z, v3.z - v1.z);	// column three
      // the sides in tangent space as a 2x2 matrix
	cs237::mat2x2f ST = cs237::mat2x2f (
	    vt2.x - vt1.x, vt3.x - vt1.x,	// first column
	    vt2.y - vt1.y, vt3.y - vt1.y);	// second column
      // Q = ST * [T B]^T, so multiply Q by ST^{-1}
	cs237::mat3x2f TB = ST.inverse() * Q;
      // extract rows T and B
	cs237::vec3f t = cs237::vec3f(TB[0][0], TB[1][0], TB[2][0]);
	cs237::vec3f b = cs237::vec3f(TB[0][1], TB[1][1], TB[2][1]);
      // add to vector sums
	tan[i1] += t;
	tan[i2] += t;
	tan[i3] += t;
	bitan[i1] += b;
	bitan[i2] += b;
	bitan[i3] += b;
    }
  // compute extended tangents for vertices
    for (int i = 0;  i < grp.nVerts;  i++) {
	cs237::vec3f n = grp.norms[i];
	cs237::vec3f t = tan[i];
      // orthogonalize
	t = normalize(t - n * dot(n, t));
	float w = (dot(cross(n, t), bitan[i]) < 0.0f ? -1.0f : 1.0f);
	tangents[i] = cs237::vec4f(t, w);
    }
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, id[TAN_ID]) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, grp.nVerts*sizeof(cs237::vec4f), tangents, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (TanAttrLoc, 4, GL_FLOAT, GL_FALSE, 0, 0) );
    delete[] tan;
    delete[] bitan;
    delete[] tangents;

  // index-array buffer initialization
    this->_ebufId = id[IDX_ID];
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->_ebufId) );
    CS237_CHECK( glBufferData (GL_ELEMENT_ARRAY_BUFFER, grp.nIndices*sizeof(uint32_t), grp.indices, GL_STATIC_DRAW) );

  // cleanup
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindVertexArray (0) );

}

MeshInfo::~MeshInfo ()
{
    /* TODO */
}

void MeshInfo::Draw (bool enableNorms, bool enableTexs, bool enableTans)
{
    CS237_CHECK( glBindVertexArray (this->_vaoId) );

  // bind the indices array
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->_ebufId) );

  // enable additional attribute buffers
    InitAttributeLoc (enableNorms, NormAttrLoc);
    InitAttributeLoc (enableTexs, TexCoordAttrLoc);
    InitAttributeLoc (enableTans, TanAttrLoc);

  // render
    CS237_CHECK( glDrawElements (this->_prim, this->_nIndices, GL_UNSIGNED_INT, 0) );

  // cleanup
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindVertexArray (0) );

}
