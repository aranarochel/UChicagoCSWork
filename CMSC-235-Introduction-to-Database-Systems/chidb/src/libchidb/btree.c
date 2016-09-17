/*
 *  chidb - a didactic relational database management system
 *
 * This module contains functions to manipulate a B-Tree file. In this context,
 * "BTree" refers not to a single B-Tree but to a "file of B-Trees" ("chidb
 * file" and "file of B-Trees" are essentially equivalent terms).
 *
 * However, this module does *not* read or write to the database file directly.
 * All read/write operations must be done through the pager module.
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

/* THIS VERSION OF BTREE.C BELONGS TO THE BIZZLE_TRIZZLE GROUP */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <chidb/log.h>
#include "chidbInt.h"
#include "btree.h"
#include "record.h"
#include "pager.h"
#include "util.h"

#include <sys/stat.h>

/* Open a B-Tree file
 *
 * This function opens a database file and verifies that the file
 * header is correct. If the file is empty (which will happen
 * if the pager is given a filename for a file that does not exist)
 * then this function will (1) initialize the file header using
 * the default page size and (2) create an empty table leaf node
 * in page 1.
 *
 * Parameters
 * - filename: Database file (might not exist)
 * - db: A chidb struct. Its bt field must be set to the newly
 *       created BTree.
 * - bt: An out parameter. Used to return a pointer to the
 *       newly created BTree.
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_ECORRUPTHEADER: Database file contains an invalid header
 * - CHIDB_ENOMEM: Could not allocate memory
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */
int chidb_Btree_open(const char *filename, chidb *db, BTree **bt)
{
    // fprintf(stderr, "IN OPEN\n");

    int status;
    Pager *pager;
    struct stat buf;
    npage_t npage;
    uint8_t pos[100];
    uint16_t page_size;

    uint8_t h12[] = {0x01,0x01,0x00,0x40,0x20,0x20};
    uint8_t h0[] = {0,0,0,0};
    uint8_t h1[] = {0,0,0,1};

    if ((status = chidb_Pager_open(&pager, filename)) != CHIDB_OK) {
        return status;
    }

    *bt = (BTree *) malloc(sizeof(BTree));

    if (!(*bt)) {
        return CHIDB_ENOMEM;
    }

    (*bt)->pager = pager;
    (*bt)->db = db;
    db->bt = *bt;

    fstat(fileno(pager->f), &buf);

    if (!buf.st_size) {
        //Newly created file
        //Need to initialize

        chidb_Pager_setPageSize(pager, DEFAULT_PAGE_SIZE);
        pager->n_pages = 0;

        if ((status = chidb_Btree_newNode(*bt, &npage, PGTYPE_TABLE_LEAF)) != CHIDB_OK) {
            return status;
        }

    } else {
        //File exists
        //Verify Header

        if ((status = chidb_Pager_readHeader(pager, pos)) != CHIDB_OK) {
            return status;
        }

        if (!memcmp(pos, "SQLite format 3", 16) &&
            !memcmp(&pos[0x12], h12, 6) &&
            !memcmp(&pos[0x20], h0, 4) &&
            !memcmp(&pos[0x24], h0, 4) &&
            !memcmp(&pos[0x2c], h1, 4) &&
            !memcmp(&pos[0x34], h0, 4) &&
            !memcmp(&pos[0x38], h1, 4) &&
            !memcmp(&pos[0x40], h0, 4) &&
            (get4byte(&pos[0x30]) == 20000)) {
            //Header is accurate
            //Read out page size

            page_size = get2byte(&pos[0x10]);
            chidb_Pager_setPageSize(pager, page_size);

        } else {
            return CHIDB_ECORRUPTHEADER;
        }

    }



    return CHIDB_OK;
}


/* Close a B-Tree file
 *
 * This function closes a database file, freeing any resource
 * used in memory, such as the pager.
 *
 * Parameters
 * - bt: B-Tree file to close
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */
int chidb_Btree_close(BTree *bt)
{
    // fprintf(stderr, "IN CLOSE\n");
    chidb_Pager_close(bt->pager);
    free(bt);

    return CHIDB_OK;
}


/* Loads a B-Tree node from disk
 *
 * Reads a B-Tree node from a page in the disk. All the information regarding
 * the node is stored in a BTreeNode struct (see header file for more details
 * on this struct). *This is the only function that can allocate memory for
 * a BTreeNode struct*. Always use chidb_Btree_freeMemNode to free the memory
 * allocated for a BTreeNode (do not use free() directly on a BTreeNode variable)
 * Any changes made to a BTreeNode variable will not be effective in the database
 * until chidb_Btree_writeNode is called on that BTreeNode.
 *
 * Parameters
 * - bt: B-Tree file
 * - npage: Page of node to load
 * - btn: Out parameter. Used to return a pointer to newly creater BTreeNode
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_EPAGENO: The provided page number is not valid
 * - CHIDB_ENOMEM: Could not allocate memory
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */
int chidb_Btree_getNodeByPage(BTree *bt, npage_t npage, BTreeNode **btn)
{

    int status;
    uint8_t *dat;

    if (!(*btn = (BTreeNode *) malloc(sizeof(BTreeNode)))) {
        return CHIDB_ENOMEM;
    }

    if ((status = chidb_Pager_readPage(bt->pager, npage, &(*btn)->page)) != CHIDB_OK) {
        return status;
    }

    dat = (*btn)->page->data + ((npage == 1) ? 100 : 0);

    (*btn)->type = *dat;
    (*btn)->free_offset = get2byte(dat + 1);
    (*btn)->n_cells = get2byte(dat + 3);
    (*btn)->cells_offset = get2byte(dat + 5);
    (*btn)->right_page = (((*btn)->type == 0x05) || ((*btn)->type == 0x02)) ? get4byte(dat+8) : 0;
    (*btn)->celloffset_array = dat + ((((*btn)->type == 0x05) || ((*btn)->type == 0x02)) ? 12 : 8);

    return CHIDB_OK;
}


