/*! \file mesh.hxx
 *
 * \author YOUR_NAME
 */


#ifndef _MESH_HXX_
#define _MESH_HXX_

#include "cs237.hxx"

//! the information needed to render a mesh
struct Mesh {

    /**HINT: Create member variables for your VBOs and VAO ids */ 
    /**HINT: Create member variable for the primitive */
    /**HINT: You could also provide member variables for the position and color inside this mesh */ 
    /** YOUR CODE HERE **/
    GLuint vaoId;      //!< vertex-array-object ID for the simple triangle 
    GLuint vbufId;     //! vertex-buffer ID for the vertex data

    GLuint tbufId;      // id's for the tex coords and the normal coords
    GLuint nbufId;

    GLuint ebufId;      // id for the indices
    int nIndices;
    GLenum prim;       //! primitive

    cs237::vec3f posn;
    cs237::color4f color;

    cs237::vec3f direc;
    cs237::color3f intensity;
    cs237::color3f ambient;

    cs237::image2d *image;
    cs237::texture2D *texture;

    /** HINT: I have provide these functions that helping load information about the mesh. You 
     * free to change anything to your liking */ 
    //! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
    //! loaded separately.
    Mesh (GLenum p);

  //! initialize the vertex data buffers for the mesh
    void LoadVertices (int nVerts, const cs237::vec3f *verts);

  //! initialize the element array for the mesh
    void LoadIndices (int n, const uint32_t *indices);

  //! initialize the texture-coordinate data buffer for the mesh
    void LoadTexCoords (int nCoords, cs237::vec2f *tcoords);

  //! initialize the normals data buffer for the mesh
    void LoadNormals (int nVerts, cs237::vec3f *norms);

  //! draw the mesh using a glDrawElements call
    void Draw ();

};

#endif // !_MESH_HXX_
