/*! \file mesh.hxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _MESH_HXX_
#define _MESH_HXX_

#include "cs237.hxx"
#include "obj.hxx"

/*! The locations of the standard mesh attributes.  The layout directives in the shaders
 * should match these values.
 */
const GLint	CoordAttrLoc = 0;	//!< location of vertex coordinates attribute
const GLint	NormAttrLoc = 1;	//!< location of normal-vector attribute
const GLint	TexCoordAttrLoc = 2;	//!< location of texture coordinates attribute
const GLint	TanAttrLoc = 3;		//!< location of extended tangent vector


//! the information needed to render a mesh
class MeshInfo {
  public:

  //! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
  //! loaded separately.
  //! \param p the primitive use to render the mesh
  //! \param grp the OBJ Group used to initialize the vertex buffers
    MeshInfo (GLenum p, OBJ::Group const &grp,
	cs237::color3f diffuseC,
	cs237::color3f specularC,
	float shininess,
	cs237::texture2D *cmap,
	cs237::texture2D *nmap);

  //! destructor
    virtual ~MeshInfo ();

  //! does this mesh have a diffuse-color map?
    bool hasDiffuseMap () const { return (this->_cMap != nullptr); }

  //! return this mesh's diffuse-color map (or nullptr)
    const cs237::texture2D *DiffuseMap() const	{ return this->_cMap; }

  //! return the diffuse color for this mesh to use in lieu of the diffuse-color map
    cs237::color3f DiffuseColor () const { return this->_diffuseC; }

  //! bind the mesh's diffuse-color map to the specified texture unit
    void BindDiffuseMap (GLuint txtUnit)
    {
	assert (this->_cMap != nullptr);
	CS237_CHECK( glActiveTexture(txtUnit) );
	this->_cMap->Bind();
    }

  //! return the specular color for this mesh
    cs237::color3f SpecularColor () const { return this->_specularC; }

  //! return the shininess of the surface (aka the Phong exponent)
    float Shininess () const { return this->_shininess; }

  //! does this mesh have a normal map?
    bool hasNormalMap () const { return (this->_nMap != nullptr); }

  //! return this mesh's normal map (or nullptr)
    const cs237::texture2D *NormalMap() const	{ return this->_nMap; }

  //! bind the mesh's normal map to the specified texture unit
    void BindNormalMap (GLuint txtUnit)
    {
	assert (this->_nMap != nullptr);
	CS237_CHECK( glActiveTexture(txtUnit) );
	this->_nMap->Bind();
    }

  //! draw the mesh using a glDrawElements call
  //! \param enableNorms   when true, we enable the normal-vector attribute buffer
  //! \param enableTxts    when true, we enable the texture-coordinate attribute buffer
  //! \param enableTxts    when true, we wnable the tangent=vector attribute buffer
    void Draw (bool enableNorms, bool enableTxts, bool enableTans);

  protected:

  //! a constructor for subclasses of meshes.
  //! \param p the primitive use to render the mesh
    MeshInfo (GLenum p);

    GLuint		_vaoId;		//!< vertex-array-object ID for this mesh
    GLuint		_ebufId;	//!< buffer ID for the index array
    GLenum		_prim;		//!< the primitive type for rendering the mesh
					//!  (e.g., GL_TRIANGLES, GL_TRIANGLE_FAN, etc.)
    int			_nIndices;	//!< the number of vertex indices
  // material properties
    cs237::color3f	_diffuseC;	//!< the diffuse color
    cs237::color3f	_specularC;	//!< the specular color
    float		_shininess;	//!< the shininess of the surface
    cs237::texture2D	*_cMap;		//!< the color-map texture for the mesh
    cs237::texture2D	*_nMap;		//!< the normal-map texture for the mesh

};

#endif // !_MESH_HXX_