/* Frees the memory allocated to an in-memory B-Tree node
 *
 * Frees the memory allocated to an in-memory B-Tree node, and
 * the in-memory page returned by the pages (stored in the
 * "page" field of BTreeNode)
 *
 * Parameters
 * - bt: B-Tree file
 * - btn: BTreeNode to free
 *
 * Return
 * - CHIDB_OK: Operation successful
 */
int chidb_Btree_freeMemNode(BTree *bt, BTreeNode *btn)
{
    int status;

    if ((status = chidb_Pager_releaseMemPage(bt->pager, btn->page)) != CHIDB_OK) {
        return status;
    }

    free(btn);

    return CHIDB_OK;
}


/* Create a new B-Tree node
 *
 * Allocates a new page in the file and initializes it as a B-Tree node.
 *
 * Parameters
 * - bt: B-Tree file
 * - npage: Out parameter. Returns the number of the page that
 *          was allocated.
 * - type: Type of B-Tree node (PGTYPE_TABLE_INTERNAL, PGTYPE_TABLE_LEAF,
 *         PGTYPE_INDEX_INTERNAL, or PGTYPE_INDEX_LEAF)
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_ENOMEM: Could not allocate memory
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */
int chidb_Btree_newNode(BTree *bt, npage_t *npage, uint8_t type)
{
    // fprintf(stderr, "IN NEW NODE\n");
    int status = chidb_Pager_allocatePage(bt->pager, npage);

    if (status == CHIDB_OK) {

        status = chidb_Btree_initEmptyNode(bt, *npage, type);

    }

    return status;
}


/* Initialize a B-Tree node
 *
 * Initializes a database page to contain an empty B-Tree node. The
 * database page is assumed to exist and to have been already allocated
 * by the pager.
 *
 * Parameters
 * - bt: B-Tree file
 * - npage: Database page where the node will be created.
 * - type: Type of B-Tree node (PGTYPE_TABLE_INTERNAL, PGTYPE_TABLE_LEAF,
 *         PGTYPE_INDEX_INTERNAL, or PGTYPE_INDEX_LEAF)
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_ENOMEM: Could not allocate memory
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */
int chidb_Btree_initEmptyNode(BTree *bt, npage_t npage, uint8_t type)
{
    // fprintf(stderr, "IN INIT NODE\n");
    MemPage *page;
    uint8_t *pos = NULL;
    int status;

    if (chidb_Pager_readPage(bt->pager, npage, &page) == CHIDB_OK) {

        pos = page->data;

        if (npage == 1) {
            //Write File Header

            //Format string
            sprintf((char *) page->data, "SQLite format 3");
            pos += 16;

            //Write Page Size
            put2byte(pos, bt->pager->page_size);
            pos += 2;

            //Write Hex Garbage (12 -17)
            *(pos++) = 0x01;
            *(pos++) = 0x01;
            *(pos++) = 0x00;
            *(pos++) = 0x40;
            *(pos++) = 0x20;
            *(pos++) = 0x20;

            //Write File Change Counter
            put4byte(pos, 0);
            pos += 8; //4 unused bytes follow

            //Write 8 bytes worth of 0
            put4byte(pos, 0);
            pos += 4;
            put4byte(pos, 0);
            pos += 4;

            //Write Schema version (init to 0)
            put4byte(pos, 0);
            pos += 4;

            //Write Hex Garbage (2C-2F)
            put4byte(pos, 1);
            pos += 4;

            //Write Page Cache Size
            put4byte(pos, 20000);
            pos += 4;

            //Write Hex Garbage (34 -37)
            put4byte(pos, 0);
            pos += 4;

            //Write Hex Garbage (38 - 3B)
            put4byte(pos, 1);
            pos += 4;

            //Write User Cookie
            put4byte(pos, 0);
            pos += 4;

            //Write Hex Garbage (40 - 43)
            put4byte(pos, 0);

            //next 32 bytes are unused 
            pos = page->data + 100;

        }

        //Write Page Header

        //Write Page Type
        *(pos++) = type;


        //Write Free offset 
        //We assume from start of Header
        put2byte(pos, ((type == 0x05 || type == 0x02) ? 12 : 8) + ((npage == 1) ? 100 : 0));
        pos += 2;

        //Write NumCells
        put2byte(pos, 0);
        pos += 2;

        //Write CellsOffset
        put2byte(pos, bt->pager->page_size);
        pos += 2;

        //Write 0
        *(pos++) = 0;

        //Write Right Page
        if (type == 0x05 || type == 0x02) {
            //Node is Internal Node
            put4byte(pos, 0);
            pos += 4;
        }

        if ((status = chidb_Pager_writePage(bt->pager, page)) == CHIDB_OK) {

            status = chidb_Pager_releaseMemPage(bt->pager, page);

        }

        return status;

    } else {

        return CHIDB_ENOMEM;

    }


    return CHIDB_OK;
}



