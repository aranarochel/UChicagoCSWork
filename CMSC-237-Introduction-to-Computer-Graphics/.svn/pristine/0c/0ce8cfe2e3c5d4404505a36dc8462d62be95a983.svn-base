/*! \file map-cell.hxx
 *
 * \author John Reppy
 */

/* CMSC23700 Final Project sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _MAP_CELL_HXX_
#define _MAP_CELL_HXX_

#include "map.hxx"
#include "qtree-util.hxx"
#include "tqt.hxx"

class Tile;

class Cell {
  public:

  //! Cell constructor
  //! \param[in] map  the map containing this cell
  //! \param[in] r    this cell's row in the grid of cells
  //! \param[in] c    this cell's column in the grid of cells
  //! \param[in] stem the stem or path prefix used to load this cell's data
    Cell (Map *map, uint32_t r, uint32_t c, std::string const &stem);

    ~Cell ();

  //! load the cell data from the "hf.cell" file
    void Load ();

  //! returns true if cell data has been loaded
    bool isLoaded () const { return (this->_tiles != nullptr); }

  //! the row of this cell in the grid of cells in the map
    int Row () const { return this->_row; }
  //! the column of this cell in the grid of cells in the map
    int Col () const { return this->_col; }
  //! the number of levels of detail supported by this cell
    int Depth () const { return this->_nLODs; }
  //! the width of this cell in hScale units.  The number of vertices across will be width()+1
    int Width () const { return this->_map->CellWidth(); }
  //! get the map horizontal scale
    float hScale () const { return this->_map->_hScale; }
  //! get the map vertical scale
    float vScale () const { return this->_map->_vScale; }

  //! return the path of a data file  for this cell
  //! \param[in] file the name of the file
  //! \return the path to the file for this cell.
    std::string Datafile (std::string const &file);

  //! get a particular tile; we assume that the cell data has been loaded
    class Tile &Tile (int id);

  //! initialize the textures for the cell
    void InitTextures (class View *view);

  //! the color texture-quad-tree for this cell (nullptr if not present)
    TQT::TextureQTree *ColorTQT () const { return this->_colorTQT; }
  //! the normal-map texture-quad-tree for this cell (nullptr if not present)
    TQT::TextureQTree *NormTQT () const { return this->_normTQT; }

  // constants
    static const uint32_t MAGIC = 0x63656C6C;  // 'cell'
    static const uint32_t MIN_NUM_LODS = 1;
    static const uint32_t MAX_NUM_LODS = 8;

  private:
    Map		*_map;		//!< the map containing this cell
    uint32_t	_row, _col;	//!< the row and column of this cell in its map
    std::string _stem;		//!< prefix of pathnames for access cell data files
    uint32_t	_nLODs;		//!< number of levels of detail in this cell's representation
    uint32_t	_nTiles;	//!< the number of tiles
    class Tile	*_tiles;	//!< the complete quadtree of tiles
    TQT::TextureQTree *_colorTQT; //!< texture quadtree for the cell's color map (nullptr if
				//! not present)
    TQT::TextureQTree *_normTQT; //!< texture quadtree for the cell's normal map (nullptr if
				//! not present)

    class Tile *LoadTile (int id);

};

//! packed vertex representation
struct Vertex {
    int16_t	_x;		//!< x coordinate relative to Cell's NW corner (in hScale units)
    int16_t	_y;		//!< y coordinate relative to Cell's base elevation (in vScale units)
    int16_t	_z;		//!< z coordinate relative to Cell's NW corner (in hScale units)
    int16_t	_morphDelta;	//!< y morph target relative to _y (in vScale units)
};

//! LOD mesh chunk
struct Chunk {
    float	_maxError;	//<! maximum geometric error (in meters) for this chunk
    int16_t	_minY;		//<! minimum Y value of the vertices in this chunk
    int16_t	_maxY;		//<! maximum Y value of the vertices in this chunk
    uint32_t	_nVertices;	//!< number of vertices in chunk; should be < 2^16
    uint32_t	_nIndices;	//!< number of indices in chunk
    Vertex	*_vertices;	//!< vertex array; each vertex is packed into 64-bits
    uint16_t	*_indices;	//!< index array

    size_t vSize() const { return this->_nVertices * sizeof(Vertex); }
    size_t iSize() const { return this->_nIndices * sizeof(uint16_t); }
};

//! A tile is a node in the LOD quadtree.  It contains the mesh data for the corresponding
//! LOD chunk, and can also be used to attach other useful information (such as the chunk's
//! bounding box).
class Tile {
  public:

    Tile();
    ~Tile();

  //! the row of this tile's NW vertex in its cell
    uint32_t NWRow () const { return this->_row; }
  //! the column of this tile's NW vertex in its cell
    uint32_t NWCol () const { return this->_col; }
  //! the width of this cell in hScale units.  The number of vertices across will be width()+1
    uint32_t Width () const { return this->_cell->Width() >> this->_lod; }
  //! the level of detail of this tile (0 is coarsest)
    int LOD () const { return this->_lod; }

  //! read-only access to mesh data for this tile
    struct Chunk const & Chunk() const { return this->_chunk; }

  //! the tile's bounding box in world coordinates
    cs237::AABBd const & BBox () const { return this->_bbox; }

  //! return the i'th child of this tile (nullptr if the tile is a leaf)
    Tile *Child (int i) const;

  // return the number of children
    int NumChildren () const;

  // dump the tile tree to an output stream
    void Dump (std::ostream &outS);

  private:
    Cell	*_cell;		//!< the cell that contains this tile
    uint32_t	_id;		//!< the ID of this tile, which is also its index in the quadtree array
    uint32_t	_row;		//<! the row of this tile's NW vertex in its cell
    uint32_t	_col;		//<! the column of this tile's NW vertex in its cell
    int32_t	_lod;		//!< the level of detail of this tile (0 == coarsest)
    struct Chunk _chunk;	//<! mesh data for this tile
    cs237::AABBd _bbox;		//<! the tile's bounding box in world coordinates; note that we use
				//!  double precision here so that we can support large worlds	

  //! initialize the _cell, _id, etc. fields of this tile and its descendants.  The chunk and
  //! bounding box get set later
    void _Init (Cell *cell, uint32_t id, uint32_t row, uint32_t col, uint32_t lod);

  //! allocate memory for the chunk
    void _AllocChunk (uint32_t nv, uint32_t ni);

    friend class Cell;
};

/***** Inline functions *****/

inline std::string Cell::Datafile (std::string const &file)
{
    return this->_stem + file;
}

inline class Tile &Cell::Tile (int id)
{
    assert (this->isLoaded());
    assert ((0 <= id) && (id < this->_nTiles));

    return this->_tiles[id];
}

inline class Tile *Tile::Child (int i) const
{
    assert ((0 <= i) && (i < 4));
    if (this->_lod+1 < this->_cell->Depth())
	return &(this->_cell->Tile(QTree::NWChild(this->_id) + i));
    else
	return nullptr;
}

inline int Tile::NumChildren () const
{
    if (this->_lod+1 < this->_cell->Depth())
	return 4;
    else
	return 0;
}

#endif //! _MAP_CELL_HXX_
