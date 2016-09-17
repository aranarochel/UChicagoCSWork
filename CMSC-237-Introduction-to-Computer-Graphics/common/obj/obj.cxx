/*! \file obj.cxx
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "obj.hxx"
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include "obj-reader.hxx"

namespace OBJ {

  // return the directory part of a pathname
    static std::string dirName (std::string const &path)
    {
	return path.substr(0, path.find_last_of('/'));
    }

  // scan one or more floats from a string
    bool scanFloats (std::string s, int n, float f[3])
    {
	assert ((0 < n) && (n <= 3));
	try {
	    for (int i = 0;  i < n;  i++) {
		size_t sz;
		f[i] = std::stof (s, &sz);
		s = s.substr(sz);
	    }
	}
	catch (std::invalid_argument) {
	    return false;
	}

	return true;
    }

    bool scanInt (std::string s, int &n)
    {
	try {
	    n = std::stoi (s);
	}
	catch (std::invalid_argument) {
	    return false;
	}

	return true;
    }

    bool Model::readMaterial (std::string m)
    {
	std::string file = dirName(this->_path) + "/" + m;
	std::ifstream inS(file);
	if (inS.fail()) {
	    std::cerr << "Error: unable to open \"" << file << "\"" << std::endl;
	    return false;
	}

      // parse the materials
	int count = 0;
	struct Material mtl;
	float fvals[3];
	int ival;
	std::string ln;
	while (std::getline(inS, ln).good()) {
	  // Note that ln will _not_ include the end-of-line terminator!
	    if ((ln.length() == 0) || (ln[0] == '#')) {
		continue;
	    }
	  // skip initial whitespace and ignore empty lines
	    size_t pos = ln.find_first_not_of(" \t");
	    if (pos == std::string::npos) {
		continue;
	    }
	    size_t pos2 = ln.find_first_of(" \t", pos);
	    std::string firstTok = ln.substr(pos, pos2-pos);
	    std::string rest = ln.substr(ln.find_first_not_of(" \t", pos2));
	    if (firstTok.compare("newmtl") == 0) { // newmtl name
		if (count > 0) {
		    this->_materials.push_back(mtl);
		}
		++count;
	      // initialize mtl to default values
		mtl.name = (rest.empty() ? "default" : rest);
		mtl.illum = NoLight;
		mtl.ambient = cs237::color3f(0.2f, 0.2f, 0.2f);
		mtl.diffuse = cs237::color3f(0.8f, 0.8f, 0.8f);
		mtl.specular = cs237::color3f(0.0f, 0.0f, 0.0f);
		mtl.shininess = 0.0f;
		mtl.ambientMap.clear();
		mtl.diffuseMap.clear();
		mtl.specularMap.clear();
		mtl.normalMap.clear();
	    }
	    else if (firstTok.compare("Ns") == 0) {  // Ns <float>
		if (scanFloats(rest, 1, fvals)) {
#ifdef WAVEFRONT_SHINY
		  /* wavefront shininess is from [0, 1000], so scale for OpenGL */
		    mtl.shininess = 128.0f * (fvals[0] / 1000.0f);
#else
		    mtl.shininess = fvals[0];
#endif
		}
		else {
		    std::cerr << "Error: excepted \"Ns <float>\"" << std::endl;
		}
	    }
#ifdef MORE_OBJ_FEATURES
	    else if (firstTok.compare("Ni") == 0) {  // Ni <float>
		/* ignore refraction index */
	    }
	    else if (firstTok.compare("Tf") == 0) {  // Tf ...
		/* ignore transmission factor */
	    }
	    else if (firstTok.compare("d") == 0) {  // d <float> or d -halo <float>
		/* ignore dissolve */
	    }
#endif
	    else if (firstTok.compare("Kd") == 0) {  // Kd <float> <float> <float>
		if (scanFloats(rest, 3, fvals)) {
		    mtl.diffuse = cs237::color3f(fvals[0], fvals[1], fvals[2]);
		}
		else {
		    std::cerr << "Error: excepted \"Kd <float> <float> <float>\"" << std::endl;
		}
	    }
	    else if (firstTok.compare("Ka") == 0) {  // Ka <float> <float> <float>
		if (scanFloats(rest, 3, fvals)) {
		    mtl.ambient = cs237::color3f(fvals[0], fvals[1], fvals[2]);
		}
		else {
		    std::cerr << "Error: excepted \"Ka <float> <float> <float>\"" << std::endl;
		}
	    }
	    else if (firstTok.compare("Ks") == 0) {  // Ks <float> <float> <float>
		if (scanFloats(rest, 3, fvals)) {
		    mtl.specular = cs237::color3f(fvals[0], fvals[1], fvals[2]);
		}
		else {
		    std::cerr << "Error: excepted \"Ks <float> <float> <float>\"" << std::endl;
		}
	    }
	    else if ((firstTok.compare("d") == 0) || (firstTok.compare("Tr") == 0)) {  // d <float> or Tr <float>
		if (scanFloats(rest, 1, fvals)) {
		    if (fvals[0] < 1.0) {
		      /* ignore transparency filter */
			std::cerr << "Warning: ignoring \"" << ln << "\" in \""
			    << file << "\"" << std::endl;
		    }
		}
		else {
		    std::cerr << "Error: excepted \"d <float>\"" << std::endl;
		}
	    }
