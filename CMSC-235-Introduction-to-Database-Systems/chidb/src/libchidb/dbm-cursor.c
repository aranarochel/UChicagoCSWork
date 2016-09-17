/*
 *  chidb - a didactic relational database management system
 *
 *  Database Machine cursors
 *
 */

/*
 *  Copyright (c) 2009-2015, The University of Chicago
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or withsend
 *  modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  - Neither the name of The University of Chicago nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software withsend specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY send OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include "dbm-cursor.h"

int chidb_dbm_cursor_print(chidb_dbm_cursor_t *c)
{
    fprintf(stderr, "\n");
    fprintf(stderr, "+++++++++ CURSOR PRINTOUT +++++++\n");
    fprintf(stderr, "Current Cell Type: %d\n", c->current_cell.type);
    fprintf(stderr, "Current Key: %d\n", c->current_cell.key);
    fprintf(stderr, "Root Page: %d\n", c->root_page);
    fprintf(stderr, "Root type: %d\n", c->root_type);
    fprintf(stderr, "Number of Columns: %d\n", c->n_cols);
    fprintf(stderr, "Cursor Type: %s\n", c->type == CURSOR_READ ? "read" : "write");
    fprintf(stderr, "====== Starting Trail Print ======\n");

    chidb_dbm_cursor_trail_t *ct;
    list_t *l = &(c->trail);
    list_iterator_start(l);
    while(list_iterator_hasnext(l))
    {
        ct = (chidb_dbm_cursor_trail_t *)list_iterator_next(l);
        fprintf(stderr, "Depth: %d\n", ct->depth);
        fprintf(stderr, "Number Current Cell: %d\n", ct->n_current_cell);
        fprintf(stderr, "Page Number: %d\n", ct->btn->page->npage);
        fprintf(stderr, "-------------------------------\n");
    }

    fprintf(stderr, "+++++++++ DONE PRINTING CURSOR ++++++++++++\n");
    fprintf(stderr, "\n");
    list_iterator_stop(l);

    return CHIDB_OK;
}

/* Creates a new cursor trail and allocates memory for it
 * 
 * Return
 * - CHIDB_OK: Operation sucessful
 * - CHIDB_ENOMEM: Malloc failed
 */
int chidb_dbm_cursor_trail_new(BTree *bt, chidb_dbm_cursor_trail_t **ct, npage_t npage, uint32_t depth)
{
    (*ct) = malloc(sizeof(chidb_dbm_cursor_trail_t));
    if((*ct)==NULL)
        return CHIDB_ENOMEM;

    BTreeNode *btn;
    int ret = chidb_Btree_getNodeByPage(bt, npage, &btn);
    if(ret != CHIDB_OK)
        return ret;

    // populate!
    (*ct)->depth = depth;
    (*ct)->btn = btn;
    (*ct)->n_current_cell = 0;

    return CHIDB_OK;
}

/* Frees a cursor trail.
 * expects that the type of elements in the list are cursor_trail *'s 
 *
 * Return
 * - CHIDB_OK: Operation sucessful
 */
int chidb_dbm_cursor_trail_list_destroy(BTree *bt, list_t *restrict l)
{
    // free all of the btn's held within the cursor trail structs and all of the trail structs
    chidb_dbm_cursor_trail_t *ct;

    while(!list_empty(l))
    {
        ct = (chidb_dbm_cursor_trail_t *)list_fetch(l);
        chidb_Btree_freeMemNode(bt, ct->btn);
        free(ct);    
    }
    
    // list_iterator_start(l);
    // while(list_iterator_hasnext(l))
    // {
    //     ct = (chidb_dbm_cursor_trail_t *)list_iterator_next(l);
    //     chidb_Btree_freeMemNode(bt, ct->btn);
    //     // free(ct);
    // }
    // list_iterator_stop(l);

    list_destroy(l); // this frees all elements as per simclist.h

    return CHIDB_OK;
}

/* Makes a copy of a whole trail list
 *
 * Return
 * - CHIDB_OK: Operation sucessful
 * - CHIDB_ENOMEM: Malloc failed
 */
int chidb_dbm_cursor_trail_cpy(BTree *bt, list_t *restrict l1, list_t *restrict l2)
{
    chidb_dbm_cursor_trail_t *ct_next, *ct_next_cpy;
    BTreeNode *btncpy;

    // initialize the list we are copying to
    list_init(l2);

    list_iterator_start(l1);
    while(list_iterator_hasnext(l1))
    {
        ct_next = (chidb_dbm_cursor_trail_t *)list_iterator_next(l1);
        
        // make a deep copy of the element
        ct_next_cpy = malloc(sizeof(chidb_dbm_cursor_t));
        if(ct_next_cpy == NULL)
            return CHIDB_ENOMEM;

        ct_next_cpy->depth = ct_next->depth;
        chidb_Btree_getNodeByPage(bt, ct_next->btn->page->npage, &btncpy); 
        ct_next_cpy->btn = btncpy;
        ct_next_cpy->n_current_cell = ct_next->n_current_cell;

        // append the new element to the new list
        list_append(l2, ct_next_cpy);
    }
    list_iterator_stop(l1);

    return CHIDB_OK;
}

