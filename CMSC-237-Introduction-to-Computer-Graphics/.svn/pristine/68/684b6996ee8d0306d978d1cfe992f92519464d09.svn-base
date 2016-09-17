/*! \file scene.cxx
 *
 * Code to load a scene description.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "json.hxx"
#include "scene.hxx"
#include <map>
#include <functional>
#include <iostream>

/* helper functions to make extracting object fields easier */

//! cast JSON value to JSON object
inline JSON::Object const *asObject (JSON::Value const *v)
{
    if (v == nullptr) return nullptr;
    else return v->asObject();
}

//! cast JSON value to JSON array
inline JSON::Array const *asArray (JSON::Value const *v)
{
    if (v == nullptr) return nullptr;
    else return v->asArray();
}

//! cast JSON value to JSON number
inline JSON::Number const *asNumber (JSON::Value const *v)
{
    if (v == nullptr) return nullptr;
    else return v->asNumber();
}

//! cast JSON value to JSON string
inline JSON::String const *asString (JSON::Value const *v)
{
    if (v == nullptr) return nullptr;
    else return v->asString();
}

//! retrieve the named field from a JSON object \arg v.  If \arg v is
//! nullptr, not an object, or if the field is not defined, then
//! return nullptr.
inline JSON::Value *field (const JSON::Value *v, const char *fld)
{
    if (v == nullptr)
	return nullptr;
    else {
	JSON::Object const *obj = v->asObject();
	if (obj == nullptr)
	    return nullptr;
	else
	    return (*obj)[fld];
    }
}

//! load a vec3f from a JSON object.
//! \return false if okay, true if there is an error.
bool LoadVec3 (JSON::Value const *jv, cs237::vec3f &vec)
{
    const JSON::Number *x = asNumber (field (jv, "x"));
    const JSON::Number *y = asNumber (field (jv, "y"));
    const JSON::Number *z = asNumber (field (jv, "z"));
    if ((x == nullptr) || (y == nullptr) || (z == nullptr)) {
	return true;
    }

    vec = cs237::vec3f (
	static_cast<float>(x->value()),
	static_cast<float>(y->value()),
	static_cast<float>(z->value()));

    return false;
}

//! load a color3f from a JSON object.
//! \return false if okay, true if there is an error.
bool LoadColor (JSON::Value const *jv, cs237::color3f &color)
{
    const JSON::Number *r = asNumber (field (jv, "r"));
    const JSON::Number *g = asNumber (field (jv, "g"));
    const JSON::Number *b = asNumber (field (jv, "b"));
    if ((r == nullptr) || (g == nullptr) || (b == nullptr)) {
	return true;
    }

    color = cs237::color3f (
	static_cast<float>(r->value()),
	static_cast<float>(g->value()),
	static_cast<float>(b->value()));

    return false;
}

//! load a ground size from a JSON object.
//! \return false if okay, true if there is an error.
bool LoadSize (JSON::Value const *jv, float &wid, float &ht)
{
    const JSON::Number *w = asNumber (field (jv, "wid"));
    const JSON::Number *h = asNumber (field (jv, "ht"));

    if ((w == nullptr) || (h == nullptr)) {
	return true;
    }
    else {
	wid = static_cast<float>(w->value());
	ht = static_cast<float>(h->value());
	return false;
    }

}

//! load a window size from a JSON object.
//! \return false if okay, true if there is an error.
bool LoadSize (JSON::Value const *jv, int &wid, int &ht)
{
    const JSON::Number *w = asNumber (field (jv, "wid"));
    const JSON::Number *h = asNumber (field (jv, "ht"));

    if ((w == nullptr) || (h == nullptr)) {
	return true;
    }
    else {
	wid = static_cast<int>(w->value());
	ht = static_cast<int>(h->value());
	return false;
    }

}

//! load a float from a JSON object.
//! \return false if okay, true if there is an error.
bool LoadFloat (JSON::Value const *jv, float &f)
{
    const JSON::Number *x = asNumber (jv);
    if (x == nullptr) {
	return true;
    }
    else {
	f = static_cast<float>(x->value());
	return false;
    }
}

/***** class Scene member functions *****/