#ifdef MORE_OBJ_FEATURES
	    else if (firstTok.compare("Tf") == 0) {  // Tf <float> <float> <float>
		/* ignore transparency filter */
	    }
#endif
	    else if (firstTok.compare("illum") == 0) {  // illumn <int>
		if (scanInt(rest, ival)) {
		    if ((0 <= ival) && (ival < 3)) {
			mtl.illum = ival;
		    }
		    else {
			std::cerr << "Warning: excepted \"illum [0-2]\"" << std::endl;
			mtl.illum = Specular;
		    }
		}
		else {
		    std::cerr << "Error: excepted \"illum <int>\"" << std::endl;
		}
	    }
	    else if (firstTok.compare("sharpness") == 0) {  // sharpness <float>
		if (scanFloats(rest, 1, fvals)) {
		  /* wavefront shininess is from [0, 1000], so scale for OpenGL */
		    mtl.shininess = 128.0f * (fvals[0] / 1000.0f);
		}
		else {
		    std::cerr << "Error: excepted \"sharpness <float>\"" << std::endl;
		}
	    }
	    else if (firstTok.compare("map_Kd") == 0) {  // map_Kd <name>
		mtl.diffuseMap = rest;
	    }
	    else if (firstTok.compare("map_Ka") == 0) {  // map_Ka <name>
		mtl.ambientMap = rest;
	    }
	    else if (firstTok.compare("map_Ks") == 0) {  // map_Ks <name>
		mtl.specularMap = rest;
	    }
	    else if ((firstTok.compare("map_Bump") == 0)
	    || (firstTok.compare("map_bump") == 0)
	    || (firstTok.compare("bump") == 0))
	    {
		mtl.normalMap = rest;
	    }
#ifdef MORE_OBJ_FEATURES
	    else if ((firstTok.compare("map_Refl") == 0) || (firstTok.compare("map_refl") == 0)) {
		/* ignore reflection map */
	    }
	    else if (firstTok.compare("map_d") == 0) {  // map_d <name>
		/* ignore transparency map */
	    }