/* Write an in-memory B-Tree node to disk
 *
 * Writes an in-memory B-Tree node to disk. To do this, we need to update
 * the in-memory page according to the chidb page format. Since the cell
 * offset array and the cells themselves are modified directly on the
 * page, the only thing to do is to store the values of "type",
 * "free_offset", "n_cells", "cells_offset" and "right_page" in the
 * in-memory page.
 *
 * Parameters
 * - bt: B-Tree file
 * - btn: BTreeNode to write to disk
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */
int chidb_Btree_writeNode(BTree *bt, BTreeNode *btn)
{
    uint8_t *pos = ((btn->page->npage == 1) ? 100 : 0) + btn->page->data;

    *pos = btn->type;
    put2byte(pos + 1, btn->free_offset);
    put2byte(pos + 3, btn->n_cells);
    put2byte(pos + 5, btn->cells_offset);
    if ((btn->type == 0x05) || (btn->type == 0x02)) {
        put4byte(pos + 8, btn->right_page);
    }

    chidb_Pager_writePage(bt->pager, btn->page);

    return CHIDB_OK;
}


/* Read the contents of a cell
 *
 * Reads the contents of a cell from a BTreeNode and stores them in a BTreeCell.
 * This involves the following:
 *  1. Find out the offset of the requested cell.
 *  2. Read the cell from the in-memory page, and parse its
 *     contents (refer to The chidb File Format document for
 *     the format of cells).
 *
 * Parameters
 * - btn: BTreeNode where cell is contained
 * - ncell: Cell number
 * - cell: BTreeCell where contents must be stored.
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_ECELLNO: The provided cell number is invalid
 */
int chidb_Btree_getCell(BTreeNode *btn, ncell_t ncell, BTreeCell *cell)
{
    uint8_t *dat = btn->page->data;
    uint8_t *curr_cell = dat + get2byte(btn->celloffset_array+ncell*2);

    if(ncell < 0 || ncell > btn->n_cells) {
            return CHIDB_ECELLNO;
    }

    switch(btn->type) {
        case PGTYPE_TABLE_INTERNAL:
            cell->type = PGTYPE_TABLE_INTERNAL;
            cell->fields.tableInternal.child_page = get4byte(curr_cell);
            getVarint32(curr_cell + 4, &cell->key);
            break;
        case PGTYPE_TABLE_LEAF:
            cell->type = PGTYPE_TABLE_LEAF;

            getVarint32(curr_cell, &cell->fields.tableLeaf.data_size);
            getVarint32(curr_cell + 4, &cell->key);
            cell->fields.tableLeaf.data = curr_cell + TABLELEAFCELL_SIZE_WITHOUTDATA;
            break;
        case PGTYPE_INDEX_INTERNAL:
            cell->type = PGTYPE_INDEX_INTERNAL;
            cell->key = get4byte(curr_cell + 8);
            cell->fields.indexInternal.keyPk = get4byte(curr_cell + 12);
            cell->fields.indexInternal.child_page = get4byte(curr_cell);
            break;
        case PGTYPE_INDEX_LEAF:
            cell->type = PGTYPE_INDEX_LEAF;
            cell->key = get4byte(curr_cell + 4);
            cell->fields.indexLeaf.keyPk = get4byte(curr_cell + 8);
            break;
        default:
	    fprintf(stderr,"getCell: invalid page type (%d)\n",btn->type);
	    exit(1);
    }

    return CHIDB_OK;
}


/* Insert a new cell into a B-Tree node
 *
 * Inserts a new cell into a B-Tree node at a specified position ncell.
 * This involves the following:
 *  1. Add the cell at the top of the cell area. This involves "translating"
 *     the BTreeCell into the chidb format (refer to The chidb File Format
 *     document for the format of cells).
 *  2. Modify cells_offset in BTreeNode to reflect the growth in the cell area.
 *  3. Modify the cell offset array so that all values in positions >= ncell
 *     are shifted one position forward in the array. Then, set the value of
 *     position ncell to be the offset of the newly added cell.
 *
 * This function assumes that there is enough space for this cell in this node.
 *
 * Parameters
 * - btn: BTreeNode to insert cell in
 * - ncell: Cell number
 * - cell: BTreeCell to insert.
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_ECELLNO: The provided cell number is invalid
 */
