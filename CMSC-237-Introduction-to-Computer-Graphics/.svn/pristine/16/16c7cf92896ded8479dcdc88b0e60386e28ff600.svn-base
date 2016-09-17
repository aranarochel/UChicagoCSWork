/*! \file qtree-util.hxx
 *
 * \author John Reppy
 *
 * Utility functions for dealing with the array representation of a complete quadtree.
 */

/* CMSC23700 Final Project sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _QTREE_UTIL_HXX_
#define _QTREE_UTIL_HXX_

namespace QTree {

    //! the clockwise ordering of the quadtree quadrants
    enum {
	NW, NE, SE, SW
    };

    //! the parent ID of the node
    //! \param[in] id the node's id
    //! \return the id of the node's parent; parent(0) == 0
    inline uint32_t Parent (uint32_t id)
    {
	if (id > 0)
	    return (id - 1) >> 2;
	else
	    return 0;
    }

    //! the child index of this node w.r.t. its parent
    //! \param[in] id the node's id
    //! \return the child index (NW, NE, SE, or SW); parent(0) == NW
    inline uint32_t ChildIndex (uint32_t id)
    {
	if (id > 0)
	    return (id - 1) & 0x3;
	else
	    return 0;
    }

    //! the first (NW) child of a node
    //! \param[in] id the node's id
    //! \return the id of the NW child of the node.
    inline uint32_t NWChild (uint32_t id)
    {
	return (id << 2) + 1;
    }

    //! the second (NE) child of a node
    //! \param[in] id the node's id
    //! \return the id of the NW child of the node.
    inline uint32_t NEChild (uint32_t id)
    {
	return (id << 2) + 2;
    }

    //! the third (SE) child of a node
    //! \param[in] id the node's id
    //! \return the id of the NW child of the node.
    inline uint32_t SEChild (uint32_t id)
    {
	return (id << 2) + 3;
    }

    //! the fourth (SW) child of a node
    //! \param[in] id the node's id
    //! \return the id of the NW child of the node.
    inline uint32_t SWChild (uint32_t id)
    {
	return (id << 2) + 4;
    }

    //! the NW sibling of a node
    //! \param[in] id the node's id
    //! \return the id of the NW child of the node's parent.
    inline uint32_t NWSibling (uint32_t id)
    {
	if (id == 0)
	    return 0;
	else
	    return (id-1 & ~0x3) + 1;
    }

    //! return the number of nodes at a given level in a quadtree
    //! \param[in] depth the tree depth (>= 0)
    //! \return the number of nodes at depth in the tree
    inline uint32_t NumNodesAt (uint32_t depth)
    {
	return (1 << (depth << 1));
    }

    //! return the total number of nodes in a tree of the given depth
    //! \param[in] depth the tree depth (> 0)
    //! \return the number of nodes in the tree
    inline uint32_t FullSize (uint32_t depth)
    {
	return ((1 << (depth << 1)) - 1) & 0x55555555;
    }

}; // namespace QTree

#endif // !_QTREE_UTIL_HXX_
