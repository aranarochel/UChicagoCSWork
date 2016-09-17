/*! \file mesh.hxx
 *
 * \author John Reppy
 * \author Lamont Samuels
 */

/* CMSC23700 Lab 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr) & Lamont Samuels
 * All rights reserved.
 */

#ifndef _MESH_HXX_
#define _MESH_HXX_

#include "cs237.hxx"

//! the information needed to render a mesh
struct Mesh {

    GLuint		vaoId;		//!< vertex-array-object ID for this mesh
    GLuint		vbufId;		//!< vertex-buffer ID for the vertex data
    GLuint		ebufId;		//!< buffer ID for the index array
    GLuint    nbufId;   //!< buffer ID for the normal-vector data
    GLuint    tcbufId;  //!< buffer ID for the texture coordinate data
    GLenum		prim;		//!< the primitive type for rendering the mesh
					             //!  (e.g., GL_TRIANGLES, GL_TRIANGLE_FAN, etc.)
    int			nIndices;	//!< the number of vertex indices

  //! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
  //! loaded separately.
   Mesh (GLenum p);

  //! initialize the texture-coordinate data buffer for the mesh
    void LoadTexCoords (int nCoords, cs237::vec2f *tcoords);

  //! initialize the normals data buffer for the mesh
    void LoadNormals (int nVerts, cs237::vec3f *norms);

  //! initialize the vertex data buffers for the mesh
    void LoadVertices (int nVerts, const cs237::vec3f *verts);

  //! initialize the element array for the mesh
    void LoadIndices (int n, const uint32_t *indices);

  //! draw the mesh using a glDrawElements call
    void Draw ();

};

#endif // !_MESH_HXX_