int chidb_Btree_insertCell(BTreeNode *btn, ncell_t ncell, BTreeCell *cell)
{
    uint8_t *dat = btn->page->data;
    uint8_t *cell_pointer = NULL;
    uint8_t hexg[] = {0x0B, 0x03, 0x04, 0x04};

    if(ncell < 0 || ncell > btn->n_cells) {
            return CHIDB_ECELLNO;
    }

    switch(btn->type) {
        case PGTYPE_TABLE_LEAF:
            cell_pointer = dat + btn->cells_offset - cell->fields.tableLeaf.data_size - TABLELEAFCELL_SIZE_WITHOUTDATA;

            putVarint32(cell_pointer, cell->fields.tableLeaf.data_size);

            putVarint32(cell_pointer + 4, cell->key);

            memcpy(cell_pointer + 8, cell->fields.tableLeaf.data, cell->fields.tableLeaf.data_size);

            btn->cells_offset -= (cell->fields.tableLeaf.data_size + TABLELEAFCELL_SIZE_WITHOUTDATA);

        break;
        case PGTYPE_TABLE_INTERNAL:
            cell_pointer = dat + btn->cells_offset - TABLEINTCELL_SIZE;

            put4byte(cell_pointer, cell->fields.tableInternal.child_page);
            putVarint32(cell_pointer + 4, cell->key);

            btn->cells_offset -= TABLEINTCELL_SIZE;

        break;
        case PGTYPE_INDEX_INTERNAL:
            cell_pointer = dat + btn->cells_offset - INDEXINTCELL_SIZE;

            put4byte(cell_pointer, cell->fields.indexInternal.child_page);
            memcpy(cell_pointer + 4, hexg, 4);
            put4byte(cell_pointer + 8, cell->key);
            put4byte(cell_pointer + 12, cell->fields.indexInternal.keyPk);

            btn->cells_offset -= INDEXINTCELL_SIZE;

        break;
        case PGTYPE_INDEX_LEAF:
            cell_pointer = dat + btn->cells_offset - INDEXLEAFCELL_SIZE;

            memcpy(cell_pointer, hexg, 4);
            put4byte(cell_pointer + 4, cell->key);
            put4byte(cell_pointer + 8, cell->fields.indexLeaf.keyPk);

            btn->cells_offset -= INDEXLEAFCELL_SIZE;
        break;
    default:
	exit(1);

    }
 
    memmove(btn->celloffset_array + (ncell * 2) + 2, btn->celloffset_array + (ncell * 2), (btn->n_cells - ncell) * 2);
    put2byte(btn->celloffset_array + (ncell * 2), btn->cells_offset);
    btn->n_cells++;
    btn->free_offset += 2;

    return CHIDB_OK;
}

/* Find an entry in a table B-Tree
 *
 * Finds the data associated for a given key in a table B-Tree
 *
 * Parameters
 * - bt: B-Tree file
 * - nroot: Page number of the root node of the B-Tree we want search in
 * - key: Entry key
 * - data: Out-parameter where a copy of the data must be stored
 * - size: Number of bytes of data
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_ENOTFOUND: No entry with the given key way found
 * - CHIDB_ENOMEM: Could not allocate memory
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */
int chidb_Btree_find(BTree *bt, npage_t nroot, chidb_key_t key, uint8_t **data, uint16_t *size)
{
    BTreeCell cell;
    BTreeNode *btn;

    int status;
    int i;
    int temp_type;

    if ((status = chidb_Btree_getNodeByPage(bt, nroot, &btn)) != CHIDB_OK) {
        return status;
    }

    for (i = 0; i < btn->n_cells; i++) {
        if (chidb_Btree_getCell(btn, i, &cell) != CHIDB_OK) {
            return CHIDB_ECELLNO;
        }

        if ((cell.key == key) && (btn->type == PGTYPE_TABLE_LEAF)) {
            *size = cell.fields.tableLeaf.data_size;
            (*data) = (uint8_t *) malloc(sizeof(uint8_t) * (*size));
            if (!(*data)) {
                chidb_Btree_freeMemNode(bt, btn);
                return CHIDB_ENOMEM;
            }
            memcpy(*data, cell.fields.tableLeaf.data, *size);

            if ((status = chidb_Btree_freeMemNode(bt, btn)) != CHIDB_OK) {
                return status;
            }

            return CHIDB_OK;

        } else if (key <= cell.key) {
            temp_type = btn->type;
            if ((status = chidb_Btree_freeMemNode(bt, btn)) != CHIDB_OK) {
                return status;
            }

            if (temp_type != PGTYPE_TABLE_LEAF) {
                return chidb_Btree_find(bt, cell.fields.tableInternal.child_page, key, data, size);
            } else {
                return CHIDB_ENOTFOUND;
            }

        }

    }

    if (btn->type != PGTYPE_TABLE_LEAF) {
        i = btn->right_page;
        if ((status = chidb_Btree_freeMemNode(bt, btn)) != CHIDB_OK) {
            return status;
        }
        return chidb_Btree_find(bt, i, key, data, size);
    } 

    return CHIDB_ENOTFOUND;

}



