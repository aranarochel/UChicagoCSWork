/*! \file cone-mesh.cxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "instance.hxx"

#define NUM_SLICES	12
#define NUM_VERTS	(NUM_SLICES+2)	// base points + center + apex
#define NUM_TRIS	(2*NUM_SLICES)	// base + sides

// vertices for a canonical cone with the apex located at the origin
static cs237::vec3f coneVerts[NUM_VERTS] = {
      // center of base
	cs237::vec3f( 0.0000f,  0.0000f,  1.0000f),	
      // 12 vertices around the base
	cs237::vec3f( 1.0000f,  0.0000f,  1.0000f),
	cs237::vec3f( 0.8660f,  0.5000f,  1.0000f),
	cs237::vec3f( 0.5000f,  0.8660f,  1.0000f),
	cs237::vec3f( 0.0000f,  1.0000f,  1.0000f),
	cs237::vec3f(-0.5000f,  0.8660f,  1.0000f),
	cs237::vec3f(-0.8660f,  0.5000f,  1.0000f),
	cs237::vec3f(-1.0000f,  0.0000f,  1.0000f),
	cs237::vec3f(-0.8660f, -0.5000f,  1.0000f),
	cs237::vec3f(-0.5000f, -0.8660f,  1.0000f),
	cs237::vec3f(-0.0000f, -1.0000f,  1.0000f),
	cs237::vec3f( 0.5000f, -0.8660f,  1.0000f),
	cs237::vec3f( 0.8660f, -0.5000f,  1.0000f),
      // apex
	cs237::vec3f( 0.0000f,  0.0000f,  0.0000f)
    };
// indices for rendering the canonical cone (CCW)
static uint32_t coneIndices[3 * NUM_TRIS] = {
      // base triangles
	 0,  1,  2,
	 0,  2,  3,
	 0,  3,  4,
	 0,  4,  5,
	 0,  5,  6,
	 0,  6,  7,
	 0,  7,  8,
	 0,  8,  9,
	 0,  9, 10,
	 0, 10, 11,
	 0, 11, 12,
	 0, 12,  1,
      // side triangles
	13,  2,  1,
	13,  3,  2,
	13,  4,  3,
	13,  5,  4,
	13,  6,  5,
	13,  7,  6,
	13,  8,  7,
	13,  9,  8,
	13, 10,  9,
	13, 11, 10,
	13, 12, 11,
	13,  1, 12
    };

class Cone : public MeshInfo {
  public:
    Cone (cs237::vec3f pos, cs237::vec3f dir, float angle, float height, cs237::AABBf &bbox);
    ~Cone ();
};

Cone::Cone (cs237::vec3f pos, cs237::vec3f dir, float angle, float height, cs237::AABBf &bbox)
    : MeshInfo(GL_TRIANGLES)
{
  // the radius of the cone's base; we want this value to be the height of
  // a base triangle so that the true circular base of the cone is inscribed
  // in the base polygon.
    float base = height * tanf(cs237::radians(angle));
    base /= cosf((2.0f * M_PI) / float(NUM_SLICES));

  // scale the vertices
    cs237::vec3f verts[14];
    cs237::vec3f scale(base, base, height);
    for (int i = 0;  i < NUM_VERTS;  i++) {
	verts[i] = scale * coneVerts[i];
    }

  // construct a rotation matrix to orient the cone
    dir.normalize();
    cs237::vec3f xAxis, yAxis;
    float magX = fabsf(dir.x), magY = fabsf(dir.y), magZ = fabsf(dir.z);
    if ((magX <= magY) && (magX <= magZ)) {
	xAxis = normalize(cross(cs237::vec3f(1.0f, 0.0f, 0.0f), dir));
	yAxis = normalize(cross(dir, xAxis));
    }
    else if ((magY <= magX) && (magY <= magZ)) {
	xAxis = normalize(cross(cs237::vec3f(0.0f, 1.0f, 0.0f), dir));
	yAxis = normalize(cross(dir, xAxis));
    }
    else {
	assert ((magZ <= magX) && (magZ <= magY));
	xAxis = normalize(cross(cs237::vec3f(0.0f, 0.0f, 1.0f), dir));
	yAxis = normalize(cross(dir, xAxis));
    }
    cs237::mat3f R(xAxis, yAxis, dir);

  // rotate the cone to point in the specified direction and then translate it.
    bbox.clear();
    for (int i = 0;  i < NUM_VERTS;  i++) {
	verts[i] = R * verts[i] + pos;
	bbox += verts[i];
    }

  // generate the vertex array object for the mesh
    CS237_CHECK( glGenVertexArrays (1, &(this->_vaoId)) );
    CS237_CHECK( glBindVertexArray (this->_vaoId) );

  // generate buffer IDs for the buffer
    GLuint id[2];
    CS237_CHECK( glGenBuffers (2, id) );

  // vertex buffer initialization
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, id[0]) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, NUM_VERTS*sizeof(cs237::vec3f), verts, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (CoordAttrLoc, 3, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (CoordAttrLoc) );

  // element buffer initialization
    this->_ebufId = id[1];
    this->_nIndices = 3 * NUM_TRIS;
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->_ebufId) );
    CS237_CHECK( glBufferData (GL_ELEMENT_ARRAY_BUFFER, 3*NUM_TRIS*sizeof(uint32_t), coneIndices, GL_STATIC_DRAW) );

  // cleanup
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindVertexArray (0) );

}

Cone::~Cone () { }

Instance *MakeCone (cs237::vec3f pos, cs237::vec3f dir, float angle, float height)
{
    assert ((0.0f < angle) && (angle < 90.0f));
    assert (0.0f < height);

    Instance *inst = new Instance;

    inst->meshes.push_back (new Cone(pos, dir, angle, height, inst->bbox));
  // the cone is already transformed to world coordinates, soe the transforms are all I
    inst->toWorld = cs237::mat4f(1.0f);	
    inst->normToWorld = cs237::mat3f(1.0f);	
    inst->normFromWorld = cs237::mat3f(1.0f);
    inst->color = cs237::color3f(1.0f, 1.0f, 0.0f);

    return inst;

}
