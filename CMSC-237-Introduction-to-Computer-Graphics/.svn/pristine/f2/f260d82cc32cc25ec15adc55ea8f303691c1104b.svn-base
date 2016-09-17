/*! \file map.cxx
 *
 * \author John Reppy
 *
 * Information about heightfield maps.
 */

/* CMSC23700 Final Project sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "json.hxx"
#include "cs237.hxx"
#include "map.hxx"
#include "map-cell.hxx"

/***** class Map member functions *****/

Map::Map () : _grid(nullptr) { }

Map::~Map ()
{
    if (this->_grid != nullptr) {
	for (int i = 0;  i < this->_nCells();  i++) {
	    if (this->_grid[i] != nullptr)
		delete this->_grid[i];
	}
	delete this->_grid;
    }
	    
}

static void error (std::string file, std::string msg)
{
    std::cerr << "error reading map file \"" << file << "\": " << msg << std::endl;
}

// helper function to get an array of three numbers from the map object
static bool GetThreeFloats (const JSON::Object *root, std::string fld, float out[3])
{
    JSON::Value *v = (*root)[fld];
    if (v != nullptr) {
	const JSON::Array *arr = v->asArray();
	if ((arr == nullptr) || (arr->length() != 3)) {
	    return false;
	}
	for (int i = 0;  i < 3;  i++) {
	    const JSON::Number *num = (*arr)[i]->asNumber();
	    if (num == nullptr) {
		return false;
	    }
	    out[i] = float(num->value());
	}
    }

    return true;
}

