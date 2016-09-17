/*! \file obj.hxx
 *
 * \author John Reppy
 *
 * This file specifies data structures to represent a subset of OBJ files.  The format
 * is limited to triangle meshes.
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _OBJ_HXX_
#define _OBJ_HXX_

#include "cs237.hxx"
#include <vector>

namespace OBJ {

//! Illumination modes define how to interpret the material values.
//! Note that other modes are mapped to Specular by the loader
enum {
    NoLight = 0,	//!< just use color component
    Diffuse = 1,	//!< ambient + diffuse
    Specular = 2	//!< include specular highlights
};

//! Structure that defines a material in a model.
struct Material {
    std::string		name;		//!< name of material
    int			illum;		//!< illumination mode (NoLight, etc.)
    cs237::color3f	ambient;	//!< ambient component
    cs237::color3f	diffuse;	//!< diffuse component
    cs237::color3f	specular;	//!< specular component
    float		shininess;	//!< specular exponent
    std::string		ambientMap;	//!< optional texture map for ambient
    std::string		diffuseMap;	//!< optional texture map for diffuse lighting
    std::string		specularMap;	//!< optional texture map for specular highlights
    std::string		normalMap;	//!< optional normal map for bump mapping
};

//! A mesh that corresponds to a group in the obj file
struct Group {
    std::string		name;		//!< name of this group */
    int			material;	//!< index to material for group (-1 for no material)
    uint32_t		nVerts;		//!< the number of vertices in this group.
    uint32_t		nIndices;	//!< the number of indices (3 * number of triangles)
    cs237::vec3f	*verts;		//!< array of nVerts texture coordinates (or nullptr)
    cs237::vec3f	*norms;		//!< array of nVerts texture coordinates (or nullptr)
    cs237::vec2f	*txtCoords;	//!< array of nVerts texture coordinates (or nullptr)
    uint32_t		*indices;	//!< array of nIndices element indices that can be used
					//!  to render the group
};

//! A model from an OBJ file
class Model {
  public:

  //! create a Model by loading it from the specified OBJ file
  //! \param filename the path of the OBJ file to be loaded
    Model (std::string filename);
    ~Model ();

  //! the model's axis-aligned bounding box
    const cs237::AABBf &BBox () const { return this->_bbox; }

  //! the number of materials associated with this model
    int NumMaterials () const { return this->_materials.size(); }
  //! get a material
    const OBJ::Material & Material (int i) const { return this->_materials[i]; }

  //! the number of groups in this model
    int NumGroups () const { return this->_groups.size(); }
  //! get a group by index
    const OBJ::Group & Group (int i) const { return this->_groups[i]; }
  //! iterator for looping over the groups in the module
    std::vector<OBJ::Group>::const_iterator beginGroups () const { return this->_groups.begin(); }
  //! terminator for looping over the groups in the module
    std::vector<OBJ::Group>::const_iterator endGroups () const { return this->_groups.end(); }

  private:
    std::string		_path;		//!< path to the obj file that this model came from
    std::string		_mtlLibName;	//!< name of the material library for this model
    cs237::AABBf	_bbox;		//!< bounding box for model

    std::vector<OBJ::Material> _materials;
    std::vector<OBJ::Group> _groups;

  // read a material library
    bool readMaterial (std::string m);

};

}; // namespace OBJ

#endif // !_OBJ_HXX_
