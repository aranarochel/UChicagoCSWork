/*! \file map-cell.cxx
 *
 * \author John Reppy
 *
 * Map cells.
 */

/* CMSC23700 Final Project sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "map.hxx"
#include "map-cell.hxx"
#include "qtree-util.hxx"
#include <fstream>
#include <vector>
#include <iomanip>

// A cell file has the following layout on disk.  All data is in little-endian layout.
//
//	uint32_t magic;		// Magic number; should be 0x63656C6C ('cell')
//	uint32_t compressed;	// true if the chunks are compressed
//	uint32_t size;		// cell width (will be width+1 vertices wide)
//	uint32_t nLODs;
//	uint64_t toc[N];	// file offsets of chunks
//
// Each chunk has the layout
//
//	float maxError;		// maximum geometric error for this chunk
//	uint32_t nVerts;	// number of vertices
//	uint32_t nIndices;	// number of indices
//	int16_t minY;		// minimum active elevation value in this chunk
//	int16_t maxY;		// maximum active elevation value in this chunk
//	Vertex verts[nVerts];
//	uint16_t indices[nIndices];
//
// Each Vertex is represented by four 16-bit signed integers.

// A generic helper function for reading binary values from a input file
template <typename T>
inline T ReadVal (std::ifstream &inS)
{
    T v;
    
    if (inS.read(reinterpret_cast<char *>(&v), sizeof(T)).fail()) {
#ifndef NDEBUG
	std::cerr << "Cell::load: error reading file" << std::endl;
#endif
	exit (1);
    }
    return v;
}

inline int16_t ReadI16 (std::ifstream &inS) { return ReadVal<int16_t>(inS); }
inline uint32_t ReadUI32 (std::ifstream &inS) { return ReadVal<uint32_t>(inS); }
inline float ReadF32 (std::ifstream &inS) { return ReadVal<float>(inS); }
inline uint64_t ReadUI64 (std::ifstream &inS) { return ReadVal<uint64_t>(inS); }


/***** class Cell member functions *****/

Cell::Cell (Map *map, uint32_t r, uint32_t c, std::string const &stem)
    : _map(map), _row(r), _col(c), _stem(stem), _nLODs(0), _nTiles(0), _tiles(nullptr),
      _colorTQT(nullptr), _normTQT(nullptr)
{
}

Cell::~Cell () { }

