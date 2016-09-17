/*! \file ground.cxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "ground.hxx"

/* the buffer indices */
enum {
    V_ID = 0,
    VN_ID,
    VT_ID,
    TAN_ID,
    IDX_ID,
    NUM_BUFFERS
};

// helper function that computes the normal for a triangle; vertices should be in CCW order
//
static cs237::vec3f TriNormal (cs237::vec3f v0, cs237::vec3f v1, cs237::vec3f v2)
{
    cs237::vec3f n = cross(v1 - v0, v2 - v0);
    return n.normalize();
}

Ground::Ground (const HeightField *hf)
    : MeshInfo(GL_TRIANGLES), _bbox(hf->BBox())
{
    CS237_CHECK( glBindVertexArray (this->_vaoId) );

  // generate buffer IDs for the mesh buffers
    GLuint id[NUM_BUFFERS];
    CS237_CHECK( glGenBuffers (NUM_BUFFERS, id) );

    uint32_t nr = hf->NumRows();
    uint32_t nc = hf->NumCols();
    uint32_t nverts = hf->NumVerts();
    assert ((nr >= 2) && (nc >= 2));

std::cerr << "Ground: " << nc << " x " << nr << " (" << nverts << ") samples" << std::endl;
  // initialize the vertices
    cs237::vec3f *verts = new cs237::vec3f[nverts];
    for (int r = 0;  r < nr;  r++) {
	for (int c = 0;  c < nc;  c++) {
	    verts[hf->IndexOf(r, c)] = hf->VertexAt(r, c);
	}
    }
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, id[V_ID]) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, nverts*sizeof(cs237::vec3f), verts, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (CoordAttrLoc, 3, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (CoordAttrLoc) );

  // compute the normals and initialize the normals buffer
    cs237::vec3f *norms = new cs237::vec3f[nverts];
  // first compute the normals for the corners
    norms[hf->IndexOf(0,    0)]    =
	TriNormal(hf->VertexAt(0,0), hf->VertexAt(1,0), hf->VertexAt(0,1));
    norms[hf->IndexOf(0,    nc-1)] =
	TriNormal(hf->VertexAt(0,nc-1), hf->VertexAt(0,nc-2), hf->VertexAt(1,nc-1));
    norms[hf->IndexOf(nr-1, 0)]    =
	TriNormal(hf->VertexAt(nr-1,0), hf->VertexAt(nr-1,1), hf->VertexAt(nr-2,1));
    norms[hf->IndexOf(nr-1, nc-1)] =
	TriNormal(hf->VertexAt(nr-1,nc-1), hf->VertexAt(nr-2,nc-1), hf->VertexAt(nr-1,nc-2));
  // next compute the normals for the edges
    for (int r = 1;  r < nr-1;  r++) {
	norms[hf->IndexOf(r, 0)] = 0.5f * (
	    TriNormal(hf->VertexAt(r, 0), hf->VertexAt(r, 1), hf->VertexAt(r-1, 0)) +
	    TriNormal(hf->VertexAt(r, 0), hf->VertexAt(r+1, 0), hf->VertexAt(r, 1)));
	norms[hf->IndexOf(r, nc-1)] = 0.5f * (
	    TriNormal(hf->VertexAt(r, nc-1), hf->VertexAt(r-1, nc-1), hf->VertexAt(r, nc-2)) +
	    TriNormal(hf->VertexAt(r, nc-1), hf->VertexAt(r, nc-2), hf->VertexAt(r+1, nc-1)));
    }
    for (int c = 1;  c < nc-1;  c++) {
	norms[hf->IndexOf(0,    c)] = 0.5f * (
	    TriNormal(hf->VertexAt(0, c), hf->VertexAt(0, c-1), hf->VertexAt(1, c)) +
	    TriNormal(hf->VertexAt(0, c), hf->VertexAt(1, c), hf->VertexAt(0, c+1)));
	norms[hf->IndexOf(nr-1, c)] = 0.5f * (
	    TriNormal(hf->VertexAt(nr-1, c), hf->VertexAt(nr-2, c), hf->VertexAt(nr-1, c-1)) +
	    TriNormal(hf->VertexAt(nr-1, c), hf->VertexAt(nr-1, c+1), hf->VertexAt(nr-2, c)));
    }
  // finally compute the normals for the interior vertices
    for (int r = 1;  r < nr-1;  r++) {
	for (int c = 1;  c < nc-1;  c++) {
	    norms[hf->IndexOf(r, c)] = 0.25f * (
		TriNormal(hf->VertexAt(r,c), hf->VertexAt(r-1,c), hf->VertexAt(r,c-1)) +
		TriNormal(hf->VertexAt(r,c), hf->VertexAt(r,c+1), hf->VertexAt(r-1,c)) +
		TriNormal(hf->VertexAt(r,c), hf->VertexAt(r+1,c), hf->VertexAt(r,c+1)) +
		TriNormal(hf->VertexAt(r,c), hf->VertexAt(r,c-1), hf->VertexAt(r+1,c)));
	}
    }
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, id[VN_ID]) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, nverts*sizeof(cs237::vec3f), norms, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (NormAttrLoc, 3, GL_FLOAT, GL_FALSE, 0, 0) );

  // compute the extended tangent vectors for normal-mapping mode
    cs237::vec4f *tangents = new cs237::vec4f[nverts];
    for (int r = 0;  r < nr;  r++) {
	for (int c = 0;  c < nc;  c++) {
	    int idx = hf->IndexOf(r, c);
	    cs237::vec3f T(1.0f, 0.0f, 0.0f);
	    cs237::vec3f B(0.0f, 0.0f, 1.0f);
	    cs237::vec3f N = norms[idx];
	  // orthogonalize using Gram-Schmidt
	    T = normalize (T - dot(T, N) * N);
	    B = normalize (B - dot(B, N) * N - dot(B, T) * T);
	    float Tw = dot(B, cross(N, T));
	    tangents[idx] = cs237::vec4f(T[0], T[1], T[2], (Tw < 0.0f) ? -1 : 1);
	}
    }
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, id[TAN_ID]) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, nverts*sizeof(cs237::vec4f), tangents, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (TanAttrLoc, 4, GL_FLOAT, GL_FALSE, 0, 0) );
    delete[] norms;
    delete[] tangents;

  // initialize the texture coordinates; we want the following mapping for the
  // corners:
  //	(row,  col)		(x, y)
  //   --------------------------------
  //	(0,    0)		(0, 1)
  //	(0,    nc-1)		(1, 1)
  //	(nr-1, 0)		(0, 0)
  //	(nr-1, nc-1)		(1, 0)
    cs237::vec2f *txtCoords = new cs237::vec2f[nverts];
    for (int r = 0;  r < nr;  r++) {
	float y = float(nr - r - 1) / float(nr-1);
	for (int c = 0;  c < nc;  c++) {
	    float x = float(c) / float(nc-1);
	    txtCoords[hf->IndexOf(r,c)] = cs237::vec2f(x, y);
	}
    }
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, id[VT_ID]) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, nverts*sizeof(cs237::vec2f), txtCoords, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (TexCoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, 0) );
    delete[] txtCoords;

  // initialize the element buffer
    uint32_t nIndices = 3 * 2 * (nr - 1) * (nc - 1); // the number of indices = 3 * triangles
    uint32_t *indices = new uint32_t[nIndices];
    int idx = 0;
    for (int r = 1;  r < nr;  r++) {
	for (int c = 1;  c < nc;  c++) {
	    if ((c & 1) == (r & 1)) {
	      // triangulate from upper left to lower right
		indices[idx + 0] = hf->IndexOf(r,   c-1);
		indices[idx + 1] = hf->IndexOf(r,   c  );
		indices[idx + 2] = hf->IndexOf(r-1, c-1);
		indices[idx + 3] = hf->IndexOf(r-1, c  );
		indices[idx + 4] = hf->IndexOf(r-1, c-1);
		indices[idx + 5] = hf->IndexOf(r,   c  );
	    }
	    else {
	      // triangulate from lower left to upper right
		indices[idx + 0] = hf->IndexOf(r,   c  );
		indices[idx + 1] = hf->IndexOf(r-1, c  );
		indices[idx + 2] = hf->IndexOf(r,   c-1);
		indices[idx + 3] = hf->IndexOf(r-1, c-1);
		indices[idx + 4] = hf->IndexOf(r,   c-1);
		indices[idx + 5] = hf->IndexOf(r-1, c  );
	    }
	    idx += 6;
	}
    }
    assert (idx == nIndices);
    this->_nIndices = nIndices;
    this->_ebufId = id[IDX_ID];
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->_ebufId) );
    CS237_CHECK( glBufferData (GL_ELEMENT_ARRAY_BUFFER, nIndices*sizeof(uint32_t), indices, GL_STATIC_DRAW) );
    delete[] indices;

  // initialize the ground textures
    if (hf->DiffuseMap() != nullptr) {
	this->_cMap = new cs237::texture2D(GL_TEXTURE_2D, hf->DiffuseMap());
	this->_cMap->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	this->_cMap->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    if (hf->NormalMap() != nullptr) {
	this->_nMap = new cs237::texture2D(GL_TEXTURE_2D, hf->NormalMap());
	this->_nMap->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	this->_nMap->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }


  // cleanup
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindVertexArray (0) );

  // initialize the other MeshInfo fields
    this->_diffuseC = hf->DiffuseC();
    this->_specularC = cs237::color3f(0.1f, 0.1f, 0.1f);
    this->_shininess = 10.0f;

  // wrap this as an Instance
    this->_instance.meshes.push_back(this);
    this->_instance.toWorld = cs237::mat4f(1.0f);  	// identity, since ground is in world coords
    this->_instance.normToWorld = cs237::mat3f(1.0f);	// ditto
    this->_instance.normFromWorld = cs237::mat3f(1.0f);	// ditto
    this->_instance.bbox = this->_bbox;
    this->_instance.color = hf->DiffuseC();

}

Ground::~Ground ()
{
  /* TODO */
}