/* Clears all elements NOT including depth
 *
 * If you want to clear the node too, just destroy the cursor and make a new one
 * Return
 * - CHIDB_OK: Operation sucessful
 */
int chidb_dbm_cursor_clear_trail_from(BTree *bt, chidb_dbm_cursor_t *c, uint32_t depth)
{
    int i;
    chidb_dbm_cursor_trail_t *ct;

    for(i = list_size(&(c->trail))-1; i > depth; i--)
    {
        ct = list_extract_at(&(c->trail), i); // remove element from list

        // free resources associated with the trail element
        chidb_Btree_freeMemNode(bt, ct->btn);
        free(ct);
    }
    return CHIDB_OK;
}

/* Clears the specific element at specified position
 *
 * Return
 * - CHIDB_OK: Operation sucessful
 */
int chidb_dbm_cursor_trail_remove_at(BTree *bt, chidb_dbm_cursor_t *c, uint32_t depth)
{
    chidb_dbm_cursor_trail_t *ct = list_extract_at(&(c->trail), depth);
    chidb_Btree_freeMemNode(bt, ct->btn);
    free(ct);

    return CHIDB_OK;
}

/* Create a new cursor.
 *
 * Return
 * - CHIDB_OK: Operation sucessful
 * - CHIDB_ENOMEM: Malloc failed
 */
int chidb_dbm_cursor_init(BTree *bt, chidb_dbm_cursor_t *c, npage_t root_page, ncol_t n_cols)
{
    int rc;
    BTreeNode *btn;
    chidb_dbm_cursor_trail_t *ct;

    // allocate space for the root btn in the trail
    ct = malloc(sizeof(chidb_dbm_cursor_trail_t));
    if(ct == NULL)
        return CHIDB_ENOMEM;

    // load up the root btree node
    if((rc = chidb_Btree_getNodeByPage(bt, root_page, &btn)) != CHIDB_OK)
        return rc;
    /*
     * WHY DOES THE ABOVE GET NODE GIVE A LEAK??????
     */

    // populate the cursor trail bit
    ct->depth = 0;
    ct->btn = btn;
    ct->n_current_cell = 0;

    // initialize stuff for cursor
    list_init(&(c->trail));
    
    // populate cursor 
    c->root_page = root_page;
    c->root_type = btn->type;
    c->n_cols = n_cols;
    list_insert_at(&(c->trail), ct, ct->depth); 

    return CHIDB_OK;
}

/* Free a cursor.
 *
 * Return
 * - CHIDB_OK: Operation sucessful
 */
int chidb_dbm_cursor_destroy(BTree *bt, chidb_dbm_cursor_t *c)
{
    // free all of the btn's held within the cursor trail structs
    chidb_dbm_cursor_trail_list_destroy(bt, &(c->trail));

    // free(c);

    return CHIDB_OK;
}

/* Wrapper for the index and table versions of forward
 *
 * Branches on index or table to call proper fwd functions
 * Return
 * - CHIDB_OK: Operation sucessful
 * - CHIDB_ENOMEM: Malloc failed
 */
int chidb_dbm_cursor_fwd(BTree *bt, chidb_dbm_cursor_t *c)
{
    uint32_t list_loc = list_size(&(c->trail)) - 1;
    chidb_dbm_cursor_trail_t *ct = list_get_at(&(c->trail), list_loc);
    uint8_t node_type = ct->btn->type;
    int ret = CHIDB_OK; // to quiet compiler warnings
    
    list_t trail_copy;
    chidb_dbm_cursor_trail_cpy(bt, &(c->trail), &trail_copy);

    switch(node_type)
    {
        case PGTYPE_TABLE_INTERNAL:
        case PGTYPE_TABLE_LEAF:
            ret = chidb_dbm_cursorTable_fwd(bt, c);
            break;

        case PGTYPE_INDEX_INTERNAL:
        case PGTYPE_INDEX_LEAF:
            ret = chidb_dbm_cursorIndex_fwd(bt, c);
            break;

        default:
            ret = CHIDB_ETYPE;
            break;
    }

    if(ret == CHIDB_CURSORCANTMOVE)
    {
        // we're currently at the root with the trail, so now put the copy of the trail
        // back into the cursor
        c->trail = trail_copy;
    }
    else
    {
        // we were able to move, so we didn't need the copy of the trail. destroy it
        chidb_dbm_cursor_trail_list_destroy(bt, &trail_copy);
    }

    return ret;
}