bool Map::LoadMap (std::string mapName, bool verbose)
{
    if (this->_grid != nullptr) {
      // map file has already been loaded, so return false
	return false;
    }

    this->_path = mapName + "/";

    JSON::Value *map = JSON::ParseFile(this->_path + "map.json");
    const JSON::Object *root = (map != nullptr) ? map->asObject() : nullptr;

    if (root == nullptr) {
	std::cerr << "error reading map file" << std::endl;
	return false;
    }

  // get map name
    const JSON::String *name = (*root)["name"]->asString();
    if (name == nullptr) {
	error (mapName, "missing/bogus name field");
	return false;
    }
    this->_name = name->value();

    const JSON::Number *num;

  // get hScale
    num = (*root)["h-scale"]->asNumber();
    if (num == nullptr) {
	error (mapName, "missing/bogus h-scale field");
	return false;
    }
    this->_hScale = num->value();

  // get vScale
    num = (*root)["v-scale"]->asNumber();
    if (num == nullptr) {
	error (mapName, "missing/bogus v-scale field");
	return false;
    }
    this->_vScale = num->value();

  // get base elevation (optional)
    JSON::Value *v = (*root)["base-elev"];
    if (v != nullptr) {
	num = v->asNumber();
	if (num == nullptr) {
	    error (mapName, "bogus base-elev field");
	    return false;
	}
	this->_baseElev = num->value();
    }
    else
	this->_baseElev = 0.0;

  // get minimum elevation
    num = (*root)["min-elev"]->asNumber();
    if (num == nullptr) {
	error (mapName, "missing/bogus min-elev field");
	return false;
    }
    this->_minElev = num->value();

  // get maximum elevation
    num = (*root)["max-elev"]->asNumber();
    if (num == nullptr) {
	error (mapName, "missing/bogus max-elev field");
	return false;
    }
    this->_maxElev = num->value();

  // get map width
    num = (*root)["width"]->asNumber();
    if ((num == nullptr) || (num->value() < 0.0)) {
	error (mapName, "missing/bogus width field");
	return false;
    }
    this->_width = static_cast<uint32_t>(num->value());

  // get map height
    num = (*root)["height"]->asNumber();
    if ((num == nullptr) || (num->value() < 0.0)) {
	error (mapName, "missing/bogus height field");
	return false;
    }
    this->_height = static_cast<uint32_t>(num->value());

  // get cell size
    num = (*root)["cell-size"]->asNumber();
    if ((num == nullptr) || (num->value() < 0.0)) {
	error (mapName, "missing/bogus cell-size field");
	return false;
    }
    this->_cellSize = static_cast<uint32_t>(num->value());

  // check properties
    v = (*root)["color-map"];
    if (v != nullptr) {
	const JSON::Bool *b = v->asBool();
	if (b == nullptr) {
	    error (mapName, "bogus color-map field");
	    return false;
	}
	this->_hasColor = b->value();
    }
    else
	this->_hasColor = false;

    v = (*root)["normal-map"];
    if (v != nullptr) {
	const JSON::Bool *b = v->asBool();
	if (b == nullptr) {
	    error (mapName, "bogus normal-map field");
	    return false;
	}
	this->_hasNormals = b->value();
    }
    else
	this->_hasNormals = false;

    v = (*root)["water-map"];
    if (v != nullptr) {
	const JSON::Bool *b = v->asBool();
	if (b == nullptr) {
	    error (mapName, "bogus water-map field");
	    return false;
	}
	this->_hasWater = b->value();
    }
    else
	this->_hasWater = false;

  // lighting info (optional)
    {
	float sunDir[3] = { 0.0, 1.0, 0.0 };  // default is high noon
	float sunI[3] = { 0.9, 0.9, 0.9 };  // bright sun
	float ambI[3] = { 0.1, 0.1, 0.1 };

	if (GetThreeFloats (root, "sun-dir", sunDir)) {
	    this->_sunDir = normalize (cs237::vec3f(sunDir[0], sunDir[1], sunDir[2]));
	}
	else {
	    error (mapName, "bogus sun-dir field");
	    return false;
	}
	if (GetThreeFloats (root, "sun-intensity", sunI)) {
	    this->_sunI = cs237::color3f(sunI[0], sunI[1], sunI[2]);
	}
	else {
	    error (mapName, "bogus sun-intensity field");
	    return false;
	}
	if (GetThreeFloats (root, "ambient", ambI)) {
	    this->_ambI = cs237::color3f(ambI[0], ambI[1], ambI[2]);
	}
	else {
	    error (mapName, "bogus ambient field");
	    return false;
	}
    }

  // compute and check other map info
    int cellShft = ilog2(this->_cellSize);
    if ((cellShft < 0)
    || (this->_cellSize < Map::MIN_CELL_SIZE)
    || (Map::MAX_CELL_SIZE < this->_cellSize)) {
	error (mapName, "cellSize must be power of 2 in range");
	return false;
    }

    this->_nRows = this->_height >> cellShft;
    this->_nCols = this->_width >> cellShft;
    if ((this->_nRows << cellShft) != this->_height) {
	error (mapName, "map height must be multiple of cell size");
	return false;
    }
    if ((this->_nCols << cellShft) != this->_width) {
	error (mapName, "map width must be multiple of cell size");
	return false;
    }

    if (verbose) {
	std::clog << "name = " << this->_name << std::endl;
	std::clog << "h-scale = " << this->_hScale << std::endl;
	std::clog << "v-scale = " << this->_vScale << std::endl;
	std::clog << "base-elev = " << this->_baseElev << std::endl;
	std::clog << "min-elev = " << this->_minElev << std::endl;
	std::clog << "max-elev = " << this->_maxElev << std::endl;
	std::clog << "width = " << this->_width
	    << " (" << this->_nCols << " cols)" << std::endl;
	std::clog << "height = " << this->_height
	    << " (" << this->_nRows << " rows)" << std::endl;
	std::clog << "cell-size = " << this->_cellSize << std::endl;
	std::clog << "sun-dir = " << this->_sunDir << std::endl;
	std::clog << "sun-intensity = " << this->_sunI << std::endl;
	std::clog << "ambient = " << this->_ambI << std::endl;
    }

  // get array of grid filenames
    const JSON::Array *grid = (*root)["grid"]->asArray();
    if (num == nullptr) {
	error (mapName, "missing/bogus h-scale field");
	return false;
    }
    else if (grid->length() != this->_nCells()) {
	error (mapName, "incorrect number of cells in grid field");
	return false;
    }
    this->_grid = new class Cell*[this->_nCells()];
    for (int r = 0;  r < this->_nRows;  r++) {
	for (int c = 0;  c < this->_nCols;  c++) {
	    int i = this->_cellIdx(r, c);
	    const JSON::String *s = (*grid)[i]->asString();
	    if (s == nullptr) {
		error (mapName, "bogus grid item");
	    }
	    this->_grid[i] = new class Cell(this, r, c, this->_path + s->value());
	}
    }

    return true;

}


/***** Utility functions *****/

// return the integer log2 of n; if n is not a power of 2, then return -1.
int ilog2 (uint32_t n)
{
    int k = 0, two_k = 1;
    while (two_k < n) {
	k++;
	two_k *= 2;
	if (two_k == n)
	    return k;
    }
    return -1;

}