// load the cell data
void Cell::Load ()
{
    if (this->isLoaded())
	return;

    std::string file = this->_stem + "/hf.cell";
    std::ifstream inS(file, std::ifstream::in | std::ifstream::binary);
    if (inS.fail()) {
#ifndef NDEBUG
	std::cerr << "Cell::load: unable to open \"" << file << "\"" << std::endl;
#endif
	exit (1);
    }

  // get header info
    uint32_t magic = ReadUI32(inS);
    bool compressed = (ReadUI32(inS) != 0);
    uint32_t size = ReadUI32(inS);
    uint32_t nLODs = ReadUI32(inS);
    if (magic != Cell::MAGIC) {
#ifndef NDEBUG
	std::cerr << "Cell::load: bogus magic number in header" << std::endl;
#endif
	exit (1);
    }
    else if (this->_map->_cellSize != size) {
#ifndef NDEBUG
	std::cerr << "Cell::load: expected cell size " << this->_map->_cellSize << " but found " << size << std::endl;
#endif
	exit (1);
    }
    else if ((nLODs < Cell::MIN_NUM_LODS) || (Cell::MAX_NUM_LODS < nLODs)) {
#ifndef NDEBUG
	std::cerr << "Cell::load: unsupported number of LODs" << std::endl;
#endif
	exit (1);
    }

    if (compressed) {
	std::cerr << "Cell::load: compressed files are not supported yet" << std::endl;
	exit (1);
    }

    uint32_t qtreeSize = QTree::FullSize(nLODs);
    std::vector<std::streamoff> toc(qtreeSize);
    for (int i = 0;  i < qtreeSize;  i++) {
	toc[i] = static_cast<std::streamoff>(ReadUI64(inS));
    }

  // allocate and load the tiles.  Note that tiles are numbered in a breadth-first
  // order in the LOD quadtree.
    this->_nLODs = nLODs;
    this->_nTiles = qtreeSize;
    this->_tiles = new class Tile[qtreeSize];

    this->_tiles[0]._Init (this, 0, 0, 0, 0);

  // load the tile mesh data
    for (uint32_t id = 0;  id < qtreeSize;  id++) {
	Chunk *cp = &(this->_tiles[id]._chunk);
      // find the beginning of the chunk in the input file
	inS.seekg(toc[id]);
      // read the chunk's header
	cp->_maxError = ReadF32(inS);
	uint32_t nVerts = ReadUI32(inS);
	uint32_t nIndices = ReadUI32(inS);
	cp->_minY = ReadI16(inS);
	cp->_maxY = ReadI16(inS);
      // allocate space for the chunk data
	this->_tiles[id]._AllocChunk(nVerts, nIndices);
      // read the vertex data
	if (inS.read(reinterpret_cast<char *>(cp->_vertices), cp->vSize()).fail()) {
	    std::cerr << "Cell::load: error reading vertex data for tile " << id << std::endl;
	    exit (1);
	}
      // read the index array
	if (inS.read(reinterpret_cast<char *>(cp->_indices), cp->iSize()).fail()) {
	    std::cerr << "Cell::load: error reading index data for tile " << id << std::endl;
	    exit (1);
	}
      // compute the tile's bounding box.  We use double precision here, so that we can
      // support large worlds.
	cs237::vec3d nwCorner =
	    this->_map->NWCellCorner(this->_row, this->_col) +
	    cs237::vec3d(
		this->_map->hScale() * double(this->_tiles[id]._col),
		double(this->_map->BaseElevation() + this->_map->vScale() * float(cp->_minY)),
		this->_map->hScale() * double(this->_tiles[id]._row));
	double w = this->_map->hScale() * this->_tiles[id].Width();
	cs237::vec3d seCorner = nwCorner + cs237::vec3d(w, 0.0, w);
	seCorner.y = static_cast<double>(
	    this->_map->BaseElevation() + this->_map->vScale() * float(cp->_maxY));
	this->_tiles[id]._bbox = cs237::AABBd(nwCorner, seCorner);
    }

}

/***** class Tile member functions *****/

Tile::Tile ()
{
    this->_chunk._nVertices = 0;
    this->_chunk._nIndices = 0;
    this->_chunk._vertices = nullptr;
    this->_chunk._indices = nullptr;
}

Tile::~Tile ()
{
    delete this->_chunk._vertices;
    delete this->_chunk._indices;
}

void Tile::_AllocChunk (uint32_t nv, uint32_t ni)
{
    this->_chunk._nVertices = nv;
    this->_chunk._nIndices = ni;
    this->_chunk._vertices = new Vertex[nv];
    this->_chunk._indices = new uint16_t[ni];
}

// initialize the _cell, _id, etc. fields of this tile and its descendants.  The chunk and
// bounding box get set later
void Tile::_Init (Cell *cell, uint32_t id, uint32_t row, uint32_t col, uint32_t lod)
{
    this->_cell = cell;
    this->_id = id;
    this->_row = row;
    this->_col = col;
    this->_lod = lod;

    if (lod+1 < cell->Depth()) {
	uint32_t halfWid = (cell->Width() >> (lod+1));
	struct { uint32_t dr, dc; } offset[4] = {
		{ 0,       0       }, // NW
		{ 0,       halfWid }, // NE
		{ halfWid, halfWid }, // SE
		{ halfWid, 0       }, // SW
	    };
	int kidId = QTree::NWChild(id);
	for (int i = 0;  i < 4;  i++) {
	    this->Child(i)->_Init (cell, kidId+i, row+offset[i].dr, col+offset[i].dc, lod+1);
	}
    }

}

void Tile::Dump (std::ostream &outS)
{
    for (int i = 0;  i < this->_lod;  i++) {
	outS << "  ";
    }

    outS << "[" << std::setw(4) << this->_id << "]"
/* ADDITIONAL TILE STATE HERE */
	<< std::endl;

    for (int i = 0;  i < this->NumChildren();  i++)
	this->Child(i)->Dump (outS);

}