/* Outer most shell for forward on a table.
 *
 * If there is a next cell to move to, advance the cell number and get the new cell.
 * Otherwise, use the cursor trail to go up the tree to check for the next cell.
 * Will only ever hit one peak where you have to then go down.
 * Will always be called on a leaf, because that is where the cursor sits
 * Return
 * - CHIDB_OK: Operation sucessful
 * - CHIDB_CURSORCANTMOVE: At rightmost edge of tree, cannot advance cursor more
 */
int chidb_dbm_cursorTable_fwd(BTree *bt, chidb_dbm_cursor_t *c)
{
    uint32_t list_loc = list_size(&(c->trail)) - 1;
    chidb_dbm_cursor_trail_t *ct = list_get_at(&(c->trail), list_loc);

    if(c->current_cell.type != PGTYPE_TABLE_LEAF)
        return CHIDB_ETYPE;

    if(ct->n_current_cell == ct->btn->n_cells - 1) // we're at the last cell in the leaf
    {
        // remove the old portion of the trail, we're going up
        chidb_dbm_cursor_trail_remove_at(bt, c, list_loc);

        // going up
        return chidb_dbm_cursorTable_fwdUp(bt, c); 
    }
    else // we can just move to next cell
    {
        ct->n_current_cell++;
        chidb_Btree_getCell(ct->btn, ct->n_current_cell, &(c->current_cell));

        return CHIDB_OK;
    }

    return CHIDB_OK;
}

/* Ascend one level in a table tree when trying to advance cursor.
 *
 * If there are more cells in the page, go down that cells child page.
 * The down function will take care of the case of the right page.
 * Otherwise, recursively go up.
 * Return
 * - CHIDB_OK: Operation sucessful
 * - CHIDB_CURSORCANTMOVE: At rightmost edge of tree, cannot advance cursor more
 * - CHIDB_ETYPE: Internal misuse of the function (mostly for debugging purposes)
 */
int chidb_dbm_cursorTable_fwdUp(BTree *bt, chidb_dbm_cursor_t *c)
{
    uint32_t list_loc = list_size(&(c->trail)) - 1;

    //In the case of a root, when trying to go up we have already removed our past trail
    //this means the trail length is actually negative
    if(list_loc == -1)
    {
        return CHIDB_CURSORCANTMOVE;
    }

    chidb_dbm_cursor_trail_t *ct = list_get_at(&(c->trail), list_loc);

    //Up can never be called on a leaf, because we can't go up into a leaf
    if(ct->btn->type != PGTYPE_TABLE_INTERNAL)
    {
        return CHIDB_ETYPE;
    }

    ct->n_current_cell++; // advance the current cell as if we are going down to next one

    if(ct->n_current_cell <= ct->btn->n_cells)
    {
        // go down the current cell or right page
        return chidb_dbm_cursorTable_fwdDwn(bt,c);
    }
    else
    {
        // remove the old portion of the trail, we're going up
        chidb_dbm_cursor_trail_remove_at(bt, c, list_loc);

        // going up
        return chidb_dbm_cursorTable_fwdUp(bt, c); 
    }

    return CHIDB_OK;
}

/* Descend one level in a table tree when trying to advance cursor.
 *
 * Keep recursively going down and adding new parts to the trail. 
 * When we hit the leaf, add the cell to the cursor.
 * Return
 * - CHIDB_OK: Operation sucessful
 * - CHIDB_CURSORCANTMOVE: At rightmost edge of tree, cannot advance cursor more
 * - CHIDB_ECELLNO: Internal misuse of the function (mostly for debugging purposes)
 * - CHIDB_ETYPE: Internal misuse of the function (mostly for debugging purposes)
 */
int chidb_dbm_cursorTable_fwdDwn(BTree *bt, chidb_dbm_cursor_t *c)
{
    uint32_t list_loc = list_size(&(c->trail)) - 1;
    chidb_dbm_cursor_trail_t *ct = list_get_at(&(c->trail), list_loc);
    uint8_t node_type = ct->btn->type;

    npage_t pg;
    switch(node_type)
    {
        case PGTYPE_TABLE_INTERNAL:
            if(ct->n_current_cell < ct->btn->n_cells)
            {
                // we need to make the new part of trail on the cell num
                BTreeCell cell;
                chidb_Btree_getCell(ct->btn, ct->n_current_cell, &cell);
                pg = cell.fields.tableInternal.child_page;
            }
            else if(ct->n_current_cell == ct->btn->n_cells)
            {
                // trail on right page
                pg = ct->btn->right_page;
            }
            else
                return CHIDB_ECELLNO;
            
            // build new cursor trail instance for next level down
            chidb_dbm_cursor_trail_t *ct_new;
            uint32_t next_depth = list_loc + 1;
            chidb_dbm_cursor_trail_new(bt, &ct_new, pg, next_depth); // adding to the NEXT position

            // add the new thing to the trail
            list_insert_at(&(c->trail), ct_new, next_depth);
            
            // call fwdDwn again
            return chidb_dbm_cursorTable_fwdDwn(bt, c);

        case PGTYPE_TABLE_LEAF:
            // get the cell and put it in the cursor
            // n_current_cell is initialized to cell zero
            chidb_Btree_getCell(ct->btn, ct->n_current_cell, &(c->current_cell));
            // update cursor fields

            return CHIDB_OK;
        default:
            return CHIDB_ETYPE;
    }

    return CHIDB_OK;
}

