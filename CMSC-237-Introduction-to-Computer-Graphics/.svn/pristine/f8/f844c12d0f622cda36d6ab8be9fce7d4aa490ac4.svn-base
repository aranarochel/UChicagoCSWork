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

    GLuint ebufId;
    int nIndices;
    GLenum prim;       //! primitive

    GLint   CoordAttrLoc;
    cs237::vec3f posn;
    cs237::color4f color;


    /** HINT: I have provide these functions that helping load information about the mesh. You 
     * free to change anything to your liking */ 
    //! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
    //! loaded separately.
    Mesh (GLenum p);

  //! initialize the vertex data buffers for the mesh
    void LoadVertices (int nVerts, const cs237::vec3f *verts);

  //! initialize the element array for the mesh
    void LoadIndices (int n, const uint32_t *indices);

  //! draw the mesh using a glDrawElements call
    void Draw ();

};

#endif // !_MESH_HXX_
