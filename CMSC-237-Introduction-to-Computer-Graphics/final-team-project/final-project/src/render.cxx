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
#include <map>

#ifndef SHADER_DIR
#  define SHADER_DIR "../shaders/"
#endif

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
// We don't have a render Class so this is useless.