//Cursors in indexes can rest either at internal OR leaves
/* Outer most shell for forward on an index.
 *
 * Cursors in indexes can rest either at internal OR leaves
 * Return
 * - CHIDB_OK: Operation sucessful
 * - CHIDB_CURSORCANTMOVE: At rightmost edge of tree, cannot advance cursor more
 */
int chidb_dbm_cursorIndex_fwd(BTree *bt, chidb_dbm_cursor_t *c)
{
    uint32_t list_loc = list_size(&(c->trail)) - 1;
    chidb_dbm_cursor_trail_t *ct = list_get_at(&(c->trail), list_loc);
    uint8_t node_type = ct->btn->type;

    switch(node_type)
    {
    	case PGTYPE_INDEX_LEAF:
    		//check if we can go to the next cell
            if(ct->n_current_cell == ct->btn->n_cells - 1) // we're at the last cell in the leaf
            {
                // remove the old portion of the trail, we're going up
                chidb_dbm_cursor_trail_remove_at(bt, c, list_loc);

                // going up
                return chidb_dbm_cursorIndex_fwdUp(bt, c); 
            }
            else // we can just move to next cell
            {
                ct->n_current_cell++;
                chidb_Btree_getCell(ct->btn, ct->n_current_cell, &(c->current_cell));

                return CHIDB_OK;
            }
    	case PGTYPE_INDEX_INTERNAL:
    		//at this point we always need to go down the next cell's child page
            //first though, need to check if there is another cell to go to
            ct->n_current_cell++;
            if(ct->n_current_cell <= ct->btn->n_cells)
            {
                // go down the current cell or right page
                return chidb_dbm_cursorIndex_fwdDwn(bt,c);
            }
            else //we've increased over the possible number of cells
            {
                // remove the old portion of the trail, we're going up
                chidb_dbm_cursor_trail_remove_at(bt, c, list_loc);

                // going up
                return chidb_dbm_cursorIndex_fwdUp(bt, c);
            }
    }

    return CHIDB_OK;
}

/* Ascend one level in an index tree when trying to advance cursor.
 *
 * Return
 * - CHIDB_OK: Operation sucessful
 * - CHIDB_CURSORCANTMOVE: At rightmost edge of tree, cannot advance cursor more
 */
int chidb_dbm_cursorIndex_fwdUp(BTree *bt, chidb_dbm_cursor_t *c)
{
    uint32_t list_loc = list_size(&(c->trail)) - 1;

    //In the case of a root, when trying to go up we have already removed our past trail
    //this means the trail length is actually negative
    if(list_loc == -1)
    {
        return CHIDB_CURSORCANTMOVE;
    }

    chidb_dbm_cursor_trail_t *ct = list_get_at(&(c->trail), list_loc);

    ct->n_current_cell++; // advance the current cell as if we are going down to next one

    if(ct->n_current_cell < ct->btn->n_cells)
    {
        //since this is an index, and you are looking for the next biggest value, we need to stop here
        //at the internal's next cell because it holds the key value pair that is greater than the leaf
        //we just came out of
        BTreeCell cell;
        chidb_Btree_getCell(ct->btn, ct->n_current_cell, &cell);

        return CHIDB_OK;
    }
    else if(ct->n_current_cell == ct->btn->n_cells)
    {
        //need to go down right page
        return chidb_dbm_cursorIndex_fwdDwn(bt,c);
    }
    else //we've already explored the right page and are out of cells to go down
    {
        // remove the old portion of the trail, we're going up
        chidb_dbm_cursor_trail_remove_at(bt, c, list_loc);

        // going up
        return chidb_dbm_cursorIndex_fwdUp(bt, c); 
    }

    return CHIDB_OK;
}

/* Descend one level in an index tree when trying to advance cursor.
 *
 * Return
 * - CHIDB_OK: Operation sucessful
 * - CHIDB_CURSORCANTMOVE: At rightmost edge of tree, cannot advance cursor more
 * - CHIDB_ECELLNO: Internal misuse of the function (mostly for debugging purposes)
 * - CHIDB_ETYPE: Internal misuse of the function (mostly for debugging purposes)
 */
