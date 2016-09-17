/*! \file ground.hxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _GROUND_HXX_
#define _GROUND_HXX_

#include "cs237.hxx"
#include "height-field.hxx"
#include "mesh.hxx"
#include "instance.hxx"

class Ground : public MeshInfo {
  public:

    Ground (const HeightField *hf);

    ~Ground();

  //! return the world-space AABB for the ground
    cs237::AABBf const &BBox () const { return this->_bbox; }

  //! return the ground as a drawable instance
    const struct Instance *Instance() const { return &(this->_instance); }

  private:
    cs237::AABBf	_bbox;		//!< the bounding box around the ground
    struct Instance	_instance;	//!< wrapper so that we can pass this to Render::Render

};

#endif //! _GROUND_HXX_
