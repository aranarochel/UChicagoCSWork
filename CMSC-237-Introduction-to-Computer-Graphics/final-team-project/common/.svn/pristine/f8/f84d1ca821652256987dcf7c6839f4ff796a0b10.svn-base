/*
      This code was ported from a C program written by Nate Robins.
 */

#ifndef _OBJ_READER_HXX_
#define _OBJ_READER_HXX_

/* OBJtriangle: Structure that defines a triangle in a model.
 */
struct OBJtriangle {
  uint32_t	vindices[3];	/* array of triangle vertex indices */
  uint32_t	nindices[3];	/* array of triangle normal indices */
  uint32_t	tindices[3];	/* array of triangle texcoord indices*/
};

/* OBJgroup: Structure that defines a group in a model.
 */
struct OBJgroup {
  char*		name;		/* name of this group */
  uint32_t	numtriangles;	/* number of triangles in this group */
  uint32_t	*triangles;	/* array of triangle indices */
  char*		material;	/* name of material for group */
  OBJgroup	*next;		/* pointer to next group in model */
};

/* OBJmodel: Structure that defines a model.  Note that the vertex/normal/texcoord
 * arrays have 1-based indices, so the 0th element is unused.
 */
struct OBJmodel {
  char*		mtllibname;	/* name of the material library */

  uint32_t	numvertices;	/* number of vertices in model */
  cs237::vec3f	*vertices;	/* array of vertices  */

  uint32_t	numnormals;	/* number of normals in model */
  cs237::vec3f	*normals;	/* array of normals */

  uint32_t	numtexcoords;	/* number of texcoords in model */
  cs237::vec2f	*texcoords;	/* array of texture coordinates */

  uint32_t	numtriangles;	/* number of triangles in model */
  OBJtriangle*	triangles;	/* array of triangles */

  uint32_t	numgroups;	/* number of groups in model */
  OBJgroup*	groups;		/* linked list of groups */

  OBJmodel ();
  ~OBJmodel ();

};

/* OBJReadOBJ: Reads a model description from a Wavefront .OBJ file.
 * Returns a pointer to the created object which should be free'd with
 * OBJDelete().
 *
 * filename - name of the file containing the Wavefront .OBJ format data.  
 */
OBJmodel *OBJReadOBJ (const char* filename);

#endif /*! _OBJ_READER_HXX_ */