int chidb_dbm_cursorIndex_fwdDwn(BTree *bt, chidb_dbm_cursor_t *c)
{
    uint32_t list_loc = list_size(&(c->trail)) - 1;
    chidb_dbm_cursor_trail_t *ct = list_get_at(&(c->trail), list_loc);
    uint8_t node_type = ct->btn->type;

    npage_t pg;
    switch(node_type)
    {
        case PGTYPE_INDEX_INTERNAL:
            if(ct->n_current_cell < ct->btn->n_cells)
            {
                // we need to make the new part of trail on the cell num
                BTreeCell cell;
                chidb_Btree_getCell(ct->btn, ct->n_current_cell, &cell);
                pg = cell.fields.indexInternal.child_page;
            }
            else if(ct->n_current_cell == ct->btn->n_cells)
            {
                // trail on right page
                pg = ct->btn->right_page;
            }
            else
                return CHIDB_ECELLNO;
            
            // build new cursor trail instance for next level down
            chidb_dbm_cursor_trail_t *ct_new;
            uint32_t next_depth = list_loc + 1;
            chidb_dbm_cursor_trail_new(bt, &ct_new, pg, next_depth); // adding to the NEXT position

            // add the new thing to the trail
            list_insert_at(&(c->trail), ct_new, next_depth);
            
            // call fwdDwn again
            return chidb_dbm_cursorIndex_fwdDwn(bt, c);

        case PGTYPE_INDEX_LEAF:
            // get the cell and put it in the cursor
            chidb_Btree_getCell(ct->btn, ct->n_current_cell, &(c->current_cell));
            // update cursor fields
            return CHIDB_OK;
        default:
            return CHIDB_ETYPE;
    }

    return CHIDB_OK;
}

/* Wrapper function for cursorTable_rev and cursor_Index_rev
 *
 * returns
 * - CHIDB_OK
 * - CHIDB_ETYPE: Invalid page type in switch statement
 */
int chidb_dbm_cursor_rev(BTree *bt, chidb_dbm_cursor_t *c)
{
    uint32_t list_loc = list_size(&(c->trail)) - 1;
    chidb_dbm_cursor_trail_t *ct = list_get_at(&(c->trail), list_loc);

    uint8_t node_type = ct->btn->type;
    int ret = CHIDB_OK;
    switch(node_type)
    {
        case PGTYPE_TABLE_INTERNAL:
        case PGTYPE_TABLE_LEAF:
            ret = chidb_dbm_cursorTable_rev(bt, c);
            break;

        case PGTYPE_INDEX_INTERNAL:
        case PGTYPE_INDEX_LEAF:
            ret = chidb_dbm_cursorIndex_rev(bt, c);
            break;

        default:
            ret = CHIDB_ETYPE;
            break;
    }
    return ret;
}


/* Moves the cursor one step backwards on the table entry, which corresponds to getting the previous cell.
 * If we can shift backwards, get previous cell at leaf, else we need to traverse up the BTree
 * and call cursorTable_revUp to get cells in previous leaf.
 * 
 * returns
 * - CHIDB_OK
 * - CHIDB_ETYPE: Invalid page type, a.k.a cursor is not pointing at a leaf
 * - cursorTable_revUp return codes
 */
int chidb_dbm_cursorTable_rev(BTree *bt, chidb_dbm_cursor_t *c)
{
    uint32_t list_loc = list_size(&(c->trail)) - 1;
    chidb_dbm_cursor_trail_t *ct = list_get_at(&(c->trail), list_loc);

    if(c->current_cell.type != PGTYPE_TABLE_LEAF)
        return CHIDB_ETYPE;

    if(ct->n_current_cell == 0) //we're at the first cell in the leaf
    {
        // remove the old portion of the trail, we're going up
        chidb_dbm_cursor_trail_remove_at(bt, c, list_loc);

        // going up
        return chidb_dbm_cursorTable_revUp(bt, c); 
    }
    else // there are cells behind us we can move into
    {
        ct->n_current_cell--;
        chidb_Btree_getCell(ct->btn, ct->n_current_cell, &(c->current_cell));

        return CHIDB_OK;
    }
   
    return CHIDB_OK;
}

/* Helper function for cursorTable_rev. If the cursor is currently pointing at the
 * first cell at a current leaf, this function traverses up the BTree up by one, goes down
 * the BTree to the appropiate leaf and points the cursor to the last cell of that leaf
 *
 * returns
 * - CHIDB_OK
 * - CHIDB_ETYPE: Invalid btn type. We expect to go up to an internal table, not leaf
 * - CHIDB_CURSORCANTMOVE: We cannot go backwards further
 */