/* Insert an entry into a table B-Tree
 *
 * This is a convenience function that wraps around chidb_Btree_insert.
 * It takes a key and data, and creates a BTreeCell that can be passed
 * along to chidb_Btree_insert.
 *
 * Parameters
 * - bt: B-Tree file
 * - nroot: Page number of the root node of the B-Tree we want to insert
 *          this entry in.
 * - key: Entry key
 * - data: Pointer to data we want to insert
 * - size: Number of bytes of data
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_EDUPLICATE: An entry with that key already exists
 * - CHIDB_ENOMEM: Could not allocate memory
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */
int chidb_Btree_insertInTable(BTree *bt, npage_t nroot, chidb_key_t key, uint8_t *data, uint16_t size)
{
    BTreeCell btc;

    btc.type = PGTYPE_TABLE_LEAF;
    btc.key = key;
    btc.fields.tableLeaf.data_size = size;
    btc.fields.tableLeaf.data = data;

    return chidb_Btree_insert(bt, nroot, &btc);

}


/* Insert an entry into an index B-Tree
 *
 * This is a convenience function that wraps around chidb_Btree_insert.
 * It takes a KeyIdx and a KeyPk, and creates a BTreeCell that can be passed
 * along to chidb_Btree_insert.
 *
 * Parameters
 * - bt: B-Tree file
 * - nroot: Page number of the root node of the B-Tree we want to insert
 *          this entry in.
 * - keyIdx: See The chidb File Format.
 * - keyPk: See The chidb File Format.
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_EDUPLICATE: An entry with that key already exists
 * - CHIDB_ENOMEM: Could not allocate memory
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */
int chidb_Btree_insertInIndex(BTree *bt, npage_t nroot, chidb_key_t keyIdx, chidb_key_t keyPk)
{
    BTreeCell btc;

    btc.type = PGTYPE_INDEX_LEAF;
    btc.key = keyIdx;
    btc.fields.indexLeaf.keyPk = keyPk;

    return chidb_Btree_insert(bt, nroot, &btc);

}

//Helper function for Btree_insert
//Checks if node has room for the cell
int hasRoomForCell(BTreeNode *btn, BTreeCell *btc)
{
    int space, size;

    size = 0;
    space = btn->cells_offset - btn->free_offset;

    switch(btc->type) {
        case PGTYPE_TABLE_LEAF:
            size = TABLELEAFCELL_SIZE_WITHOUTDATA + btc->fields.tableLeaf.data_size;
            break;
        case PGTYPE_TABLE_INTERNAL:
            size = TABLEINTCELL_SIZE;
            break;
        case PGTYPE_INDEX_LEAF:
            size = INDEXLEAFCELL_SIZE;
            break;
        case PGTYPE_INDEX_INTERNAL:
            size = INDEXINTCELL_SIZE;
            break;
    }

    if (space >= size) {
        return 1;
    } else {
        return 0;
    }

}
/* Insert a BTreeCell into a B-Tree
 *
 * The chidb_Btree_insert and chidb_Btree_insertNonFull functions
 * are responsible for inserting new entries into a B-Tree, although
 * chidb_Btree_insertNonFull is the one that actually does the
 * insertion. chidb_Btree_insert, however, first checks if the root
 * has to be split (a splitting operation that is different from
 * splitting any other node). If so, chidb_Btree_split is called
 * before calling chidb_Btree_insertNonFull.
 *
 * Parameters
 * - bt: B-Tree file
 * - nroot: Page number of the root node of the B-Tree we want to insert
 *          this cell in.
 * - btc: BTreeCell to insert into B-Tree
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_EDUPLICATE: An entry with that key already exists
 * - CHIDB_ENOMEM: Could not allocate memory
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */
int chidb_Btree_insert(BTree *bt, npage_t nroot, BTreeCell *btc)
{
    BTreeNode *root;
    BTreeNode *new_child;

    BTreeCell temp_cell;

    int status, i;
    uint8_t beginning_root_type;
    npage_t lower_num, new_child_num;

    if ((status = chidb_Btree_getNodeByPage(bt, nroot, &root)) != CHIDB_OK) {
        return status;
    }

    /*
     * WHERE IS THIS NODE FREE'D???
     */

    if (!hasRoomForCell(root, btc)) {

	// root doesn't have room, so we need to prepare a new right child and populate it with everything in the root

	// prepare new node and initialize (accomplished by newNode...calls initEmptyNode)
	if ((status = chidb_Btree_newNode(bt, &new_child_num, root->type)) != CHIDB_OK) {
	    return status;
	}
	// get that new node into the new_child btn
	if ((status = chidb_Btree_getNodeByPage(bt, new_child_num, &new_child)) != CHIDB_OK) {
	    return status;
	}
	
	// now, dump everything from the root into this new child node
	for (i = 0; i < root->n_cells; i++) {
	    // get the cell
	    if ((status = chidb_Btree_getCell(root, i, &temp_cell)) != CHIDB_OK) {
		return status;
	    }
	    // put the cell
	    if ((status = chidb_Btree_insertCell(new_child, i, &temp_cell)) != CHIDB_OK) {
		return status;
	    }
	}

	// in case we're splitting a root at any point other than the beginning, the root is going to have a right_page
	// this needs to be preserved in the new_child prior to splitting.
	switch(root->type) {
	case PGTYPE_INDEX_INTERNAL:
	case PGTYPE_TABLE_INTERNAL:
	    new_child->right_page = root->right_page;
	    break;
	default:
	    break;
	}
	
	// write and close the new child
	if ((status = chidb_Btree_writeNode(bt, new_child)) != CHIDB_OK) {
	    return status;
	}
	if ((status = chidb_Btree_freeMemNode(bt, new_child)) != CHIDB_OK) {
	    return status;
	}

	// reinitialize the root as appropriate type (if formerly leaf, make internal)
	beginning_root_type = root->type;

	// CLOSE THE ROOT BEFORE REINITIALIZING!!!
	if ((status = chidb_Btree_writeNode(bt, root)) != CHIDB_OK) {
            return status;
        }
        if ((status = chidb_Btree_freeMemNode(bt, root)) != CHIDB_OK) {
            return status;
        }

	switch(beginning_root_type) {
	case PGTYPE_INDEX_LEAF:
	case PGTYPE_INDEX_INTERNAL:
	    if ((status = chidb_Btree_initEmptyNode(bt, nroot, PGTYPE_INDEX_INTERNAL)) != CHIDB_OK) {
		return status;
	    }
	    break;
	case PGTYPE_TABLE_LEAF:
	case PGTYPE_TABLE_INTERNAL:
	    if ((status = chidb_Btree_initEmptyNode(bt, nroot, PGTYPE_TABLE_INTERNAL)) != CHIDB_OK) {
		return status;
	    }
	    break;
	default:
	    fprintf(stderr,"split: invalid root type\n");
	    exit(1);
	}

	// re-open the root
	if ((status = chidb_Btree_getNodeByPage(bt, nroot, &root)) != CHIDB_OK) {
	    return status;
	}

	// set the root's right_page to the new_child_num
	root->right_page = new_child_num;

	// write and close the root
	if ((status = chidb_Btree_writeNode(bt, root)) != CHIDB_OK) {
	    return status;
	}
	if ((status = chidb_Btree_freeMemNode(bt, root)) != CHIDB_OK) {
	    return status;
	}

	// split the root
	if ((status = chidb_Btree_split(bt, nroot, new_child_num, 0, &lower_num)) != CHIDB_OK) {
	    return status;
	}

    }

    return chidb_Btree_insertNonFull(bt, nroot, btc);

}

