/*! \file tqt.cxx
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2014 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "tqt.hxx"

/***** inline utility functions *****/

//! return the number of nodes in a tree of the given depth
//! \param depth[in] the tree depth (> 0)
//! \return the number of nodes in the tree
inline uint32_t FullSize (uint32_t depth)
{
    return ((1 << (depth << 1)) - 1) & 0x55555555;
}

// Given a tree level and the indices of a node within that tree
// level, this function returns a node index.
inline uint32_t NodeIndex (uint32_t level, uint32_t row, uint32_t col)
{
    assert ((col >= 0) && (col < (1 << level)));
    assert ((row >= 0) && (row < (1 << level)));

    return FullSize(level) + (row << level) + col;
}

inline bool ReadUI32 (std::ifstream *inS, uint32_t &v)
{
    if (inS->read(reinterpret_cast<char *>(&v), sizeof(v)).fail()) {
#ifndef NDEBUG
	std::cerr << "TextureQTree::loadImage: error reading file" << std::endl;
#endif
	return false;
    }
    else
	return true;
}

inline bool ReadUI64 (std::ifstream *inS, uint64_t &v)
{
    if (inS->read(reinterpret_cast<char *>(&v), sizeof(v)).fail()) {
#ifndef NDEBUG
	std::cerr << "TextureQTree::loadImage: error reading file" << std::endl;
#endif
	return false;
    }
    else
	return true;
}

namespace TQT {

  // file header
    struct Hdr {
	uint32_t	magic;		// magic number
	uint32_t	version;	// file format version
	uint32_t	depth;		// tree depth
	uint32_t	tileSize;	// width of tiles; should be power of 2
    };

    #define TQT_MAGIC	0x00545154	// "TQT\0" in little-endian order
    #define TQT_VERSION	1

    static bool ReadHeader (std::ifstream *inS, Hdr &hdr)
    {
      // read header data
	if ((! ReadUI32(inS, hdr.magic))
	||  (! ReadUI32(inS, hdr.version))
	||  (! ReadUI32(inS, hdr.depth))
	||  (! ReadUI32(inS, hdr.tileSize))) {
	    return false;
	}

      // check data
	if ((hdr.magic != TQT_MAGIC) || (hdr.version != TQT_VERSION)) {
	    return false;
	}
// FIXME: should also check that depth is reasonable and tileSize is power of 2

	return true;
    }

    /***** class TextureQuadTree member functions *****/

    TextureQTree::TextureQTree (const char* filename)
	: _source(nullptr)
    {
	Hdr hdr;

	std::ifstream *inS = new std::ifstream(filename);
	if (inS->fail()) {
#ifndef NDEBUG
	    std::cerr << "TextureQTree::TextureQTree: unable to open \"" << filename << "\"" << std::endl;
	    delete inS;
#endif
	    exit (1);
	}
	else if (! ReadHeader(inS, hdr)) {
#ifndef NDEBUG
	    std::cerr << "TextureQTree::TextureQTree: file \"" << filename << "\" has bogus header" << std::endl;
	    delete inS;
#endif
	    exit (1);
	}
	else {
	    this->_depth = hdr.depth;
	    this->_tileSize = hdr.tileSize;
	    int nTiles = FullSize(hdr.depth);
	    this->_toc.resize(nTiles, 0);
	    this->_source = inS;
	  // read the TOC
	    for (int i = 0;  i < nTiles;  i++) {
		uint64_t offset;
		if (! ReadUI64(inS, offset)) {
#ifndef NDEBUG
		    std::cerr << "TextureQTree::TextureQTree: file \"" << filename << "\" has bogus TOC" << std::endl;
#endif
		    this->_source = nullptr;
		    inS->close();
		    delete inS;
		    exit (1);
		}
		this->_toc[i] = static_cast<std::streamoff>(offset);
	    }
	}
    }

    TextureQTree::~TextureQTree ()
    {
	if (this->_source != nullptr) {
	    this->_source->close();
	    delete this->_source;
	}
    }

    cs237::image2d *TextureQTree::LoadImage (int level, int row, int col, bool flip)
    {
	if (! this->isValid()) {
	    return nullptr;
	}
	assert (level < this->_depth);

	int index = NodeIndex(level, row, col);
	assert (index < this->_toc.size());
//std::cout << "toc[" << index << "] = " << this->_toc[index] << std::endl;

	this->_source->seekg(this->_toc[index]);
	cs237::image2d *img = new cs237::image2d (*(this->_source), flip);
	if ((img->width() != this->_tileSize)
	||  (img->height() != this->_tileSize)
	||  (img->format () != GL_RGB)) {
	    delete img;
	    return nullptr;
	}
	else {
	    return img;
	}
    }

  // Return true if the given file looks like a .tqt file of our
  // appropriate version.  Do this by attempting to read the header.
    /* static */ bool TextureQTree::isTQTFile (const char* filename)
    {
	std::ifstream inS(filename);
	if (inS.fail()) {
	    return false;
	}
	Hdr hdr;
	bool sts = ReadHeader (&inS, hdr);
	inS.close();
	return sts;
    }

};