int chidb_dbm_cursorTable_revUp(BTree *bt, chidb_dbm_cursor_t *c)
{
    uint32_t list_loc = list_size(&(c->trail)) - 1;

    if(list_loc == -1)
    {
        return CHIDB_CURSORCANTMOVE;
    }

    chidb_dbm_cursor_trail_t *ct = list_get_at(&(c->trail), list_loc);

    //Sanity check. we can't have gone up to another leaf, so...
    if(ct->btn->type != PGTYPE_TABLE_INTERNAL)
    {
        return CHIDB_ETYPE;
    }

    ct->n_current_cell--; // advance the current cell as if we are going down to next one

    if(ct->n_current_cell >= 0)
    {
        // we can go down the current cell or right page
        return chidb_dbm_cursorTable_revDwn(bt,c);   
    }
    else
    {
        // remove the old portion of the trail, we're going up
        chidb_dbm_cursor_trail_remove_at(bt, c, list_loc);

        // we need to go up again, backed up the farthest we can here
        return chidb_dbm_cursorTable_revUp(bt,c);
    }

    return CHIDB_OK;
}

/* Helper function to chidb_dbm_cursorTable_rev
 *
 * Determines where to place the cursor once _rev or recursively _revDwn
 * has decided it is time to walk down the B-Tree. 
 *
 * Eventually, it gets the cell and stores it in the cursor once
 * the desired destination in some leaf page is reached.
 */
int chidb_dbm_cursorTable_revDwn(BTree *bt, chidb_dbm_cursor_t *c)
{
    uint32_t list_loc = list_size(&(c->trail)) - 1;
    chidb_dbm_cursor_trail_t *ct = list_get_at(&(c->trail), list_loc);
    uint8_t node_type = ct->btn->type;

    npage_t pg;
    switch(node_type)
    {
        //if it is internal, then we will definitely need to go further down to reach more leaves
        case PGTYPE_TABLE_INTERNAL:
            if(ct->n_current_cell < ct->btn->n_cells)
            {
                BTreeCell cell;
                chidb_Btree_getCell(ct->btn, ct->n_current_cell, &cell);
                pg = cell.fields.tableInternal.child_page;
            }
            else if(ct->n_current_cell == ct->btn->n_cells)
            {
                // trail on right page
                pg = ct->btn->right_page;
            }
            else
                return CHIDB_ECELLNO;

            // build new cursor trail instance for next level down
            chidb_dbm_cursor_trail_t *ct_new;
            uint32_t next_depth = list_loc + 1;
            chidb_dbm_cursor_trail_new(bt, &ct_new, pg, next_depth);

            //since we are going backwards, need to manually set the new cell as the maximum
            ct_new->n_current_cell = ct_new->btn->n_cells; 
            
            // if the new trail instance holds a leaf btn, it doesn't have a right page so
            // its max current cell is n_cells-1.
            if(ct_new->btn->type == PGTYPE_TABLE_LEAF)
                ct_new->n_current_cell--;

            // add the new thing to the trail
            list_insert_at(&(c->trail), ct_new, next_depth);

            // finally able to go down now that trail has been constructed
            return chidb_dbm_cursorTable_revDwn(bt, c);

        case PGTYPE_TABLE_LEAF:
            // get the cell and put it in the cursor
            chidb_Btree_getCell(ct->btn, ct->n_current_cell, &(c->current_cell));

            return CHIDB_OK;

        default:
            return CHIDB_ETYPE;
    }

    return CHIDB_OK;
}

/* Moves the cursor to the previous entry in the index BTree
 *
 * This function is used to navigate entries one by one as if they
 * were all in one linear array. This determines if extra work needs 
 * to be done to get to the previous entry (because we have run out of
 * cells on the current page loaded into memory)
 * 
 * Otherwise, it goes and grabs the information from the next cell
 */
int chidb_dbm_cursorIndex_rev(BTree *bt, chidb_dbm_cursor_t *c)
{
    uint32_t list_loc = list_size(&(c->trail)) - 1;
    chidb_dbm_cursor_trail_t *ct = list_get_at(&(c->trail), list_loc);
    uint8_t node_type = ct->btn->type;

    switch(node_type)
    {
        case PGTYPE_INDEX_LEAF:
            //check if we can go to the next cell
            if(ct->n_current_cell == 0) // we're at the first cell in the leaf
            {
                // remove the old portion of the trail, we're going up
                chidb_dbm_cursor_trail_remove_at(bt, c, list_loc);

                // going up
                return chidb_dbm_cursorIndex_revUp(bt, c); 
            }
            else // there are cells behind us we can move into
            {
                ct->n_current_cell--;
                chidb_Btree_getCell(ct->btn, ct->n_current_cell, &(c->current_cell));

                return CHIDB_OK;
            }
        case PGTYPE_INDEX_INTERNAL:
            //at this point we always need to go down the ~current~ cell's child page
            if(ct->n_current_cell >= 0)
            {
                // go down the current cell or right page
                return chidb_dbm_cursorIndex_revDwn(bt,c);
            }
            else //we've decreased over the possible number of cells
            {
                // remove the old portion of the trail, we're going up
                chidb_dbm_cursor_trail_remove_at(bt, c, list_loc);

                // going up
                return chidb_dbm_cursorIndex_revUp(bt, c);
            }
    }

    return CHIDB_OK;
}