/* Insert a BTreeCell into a non-full B-Tree node
 *
 * chidb_Btree_insertNonFull inserts a BTreeCell into a node that is
 * assumed not to be full (i.e., does not require splitting). If the
 * node is a leaf node, the cell is directly added in the appropriate
 * position according to its key. If the node is an internal node, the
 * function will determine what child node it must insert it in, and
 * calls itself recursively on that child node. However, before doing so
 * it will check if the child node is full or not. If it is, then it will
 * have to be split first.
 *
 * Parameters
 * - bt: B-Tree file
 * - nroot: Page number of the root node of the B-Tree we want to insert
 *          this cell in.
 * - btc: BTreeCell to insert into B-Tree
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_EDUPLICATE: An entry with that key already exists
 * - CHIDB_ENOMEM: Could not allocate memory
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */
int chidb_Btree_insertNonFull(BTree *bt, npage_t npage, BTreeCell *btc)
{
    BTreeNode *btn;
    BTreeNode *child_btn;
    BTreeCell temp_cell;
    int i, status;
    npage_t child_num, temp_right_page;

    if ((status = chidb_Btree_getNodeByPage(bt, npage, &btn)) != CHIDB_OK) {
        return status;
    }

    if(!npage) {
	fprintf(stderr,"insertNonFull: npage is 0.......\n");
	exit(1);
    }

    for (i = 0; i < btn->n_cells; i++) {

        if (chidb_Btree_getCell(btn, i, &temp_cell) != CHIDB_OK) {
	    fprintf(stderr,"insertNonFull: ecelno on cell (%d)\n",i);
            return CHIDB_ECELLNO;
        }
        if ((temp_cell.key == btc->key) 
            && (btn->type != PGTYPE_TABLE_INTERNAL)) {

            if ((status = chidb_Btree_freeMemNode(bt, btn)) != CHIDB_OK) {
                return status;
            }

            return CHIDB_EDUPLICATE;

        }


        if (btc->key <= temp_cell.key) {
            switch(btn->type) {
                case PGTYPE_TABLE_INTERNAL:
                    if ((status = chidb_Btree_freeMemNode(bt, btn)) != CHIDB_OK) {
                        return status;
                    }

                    if ((status = chidb_Btree_getNodeByPage(bt, temp_cell.fields.tableInternal.child_page, &child_btn)) 
                        != CHIDB_OK) {
                        return status;
                    }

                    if (!hasRoomForCell(child_btn, btc)) {
			// close child_btn
			if ((status = chidb_Btree_freeMemNode(bt,child_btn)) != CHIDB_OK) {
			    return status;
			}
                        if ((status = chidb_Btree_split(bt, npage, temp_cell.fields.tableInternal.child_page, i, &child_num))
                            != CHIDB_OK) {
                            return status;
                        }
                        return chidb_Btree_insertNonFull(bt, npage, btc);
                    }

                    return chidb_Btree_insertNonFull(bt, temp_cell.fields.tableInternal.child_page, btc);

                case PGTYPE_INDEX_INTERNAL:   
                    if ((status = chidb_Btree_freeMemNode(bt, btn)) != CHIDB_OK) {
                        return status;
                    }

                    if ((status = chidb_Btree_getNodeByPage(bt, temp_cell.fields.indexInternal.child_page, &child_btn)) 
                        != CHIDB_OK) {
                        return status;
                    }

                    if (!hasRoomForCell(child_btn, btc)) {
			// close child_btn
			if ((status = chidb_Btree_freeMemNode(bt,child_btn)) != CHIDB_OK) {
			    return status;
			}
                        if ((status = chidb_Btree_split(bt, npage, temp_cell.fields.indexInternal.child_page, i, &child_num))
                            != CHIDB_OK) {
                            return status;
                        }
                        return chidb_Btree_insertNonFull(bt, npage, btc);
                    }

                    return chidb_Btree_insertNonFull(bt, temp_cell.fields.indexInternal.child_page, btc); 
                    
                case PGTYPE_TABLE_LEAF:
                case PGTYPE_INDEX_LEAF:
                    status = chidb_Btree_insertCell(btn, i, btc);
                    chidb_Btree_writeNode(bt, btn);
                    chidb_Btree_freeMemNode(bt, btn);
                    return status;

            }
        }

    }

    if ((btn->type == PGTYPE_INDEX_LEAF) || (btn->type == PGTYPE_TABLE_LEAF)) {
        status = chidb_Btree_insertCell(btn, i, btc);
        chidb_Btree_writeNode(bt, btn);
        chidb_Btree_freeMemNode(bt, btn);
        return status;
    } else {
        temp_right_page = btn->right_page;

        chidb_Btree_freeMemNode(bt, btn);

        if ((status = chidb_Btree_getNodeByPage(bt, temp_right_page, &child_btn)) 
            != CHIDB_OK) {
            return status;
        }

        if (!hasRoomForCell(child_btn, btc)) {
	    // close child_btn
	    if ((status = chidb_Btree_freeMemNode(bt,child_btn)) != CHIDB_OK) {
		return status;
	    }
            if ((status = chidb_Btree_split(bt, npage, temp_right_page, i, &child_num))
                != CHIDB_OK) {
                return status;
            }
            return chidb_Btree_insertNonFull(bt, npage, btc);
        }

        return chidb_Btree_insertNonFull(bt, temp_right_page, btc);
    }
}


