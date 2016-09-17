/*! \file scene.cxx
 *
 * Code to load a scene description.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Autumn 2015)
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

  // load the scene description file
    JSON::Value *root = JSON::ParseFile(path + "/scene.json");

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

  // get the object array from the JSON tree and check that it is non-empty
    JSON::Array const *objs = asArray(field (root, "objects"));
    if ((objs == nullptr) || (objs->length() == 0)) {
	std::cerr << "Invalid scene description in \"" << path
	    << "\"; bad objects array" << std::endl;
	return true;
    }

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
	if ((file == nullptr)
	||  LoadVec3 ((*object)["pos"], this->_objs[i].pos)
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
	    OBJ::Model *model = new OBJ::Model (path + "/" + file->value());
	    this->_models.push_back(model);
	    objMap.insert (std::pair<std::string, int> (file->value(), modelId));
	}
	this->_objs[i].model = modelId;
    }

    delete root;

    return false;
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
}