bool Scene::Load (std::string path)
{
    if (this->_loaded) {
	std::cerr << "Scene is already loaded" << std::endl;
	return true;
    }
    this->_loaded = true;

    std::string sceneDir = path + "/";

  // load the scene description file
    JSON::Value *root = JSON::ParseFile(sceneDir + "scene.json");

  // check for errors
    if (root == nullptr) {
	std::cerr << "Unable to load scene \"" << path << "\"" << std::endl;
	return true;
    } else if (! root->isObject()) {
	std::cerr << "Invalid scene description in \"" << path
	    << "\"; root is not an object" << std::endl;
	return true;
    }

  // load the camera info
    JSON::Value *cam = field (root, "camera");
    if (LoadSize (field (cam, "size"), this->_wid, this->_ht)
    ||  LoadFloat (field (cam, "fov"), this->_fov)
    ||  LoadVec3 (field (cam, "pos"), this->_camPos)
    ||  LoadVec3 (field (cam, "look-at"), this->_camAt)
    ||  LoadVec3 (field (cam, "up"), this->_camUp)) {
	std::cerr << "Invalid scene description in \"" << path
	    << "\"; bad camera" << std::endl;
	return true;
    }

  // load the directional lighting information
    JSON::Value *light = field (root, "lighting");
    if (LoadVec3 (field (light, "direction"), this->_lightDir)
    ||  LoadColor (field (light, "intensity"), this->_lightI)
    ||  LoadColor (field (light, "ambient"), this->_ambI)
    ||  LoadFloat (field (light, "shadow"), this->_shadowFactor)) {
	std::cerr << "Invalid scene description in \"" << path
	    << "\"; bad lighting" << std::endl;
	return true;
    }
  //! make sure that the light direction is a unit vector
    this->_lightDir.normalize();
  //! make sure that color values are in 0..1 range
    this->_lightI.clamp();
    this->_ambI.clamp();

  // get the spot-light array from the JSON tree
    JSON::Array const *lights = asArray(field (light, "lights"));
    if (lights == nullptr) {
	std::cerr << "Invalid scene description in \"" << path
	    << "\"; bad lights array" << std::endl;
	return true;
    }

    if (lights->length() != 0) {
	this->_lights.resize(lights->length());
	for (int i = 0;  i < lights->length();  i++) {
	    JSON::Object const *light = (*lights)[i]->asObject();
	    JSON::String const *name = asString (field (light, "name"));
	    JSON::Object const *atten = asObject (field (light, "attenuation"));
	    if ((name == nullptr)
	    ||  LoadVec3 ((*light)["pos"], this->_lights[i].pos)
	    ||  LoadVec3 ((*light)["direction"], this->_lights[i].dir)
	    ||  LoadFloat ((*light)["cutoff"], this->_lights[i].cutoff)
	    ||  LoadFloat ((*light)["exponent"], this->_lights[i].exponent)
	    ||  LoadColor ((*light)["intensity"], this->_lights[i].intensity)
	    ||  LoadFloat ((*atten)["constant"], this->_lights[i].atten[0])
	    ||  LoadFloat ((*atten)["linear"], this->_lights[i].atten[1])
	    ||  LoadFloat ((*atten)["quadratic"], this->_lights[i].atten[2])) {
		std::cerr << "Invalid lights description in \"" << path << "\"" << std::endl;
		return true;
	    }
	    this->_lights[i].name = name->value();
	    this->_lights[i].dir.normalize();
	    this->_lights[i].intensity.clamp();
	  // check attenuation values to ensure that there is some attenuation
	    if ((this->_lights[i].atten[0] < 0.0f)
	    ||  (this->_lights[i].atten[1] < 0.0f)
	    ||  (this->_lights[i].atten[2] < 0.0f)
	    || ((this->_lights[i].atten[2] < 0.001f) && (this->_lights[i].atten[1] <= 1.0f))) {
		std::cerr << "Invalid attenuation factors for light "
		    << this->_lights[i].name << std::endl;
		return true;
	    }
	}
    }

  // get the object array from the JSON tree
    JSON::Array const *objs = asArray(field (root, "objects"));
    if (objs == nullptr) {
	std::cerr << "Invalid scene description in \"" << path
	    << "\"; bad objects array" << std::endl;
	return true;
    }

    if (objs->length() != 0) {
      // allocate space for the objects in the scene
	this->_objs.resize(objs->length());

      // we use a map to keep track of which models have already been loaded
	std::map<std::string, int> objMap;
	std::map<std::string, int>::iterator it;

      // load the objects in the scene
	int numModels = 0;
	for (int i = 0;  i < objs->length();  i++) {
	    JSON::Object const *object = (*objs)[i]->asObject();
	    JSON::String const *file = asString (field (object, "file"));
	    JSON::Object const *frame = asObject (field (object, "frame"));
	    cs237::vec3f pos, xAxis, yAxis, zAxis;
	    if ((file == nullptr) || (frame == nullptr)
	    ||  LoadVec3 ((*object)["pos"], pos)
	    ||  LoadVec3 ((*frame)["x-axis"], xAxis)
	    ||  LoadVec3 ((*frame)["y-axis"], yAxis)
	    ||  LoadVec3 ((*frame)["z-axis"], zAxis)
	    ||  LoadColor ((*object)["color"], this->_objs[i].color)) {
		std::cerr << "Invalid objects description in \"" << path << "\"" << std::endl;
		return true;
	    }
	  // have we already loaded this model?
	    it = objMap.find(file->value());
	    int modelId;
	    if (it != objMap.end()) {
		modelId = it->second;
	    }
	    else {
	      // load the model from the file sytem and add it to the map
		modelId = numModels++;
		OBJ::Model *model = new OBJ::Model (sceneDir + file->value());
		this->_models.push_back(model);
		objMap.insert (std::pair<std::string, int> (file->value(), modelId));
	    }
	    this->_objs[i].model = modelId;
	  // set the object-space to world-space transform
	    this->_objs[i].toWorld = cs237::mat4f (
		cs237::vec4f (xAxis, 0.0f),
		cs237::vec4f (yAxis, 0.0f),
		cs237::vec4f (zAxis, 0.0f),
		cs237::vec4f (pos, 1.0f));
	}

      // load the texture images used by the materials in the models
	for (auto modIt = this->_models.begin();  modIt != this->_models.end();  modIt++) {
	    const OBJ::Model *model = *modIt;
	    for (auto grpIt = model->beginGroups();  grpIt != model->endGroups();  grpIt++) {
		const OBJ::Material *mat = &model->Material((*grpIt).material);
		this->_LoadTexture (sceneDir, mat->diffuseMap);
		this->_LoadTexture (sceneDir, mat->normalMap);
	    }
	}
    }

  // load the ground information (if present)
    const JSON::Object *ground = asObject (field (root, "ground"));
    if (ground != nullptr) {
	float wid, ht;
	float vScale;
	cs237::color3f color;
	JSON::String const *hf = asString ((*ground)["height-field"]);
	JSON::String const *cmap = asString ((*ground)["color-map"]);
	JSON::String const *nmap = asString ((*ground)["normal-map"]);
	if ((hf == nullptr)
	||  LoadSize ((*ground)["size"], wid, ht)
	||  LoadFloat ((*ground)["v-scale"], vScale)
	||  LoadColor ((*ground)["color"], color)) {
	    std::cerr << "Invalid ground description in \"" << path << "\"" << std::endl;
	    return true;
	}
	cs237::image2d *cmapImg = nullptr;
	if (cmap != nullptr) {
	  // load the color-map texture
	    this->_LoadTexture (sceneDir, cmap->value());
	    cmapImg = this->TextureByName (cmap->value());
	}
	cs237::image2d *nmapImg = nullptr;
	if (nmap != nullptr) {
	  // load the normal-map texture
	    this->_LoadTexture (sceneDir, nmap->value());
	    nmapImg = this->TextureByName (nmap->value());
	}
	this->_hf = new HeightField (sceneDir + hf->value(), wid, ht, vScale, color, cmapImg, nmapImg);
    }
    else {
	this->_hf = nullptr;
    }

    if ((ground == nullptr) && (objs->length() == 0)) {
	std::cerr << "Invalid empty scene description in \"" << path << "\"" << std::endl;
	return true;
    }

  // free up the space used by the JSON object
    delete root;

    return false;
}

void Scene::_LoadTexture (std::string path, std::string name)
{
    if (name.empty()) {
	return;
    }
  // have we already loaded this texture?
    if (this->_texs.find(name) != this->_texs.end()) {
	return;
    }
  // load the image data; 
    cs237::image2d *img = new cs237::image2d(path + name);
  // add to _texs map
    this->_texs.insert (std::pair<std::string, cs237::image2d *>(name, img));

}

cs237::image2d *Scene::TextureByName (std::string name) const
{
    if (! name.empty()) {
	auto it = this->_texs.find(name);
	if (it != this->_texs.end()) {
	    return it->second;
	}
    }
    return nullptr;

}

Scene::Scene ()
    : _loaded(false), _wid(0), _ht(0), _fov(0),
      _camPos(), _camAt(), _camUp(),
      _models(), _objs()
{ }

Scene::~Scene ()
{
    for (auto it = this->_models.begin();  it != this->_models.end();  it++) {
	delete *it;
    }
    for (auto it = this->_texs.begin();  it != this->_texs.end();  it++) {
	delete it->second;
    }
}