/* Helper function to chidb_dbm_cursorIndex_rev
 *
 * Only ever called by _rev and recursively by _revUp.
 * 
 * Once we've decided in either _rev or _revUp that we need to go back 
 * up the tree structure in order to reach the next entry.
 * 
 * This will stop the cursor at an internal node and get the cell.
 */
int chidb_dbm_cursorIndex_revUp(BTree *bt, chidb_dbm_cursor_t *c)
{
    uint32_t list_loc = list_size(&(c->trail)) - 1;

    //In the case of a root, when trying to go up we have already removed our past trail
    //this means the trail length is actually negative
    if(list_loc == -1)
    {
        return CHIDB_CURSORCANTMOVE;
    }

    chidb_dbm_cursor_trail_t *ct = list_get_at(&(c->trail), list_loc);

    ct->n_current_cell--; // decrease the current cell as if we are going down to prev one

    if(ct->n_current_cell >= 0)
    {
        //since this is an index, and you are looking for the next smallest value, we need to stop here
        //at the internal's next cell because it holds the key value pair that is less than the child
        //we just came out of
        BTreeCell cell;
        chidb_Btree_getCell(ct->btn, ct->n_current_cell, &cell);

        return CHIDB_OK;
    }
    else //we've already explored the left most child and we are out of cells to go down
    {
        // remove the old portion of the trail, we're going up
        chidb_dbm_cursor_trail_remove_at(bt, c, list_loc);

        // going up
        return chidb_dbm_cursorIndex_fwdUp(bt, c); 
    }

    return CHIDB_OK;
}

/* Helper function to chidb_dbm_cursorIndex_rev
 *
 * Determines where to place the cursor once _rev or recursively _revDwn
 * has decided it is time to walk down the B-Tree. 
 *
 * Eventually, it gets the cell and stores it in the cursor once
 * the desired destination is reached.
 */
int chidb_dbm_cursorIndex_revDwn(BTree *bt, chidb_dbm_cursor_t *c)
{
    uint32_t list_loc = list_size(&(c->trail)) - 1;
    chidb_dbm_cursor_trail_t *ct = list_get_at(&(c->trail), list_loc);
    uint8_t node_type = ct->btn->type;

    npage_t pg;
    switch(node_type)
    {
        case PGTYPE_INDEX_INTERNAL:
            if(ct->n_current_cell >= 0)
            {
                // we need to make the new part of trail on the cell num
                BTreeCell cell;
                chidb_Btree_getCell(ct->btn, ct->n_current_cell, &cell);
                pg = cell.fields.indexInternal.child_page;
            }
            else if(ct->n_current_cell == ct->btn->n_cells)
            {
                // trail on right page
                pg = ct->btn->right_page;
            }
            else
                return CHIDB_ECELLNO;
            
            // build new cursor trail instance for next level down
            chidb_dbm_cursor_trail_t *ct_new;
            uint32_t next_depth = list_loc + 1;
            chidb_dbm_cursor_trail_new(bt, &ct_new, pg, next_depth); 

            //since we are going backwards, need to manually set the new cell as the maximum
            ct_new->n_current_cell = ct_new->btn->n_cells; 
            
            // if the new trail instance holds a leaf btn, it doesn't have a right page so
            // its max current cell is n_cells-1.
            if(ct_new->btn->type == PGTYPE_TABLE_LEAF)
                ct_new->n_current_cell--;

            // add the new thing to the trail
            list_insert_at(&(c->trail), ct_new, next_depth);
            
            // call fwdDwn again
            return chidb_dbm_cursorIndex_fwdDwn(bt, c);

        case PGTYPE_INDEX_LEAF:
            // get the cell and put it in the cursor
            chidb_Btree_getCell(ct->btn, ct->n_current_cell, &(c->current_cell));

            return CHIDB_OK;

        default:
            return CHIDB_ETYPE;
    }

    return CHIDB_OK;
}

/* seek bt c key next depth seek_type
 * bt:        our full B-tree for searching
 * c:         our cursor for cursing
 * key:       key we are searching for
 * depth:     the current depth of the trail (how many items in the trail -1)
 * seek_type: can be any of the following: SEEK, SEEKLT, SEEKGT, SEEKLE, SEEKGE
 *              I hope you can figure out what those stand for
 */