#endif
	    else {
		std::cerr << "Warning: ignoring \"" << ln << "\" in \"" << file << "\"" << std::endl;
	    }
	} // while

	if (!inS.eof()) {
	    std::cerr << "Error: reading \"" << file << "\"" << std::endl;
	    return false;
	}

	if (count > 0) {
	    this->_materials.push_back(mtl);
	}

	return true;

    } // Model::readMaterial

    struct VInfo {
	uint32_t _v, _n, _t;

	VInfo (uint32_t v, uint32_t n, uint32_t t) : _v(v), _n(n), _t(t) { }

	struct Hash {
	    std::size_t operator() (VInfo const &v) const
	    {
		return static_cast<std::size_t>(
		    v._v +
		    (v._n << 5) * 101 +
		    (v._t << 10) * 101);
	    }
	};
	struct Equal {
	    bool operator()(VInfo const &v1, VInfo const &v2) const
	    {
		return (v1._v == v2._v) && (v1._n == v2._n) && (v1._t == v2._t);
	    }
	};

    }; // VInfo

    typedef std::unordered_map<VInfo,uint32_t,VInfo::Hash,VInfo::Equal> VertexMap_t;

    Model::Model (std::string file)
	: _path(file), _bbox()
    {
      // read the file
	OBJmodel *model = OBJReadOBJ (file.c_str());
	if (model == 0) {
	    std::cerr << "unable to read model \"" << file << "\"" << std::endl;
	    exit (1);
	}

      // load materials
	if (model->mtllibname != nullptr) {
	    this->_mtlLibName = model->mtllibname;
	    if (! this->readMaterial (this->_mtlLibName)) {
		std::cerr << "warning: error reading material library \"" << this->_mtlLibName << "\"" << std::endl;
		this->_materials.clear();
	    }
	}
	else {
	    this->_materials.clear();
	}

      // compute the bounding box
	for (int i = 0;  i < model->numvertices;  i++) {
	    this->_bbox.addPt (model->vertices[i]);
	}

      // build mesh data structures for the groups.  We need to identify unique v/n/t
      // triplets
	VertexMap_t map;
	std::vector<VInfo> verts;
	std::vector<uint32_t> indices;
	uint32_t idx;
	for (OBJgroup *grp = model->groups;  grp != nullptr;  grp = grp->next) {
	    for (uint32_t i = 0;  i < grp->numtriangles;  i++) {
		for (int j = 0;  j < 3;  j++) {
		    OBJtriangle *tri = &(model->triangles[i]);
		    VInfo v(tri->vindices[j], tri->nindices[j], tri->tindices[j]);
		    VertexMap_t::const_iterator got = map.find (v);
		    if (got == map.end()) {
			idx = verts.size();
			verts.push_back (v);
			map.insert (std::pair<VInfo,uint32_t>(v, idx));
		    }
		    else {
			idx = got->second;
		    }
		    indices.push_back(idx);
		}
	    }
	  // here we have identified the mesh vertices for the group
	    struct Group g;
	    g.name = std::string(grp->name);
	    g.material = -1;
	    if (grp->material == nullptr) {
		for (int i = 0;  i < this->_materials.size();  i++) {
		    if (this->_materials[i].name.compare("default") == 0) {
			g.material = i;
			break;
		    }
		}
		if (g.material == -1) {
		    std::cerr << "warning: no default material for group \"" << g.name << "\"" << std::endl;
		}
	    }
	    else {
		for (int i = 0;  i < this->_materials.size();  i++) {
		    if (this->_materials[i].name.compare(grp->material) == 0) {
			g.material = i;
			break;
		    }
		}
		if (g.material == -1) {
		    std::cerr << "warning: unable to find material \"" << grp->material
			<< "\" for group \"" << g.name << "\"" << std::endl;
		}
	    }
	  // initialize the vertex data arrays
	    g.nVerts = verts.size();
	    g.verts = new cs237::vec3f[g.nVerts];
	    if ((model->numnormals > 0) && (model->numtexcoords > 0)) {
	      // has all three components
		g.norms = new cs237::vec3f[g.nVerts];
		g.txtCoords = new cs237::vec2f[g.nVerts];
		for (uint32_t i = 0;  i < g.nVerts;  i++) {
		    g.verts[i] = model->vertices[verts[i]._v];
		    g.norms[i] = model->normals[verts[i]._n];
		    g.txtCoords[i] = model->texcoords[verts[i]._t];
		}
	    }
	    else if (model->numnormals > 0) {
		g.norms = new cs237::vec3f[g.nVerts];
		g.txtCoords = nullptr;
		for (uint32_t i = 0;  i < g.nVerts;  i++) {
		    g.verts[i] = model->vertices[verts[i]._v];
		    g.norms[i] = model->normals[verts[i]._n];
		}
	    }
	    else if (model->numtexcoords > 0) {
		g.norms = nullptr;
		g.txtCoords = new cs237::vec2f[g.nVerts];
		for (uint32_t i = 0;  i < g.nVerts;  i++) {
		    g.verts[i] = model->vertices[verts[i]._v];
		    g.txtCoords[i] = model->texcoords[verts[i]._t];
		}
	    }
	    else {
		g.norms = nullptr;
		g.txtCoords = nullptr;
		for (uint32_t i = 0;  i < g.nVerts;  i++) {
		    g.verts[i] = model->vertices[verts[i]._v];
		}
	    }
	  // initialize the group's indices array
	    g.nIndices = indices.size();
	    g.indices = new uint32_t[g.nIndices];
	    for (uint32_t i = 0;  i < g.nIndices;  i++) {
		g.indices[i] = indices[i];
	    }
	  // add to this model
	    this->_groups.push_back (g);
	  // cleanup
	    map.clear();
	    verts.clear();
	    indices.clear();
	}

	delete model;

    } // Model::Model

    Model::~Model ()
    {
      // free the storage for the groups
	for (int i = 0;  i < this->_groups.size();  i++) {
	    assert (this->_groups[i].verts != nullptr);
	    delete[] this->_groups[i].verts;
	    if (this->_groups[i].norms != nullptr) {
		delete[] this->_groups[i].norms;
	    }
	    if (this->_groups[i].txtCoords != nullptr) {
		delete[] this->_groups[i].txtCoords;
	    }
	    assert (this->_groups[i].indices != nullptr);
	    delete[] this->_groups[i].indices;
	}
    } // Model::~Model

} // namespace OBJ
