/*! \file tqt.hxx
 *
 * \author John Reppy
 *
 * Support for texture quadtrees.  This implementation is based on the public-domain
 * implementation by Thatcher Ulrich.  The main difference is that we use PNG files to
 * represent the texture data.
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _TQT_HXX_
#define _TQT_HXX_

#include "cs237.hxx"
#include <vector>

namespace TQT {

  //! Manages a disk-based texture quadtree.
    class TextureQTree {
      public:

	TextureQTree (const char* filename);
	~TextureQTree();

      //! is this a valid TQT?
	bool isValid () const { return this->_source != nullptr; }
      //! the depth of the TQT
	int Depth() const { return this->_depth; }
      //! the size of a texture tile measured in pixels (tiles are always square)
	int TileSize() const { return this->_tileSize; }

      //! return the image tile at the specified quadtree node.
      //! \param[in] level the level of the node in the tree (root = 0)
      //! \param[in] row the row of the node on its level (north == 0)
      //! \param[in] col the column of the node on its level (west == 0)
      //! \param[in] flip should the image be flipped to match OpenGL (default true)
      //! \return a pointer to the image; nullptr is returned if there is
      //!         an error.  It is the caller's responsibility to manage the
      //!         image's storage.
	cs237::image2d *LoadImage (int level, int row, int col, bool flip = true);

      //! return true if the file looks like a TQT file of the right version
	static bool isTQTFile (const char* filename);

      private:
	std::vector<std::streamoff> _toc;	//!< stream offsets for images
	int _depth;				//!< the depth of the TQT
	int _tileSize;				//!< the size of a texture tile in pixels across
	std::ifstream *_source;			//!< the source file for the textures

    };  // class TextureQTree

};

#endif // !_TQT_HXX_
