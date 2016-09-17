/*! \file render.cxx
 *
 * \author John Reppy
 *
 * These are additional functions for the View, Cell, Tile, and Chunk classes for
 * rendering the mesh.
 */

/* CMSC23700 Final Project sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "view.hxx"
#include "camera.hxx"
#include "map-cell.hxx"
#include "buffer-cache.hxx"
#include "texture-cache.hxx"

//! Colors to use for rendering wireframes at different levels of detail
static cs237::color4ub MeshColor[8] = {
	cs237::color4ub{ 255, 255,   0, 255 },
	cs237::color4ub{ 255,   0, 255, 255 },
	cs237::color4ub{   0, 255, 255, 255 },
	cs237::color4ub{ 255, 128, 128, 255 },
	cs237::color4ub{ 128, 255, 128, 255 },
	cs237::color4ub{ 128, 128, 255, 255 },
	cs237::color4ub{ 255,   0,   0, 255 },
	cs237::color4ub{   0, 255,   0, 255 }
    };


void View::Render ()
{
    if (! this->_isVis)
	return;

    double now = glfwGetTime();
    float dt = float(now - this->_lastFrameTime);
    this->_lastFrameTime = now;

    /* YOUR CODE HERE */

    glfwSwapBuffers (this->_window);

}

void Cell::InitTextures (View *view)
{
  // load textures
    if (this->_map->hasColorMap()) {
	this->_colorTQT = new TQT::TextureQTree (this->Datafile("/color.tqt").c_str());
    }
    if (this->_map->hasNormalMap()) {
	this->_normTQT = new TQT::TextureQTree (this->Datafile("/norm.tqt").c_str());
    }
#ifndef NDEBUG
    if ((this->_colorTQT != nullptr) && (this->_normTQT != nullptr)) {
	assert (this->_colorTQT->Depth() == this->_normTQT->Depth());
    }
#endif

}
