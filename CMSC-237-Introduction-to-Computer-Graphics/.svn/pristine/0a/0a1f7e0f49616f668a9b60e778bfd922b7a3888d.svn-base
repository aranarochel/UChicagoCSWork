/*! \file texture-cache.hxx
 *
 * \author John Reppy
 *
 * The interface to a caching mechanism for textures.
 */

/* CMSC23700 Final Project sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _TEXTURE_CACHE_
#define _TEXTURE_CACHE_

#include "tqt.hxx"
#include <unordered_map>
#include <vector>

class TextureCache;

class Texture {
  public:
    ~Texture ();

  //! is this texture active?
    bool isActive () const { return this->_active; }

  //! activate the texture; this operation is a hint to the texture
  //! cache that the texture is going to be used soon.
    void Activate ();

  //! bind this texture to the given texture unit (0 based)
    void Use (int txtUnit)
    {
	if (! this->_active) {
	    this->Activate();
	}
	CS237_CHECK( glActiveTexture (GL_TEXTURE0 + txtUnit) );
	this->_txt->Bind();
    }

  //! hint to the texture cache that this texture is not needed.
    void Release ();

  private:
    cs237::texture2D	*_txt;		//!< the OpenGL texture (or nullptr, if not resident)
    TextureCache	*_cache;	//!< the cache that this belongs to
    TQT::TextureQTree	*_tree;		//!< the texture quadtree from which this texture comes
    uint32_t		_level;		//!< the TQT level of this texture
    uint32_t		_row;		//!< the TQT row of this texture
    uint32_t		_col;		//!< the TQT column of this texture
    uint32_t		_lastUsed;	//!< the last frame that this texture was used
    int			_activeIdx;	//!< index of this texture in the cache's _active vector
    bool		_active;	//!< true when this texture is in use

    Texture (TextureCache *cache, TQT::TextureQTree *tree, int level, int row, int col);

    friend class TextureCache;
    friend struct TxtCompare;
};

//! A cache of OpenGL textures that is backed by texture-quad-trees
class TextureCache {
  public:

    TextureCache();
    ~TextureCache();

  //! make a texture handle for the specified quad in the texture quad tree
    Texture *Make (TQT::TextureQTree *tree, int level, int row, int col);

  //! mark the beginning of a new frame; the texture cache uses this information to
  //! track LRU information
    void NewFrame () { this->_clock++; }

  private:
    uint64_t	_residentLimit;	//!< soft upper bound on the size of GL resident textures
    uint64_t	_residentSzb;	//!< estimate of the size of GL resident textures
    uint32_t	_clock;		//!< counts number of frames

  //! keys for hashing texture specifications
    struct Key {
	TQT::TextureQTree	*_tree;	//!< the texture quadtree from which this texture comes
	int			_level;	//!< the TQT level of this texture
	int			_row;	//!< the TQT row of this texture
	int			_col;	//!< the TQT column of this texture

	Key (TQT::TextureQTree *t, int l, int r, int c)
	    : _tree(t), _level(l), _row(r), _col(c)
	{ }
    };
  //! hading keys
    struct Hash {
	std::size_t operator() (Key const &k) const
	{
	    return static_cast<std::size_t>(
		reinterpret_cast<std::size_t>(k._tree) +
		(static_cast<int>(k._level) << 5) * 101 +
		(static_cast<int>(k._row) << 10) * 101 +
		static_cast<int>(k._col));
	}
    };
  //! equality test on keys
    struct Equal {
	bool operator()(Key const &k1, Key const &k2) const
	{
	    return (k1._tree == k2._tree) && (k1._level == k2._level) && (k1._row == k2._row) && (k1._col == k2._col);
	}
    };

  // for ordering textures by timestamp
    struct TxtCompare {
	bool operator() (const Texture &lhs, const Texture &rhs) const
	{
	    return (lhs._lastUsed > rhs._lastUsed);
	}
    };

    typedef std::unordered_map<Key,Texture *,Hash,Equal> TextureTbl;

    TextureTbl _textureTbl;		//!< mapping from TQT spec to Texture
    std::vector<Texture *> _active;	//!< active textures
    std::vector<Texture *> _inactive;	//!< inactive textures that are loaded, but may be reused.

  //! record that the given texture is now active
    void _MakeActive (Texture *txt);

  //! record that the given texture is now inactive
    void _Release (Texture *txt);

  //! allocate an OpenGL texture, either by reusing a free texture or by creating a new one.
    cs237::texture2D *_AllocTex2D (cs237::image2d *img);

    friend class Texture;
};

#endif // !_TEXTURE_CACHE_