int chidb_dbm_cursor_seek(BTree *bt, chidb_dbm_cursor_t *c, chidb_key_t key, npage_t next, int depth, int seek_type)
{
    chidb_dbm_cursor_trail_t *trail_entry;

    if (!depth)
    {
        chidb_dbm_cursor_clear_trail_from(bt, c, 0);
        next = c->root_page; // jic you didn't call it right
        trail_entry = list_get_at(&(c->trail), 0);
    }
    else
    {
        trail_entry = malloc(sizeof(chidb_dbm_cursor_trail_t));
    }
    
    BTreeCell cell;
    BTreeNode *btn;
    

    int status;
    int i = 0;

    if ((status = chidb_Btree_getNodeByPage(bt, next, &btn)) != CHIDB_OK)
    {
        free(btn);
        return status;
    }

    // Same info no matter what cell we choose. Just need cell # and to append
    trail_entry->depth = depth;
    trail_entry->btn = btn;

    if (btn->type == PGTYPE_TABLE_LEAF)
    {
        do 
        {
            if (chidb_Btree_getCell(btn, i, &cell) != CHIDB_OK)
                return CHIDB_ECELLNO;

            if (cell.key == key)
            {
                trail_entry->n_current_cell = i;
                c->current_cell = cell;
                if (depth)
                    list_append(&c->trail, trail_entry);
                
                if (seek_type == SEEKLT)
                    return chidb_dbm_cursor_rev(bt, c);

                else if (seek_type == SEEKGT)
                    return chidb_dbm_cursor_fwd(bt, c);

                return CHIDB_OK;
            }
            else if (cell.key > key)
            {
                // technically not correct for if it's seek, but doesn't matter...
                trail_entry->n_current_cell = i;
                c->current_cell = cell;
                if (depth)
                    list_append(&c->trail, trail_entry);

                if (seek_type == SEEK)
                    return CHIDB_ENOTFOUND;

                else if (seek_type == SEEKLT || seek_type == SEEKLE)
                    return chidb_dbm_cursor_rev(bt, c);

                return CHIDB_OK;
            }
            else if (cell.key < key)
            {
                i++;
            }

            // i++;
        } while (i < btn->n_cells);

        return CHIDB_CURSORCANTMOVE;
    }

    else if (btn->type == PGTYPE_TABLE_INTERNAL)
    {
        do
        {
            if (chidb_Btree_getCell(btn, i, &cell) != CHIDB_OK)
                return CHIDB_ECELLNO;

            if ((cell.key >= key))
            {
                // add the node to the cursor's list
                trail_entry->n_current_cell = i;
                c->current_cell = cell;
                if (depth)
                    list_append(&c->trail, trail_entry);

                if ((status = chidb_dbm_cursor_seek(bt, c, key, 
                    cell.fields.tableInternal.child_page, depth+1, seek_type)) != CHIDB_OK)
                    return status;
                return CHIDB_OK;
            }
            else
            {
                i++;
            }

        } while ((cell.key < key) && (i < btn->n_cells));


        trail_entry->n_current_cell = btn->n_cells;
        c->current_cell = cell;
        if (depth)
            list_append(&c->trail, trail_entry);

        return chidb_dbm_cursor_seek(bt, c, key, btn->right_page, depth+1, seek_type);
    }

    else
    {
        do 
        {
            if (chidb_Btree_getCell(btn, i, &cell) != CHIDB_OK)
                return CHIDB_ECELLNO;

            if (cell.key == key) // WE FOUND A THING
            {
                trail_entry->n_current_cell = i;
                c->current_cell = cell;
                if (depth)
                    list_append(&c->trail, trail_entry);

                if (seek_type == SEEKLT)
                    return chidb_dbm_cursor_rev(bt, c);
                else if (seek_type == SEEKGT)
                    return chidb_dbm_cursor_fwd(bt, c);

                return CHIDB_OK;
            }
            else if (cell.key > key)
            {
                trail_entry->n_current_cell = i;
                c->current_cell = cell;
                if (depth)
                    list_append(&c->trail, trail_entry);

                if (btn->type == PGTYPE_INDEX_INTERNAL)
                    return chidb_dbm_cursor_seek(bt, c, key, 
                        cell.fields.indexInternal.child_page, depth+1, seek_type);
                else
                {
                    if (seek_type == SEEK)
                        return CHIDB_ENOTFOUND;

                    else if (seek_type == SEEKLT || seek_type == SEEKLE)
                        return chidb_dbm_cursor_rev(bt, c);
                    
                    return CHIDB_OK;
                }
            }

            i++;
        } while (i < btn->n_cells);

        if (btn->type == PGTYPE_INDEX_INTERNAL)
        {
            trail_entry->n_current_cell = btn->n_cells;
            c->current_cell = cell;
            if (depth)
                list_append(&c->trail, trail_entry);

            return chidb_dbm_cursor_seek(bt, c, key, btn->right_page, depth+1, seek_type);
        }

        else if (btn->type == PGTYPE_INDEX_LEAF)
            return CHIDB_CURSORCANTMOVE;
    }

    return CHIDB_OK; // this should never be reached!
}
