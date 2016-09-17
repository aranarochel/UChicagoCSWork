/*! \file scene.hxx
 *
 * \author John Reppy
 *
 * This file defines the Scene class, which represents the contents of a scene
 * loaded from disk.
 */

/* CMSC23700 Project 2 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _SCENE_HXX_
#define _SCENE_HXX_

#include <vector>
#include <string>
#include <map>
#include "cs237.hxx"
#include "obj.hxx"

//! an instance of a model, which has its own position and color.
struct SceneObj {
    int			model;	//!< the ID of the model that defines the object's mesh
    cs237::vec3f	pos;	//!< the world-space coordinates of the object
    cs237::color3f	color;	//!< the color of the object
};

//! a scene consisting of an initial camera configuration and some objects
class Scene {
  public:

  // constructor/destructor
    Scene ();
    ~Scene ();

  //! load a scene from the specified path
    bool Load (std::string path);

  //! the width of the viewport as specified by the scene
    int Width () const { return this->_wid; }

  //! the height of the viewport as specified by the scene
    int Height () const { return this->_ht; }

  //! the horizontal field of vew
    float HorizontalFOV () const { return this->_fov; }

  //! the initial camera position
    cs237::vec3f CameraPos () const { return this->_camPos; }

  //! the initial camera look-at point
    cs237::vec3f CameraLookAt () const { return this->_camAt; }

  //! the initial camera up vector
    cs237::vec3f CameraUp () const { return this->_camUp; }

  //! the light's unit-length direction vector
    cs237::vec3f LightDir () const { return this->_lightDir; }

  //! the directional light's intensity
    cs237::color3f LightIntensity () const { return this->_lightI; }

  //! the ambient light intensity in the scene
    cs237::color3f AmbientLight () const { return this->_ambI; }

  //! return the number of objects in the scene
    int NumObjects () const { return this->_objs.size(); }

  //! iterator for looping over the objects in the scene
    std::vector<SceneObj>::const_iterator beginObjs () const { return this->_objs.begin(); }

  //! terminator for looping over the objects in the scene
    std::vector<SceneObj>::const_iterator endObjs () const { return this->_objs.end(); }

  //! return the i'th object in the scene
    SceneObj const Object (int idx) const { return this->_objs[idx]; }

  //! return the number of distinct models in the scene
    int NumModels () const { return this->_models.size(); }

  //! iterator for looping over the models in the scene
    std::vector<OBJ::Model const *>::const_iterator beginModels () const
    {
	return this->_models.begin();
    }

  //! terminator for looping over the models in the scene
    std::vector<OBJ::Model const *>::const_iterator endModels () const
    {
	return this->_models.end();
    }

  //! return the i'th model in the scene
    const OBJ::Model *Model (int idx) const { return this->_models[idx]; }

  //! lookup a texture image by name
  //! \returns a pointer to the image object or nullptr if the image is not found
    cs237::image2d *TextureByName (std::string name) const;

  private:
    bool _loaded;		//!< has the scene been loaded?

    int _wid;			//!< initial window width
    int _ht;			//!< initial window height
    float _fov;			//!< horizontal field of view in degrees
    cs237::vec3f _camPos;	//!< camera position
    cs237::vec3f _camAt;	//!< the point at which the camera is pointing at
    cs237::vec3f _camUp;	//!< the camera's up vector
    cs237::vec3f _lightDir;	//!< unit vector that specifies the light's direction
    cs237::color3f _lightI;	//!< the directional light's intensity
    cs237::color3f _ambI;	//!< the ambient light's intensity

    std::vector<OBJ::Model const *> _models;	//!< the OBJ models in the scene
    std::vector<SceneObj> _objs;		//!< the objects in the scene
    std::map<std::string, cs237::image2d *> _texs;  //!< the textures keyed by name

  // helper function for loading textures into the _texs map
    void _LoadTexture (std::string path, std::string name);

};

#endif /* !_SCENE_HXX_ */