/* Split a B-Tree node
 *
 * Splits a B-Tree node N. This involves the following:
 * - Find the median cell in N.
 * - Create a new B-Tree node M.
 * - Move the cells before the median cell to M (if the
 *   cell is a table leaf cell, the median cell is moved too)
 * - Add a cell to the parent (which, by definition, will be an
 *   internal page) with the median key and the page number of M.
 *
 * Parameters
 * - bt: B-Tree file
 * - npage_parent: Page number of the parent node
 * - npage_child: Page number of the node to split
 * - parent_ncell: Position in the parent where the new cell will
 *                 be inserted.
 * - npage_child2: Out parameter. Used to return the page of the new child node.
 * - btc: BTreeCell to insert into B-Tree
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_ENOMEM: Could not allocate memory
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */
int chidb_Btree_split(BTree *bt, npage_t npage_parent, npage_t npage_child, ncell_t parent_ncell, npage_t *npage_child2)
{

    BTreeNode *parent;
    BTreeNode *child;

    BTreeNode *upper;
    npage_t upper_num;

    BTreeNode *lower;
    npage_t lower_num, upper_right;

    BTreeNode *temp;
    npage_t temp_num;

    BTreeCell insert_median_cell;
    BTreeCell original_median_cell;
    BTreeCell temp_cell;

    int i, j, status, median_index;

    //get parent page
    if ((status = chidb_Btree_getNodeByPage(bt, npage_parent, &parent)) != CHIDB_OK) {
        return status;
    }

    //get child page
    if ((status = chidb_Btree_getNodeByPage(bt, npage_child, &child)) != CHIDB_OK) {
        return status;
    }

    median_index = child->n_cells / 2;

    //initialize new node with page_num = lower_num
    if ((status = chidb_Btree_newNode(bt, &lower_num, child->type)) != CHIDB_OK) {
        return status;
    }

    //get page at page_num = lower_num
    if ((status = chidb_Btree_getNodeByPage(bt, lower_num, &lower)) != CHIDB_OK) {
        return status;
    }

    upper = child;
    upper_num = npage_child;

    //initialize new node with page_num = temp_num
    if ((status = chidb_Btree_newNode(bt, &temp_num, child->type)) != CHIDB_OK) {
        return status;
    }

    //get page at page_num = temp_num
    if ((status = chidb_Btree_getNodeByPage(bt, temp_num, &temp))) {
        return status;
    }

    //Setup insert_median_cell for insertion into parent
    if ((status = chidb_Btree_getCell(child, median_index, &original_median_cell)) != CHIDB_OK) {
        return status;
    }

    insert_median_cell.type = parent->type;
    insert_median_cell.key = original_median_cell.key;

    switch(insert_median_cell.type) {
        case PGTYPE_TABLE_INTERNAL:
            insert_median_cell.fields.tableInternal.child_page = lower_num;
            break;
        case PGTYPE_INDEX_INTERNAL:

            insert_median_cell.fields.indexInternal.child_page = lower_num;

            if (original_median_cell.type == PGTYPE_INDEX_INTERNAL) {
                insert_median_cell.fields.indexInternal.keyPk = original_median_cell.fields.indexInternal.keyPk;
            } else {
                insert_median_cell.fields.indexInternal.keyPk = original_median_cell.fields.indexLeaf.keyPk;
            }
	    break;
        default:
	    fprintf(stderr,"split: type of parent should never be a leaf type; got type (%d)\n",insert_median_cell.type);
	    exit(1);
    }

    //Insert insert_median_cell into parent
    if ((status = chidb_Btree_insertCell(parent, parent_ncell, &insert_median_cell)) != CHIDB_OK) {

	fprintf(stderr,"split: median cell insert into parent error (%d)\n",status);

        return status;
    }

    //copy cells below the median to lower
    for (i = 0; i < median_index; i++) {
        if ((status = chidb_Btree_getCell(upper, i, &temp_cell)) != CHIDB_OK) {
            return status;
        }
        if ((status = chidb_Btree_insertCell(lower, i, &temp_cell)) != CHIDB_OK) {
            return status;
        }
    }

    //copy original median cell if necessary
    // get that original median cell into temp_cell
    if ((status = chidb_Btree_getCell(upper, i, &temp_cell)) != CHIDB_OK) {
	return status;
    }	
    if(upper->type == PGTYPE_TABLE_LEAF) {
	if ((status = chidb_Btree_insertCell(lower, i, &temp_cell)) != CHIDB_OK) {
	    return status;
	}
	i++;
    } else if (temp_cell.type != PGTYPE_INDEX_LEAF) {
	// if the type of the children is not a leaf type, then we must set right_page of lower to the former child of the median
	switch(temp_cell.type) {
	case PGTYPE_TABLE_INTERNAL:
	    lower->right_page = temp_cell.fields.tableInternal.child_page;
	    break;
	case PGTYPE_INDEX_INTERNAL:
	    lower->right_page = temp_cell.fields.indexInternal.child_page;
	    break;
	}
	if(lower->right_page == 0) {
	    fprintf(stderr,"split: 0 right page passed\n");
	    exit(2);
	}
    }

    //copy cells above the median to temp
    for (j = 0; i < upper->n_cells; i++, j++) {
        if ((status = chidb_Btree_getCell(upper, i, &temp_cell)) != CHIDB_OK) {
            return status;
        }
        if ((status = chidb_Btree_insertCell(temp, j, &temp_cell)) != CHIDB_OK) {
            return status;
        }        
    }

    upper_right = upper->right_page;
    // close upper

    if ((status = chidb_Btree_freeMemNode(bt,upper)) != CHIDB_OK) {
	return status;
    }

    //Clear upper
    if ((status = chidb_Btree_initEmptyNode(bt, upper_num, temp->type)) != CHIDB_OK) {
        return status;
    }

    // reopen upper

    if ((status = chidb_Btree_getNodeByPage(bt, upper_num, &upper)) != CHIDB_OK) {
	return status;
    }
    upper->right_page = upper_right;

    //insert each cell in temp into upper
    for (i = 0; i < temp->n_cells; i++) {
        if ((status = chidb_Btree_getCell(temp, i, &temp_cell)) != CHIDB_OK) {
            return status;
        }
        if ((status = chidb_Btree_insertCell(upper, i, &temp_cell)) != CHIDB_OK) {
            return status;
        }           
    }

    //free temp
    //decrement number of pages (like it was never here)
    chidb_Btree_freeMemNode(bt, temp);
    bt->pager->n_pages--;

    //write parent
    if ((status = chidb_Btree_writeNode(bt, parent)) != CHIDB_OK) {
        return status;
    }

    //write upper
    if ((status = chidb_Btree_writeNode(bt, upper)) != CHIDB_OK) {
        return status;
    }

    //write lower
    if ((status = chidb_Btree_writeNode(bt, lower)) != CHIDB_OK) {
        return status;
    }

    //set out parameter to page number of child node
    *npage_child2 = lower_num;

    //free in-memory nodes
    chidb_Btree_freeMemNode(bt, parent);
    chidb_Btree_freeMemNode(bt, upper);
    chidb_Btree_freeMemNode(bt, lower);

    return CHIDB_OK;

}

