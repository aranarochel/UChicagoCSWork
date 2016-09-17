/*
 *  chidb - a didactic relational database management system
 *
 *  Database Machine cursors -- header
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


#ifndef DBM_CURSOR_H_
#define DBM_CURSOR_H_

#include "chidbInt.h"
#include "btree.h"
#include "../simclist/simclist.h"

typedef uint32_t ncol_t;   // number of columns a table has OR the number of a column

typedef enum chidb_dbm_cursor_type
{
    CURSOR_UNSPECIFIED,
    CURSOR_READ,
    CURSOR_WRITE
} chidb_dbm_cursor_type_t;

typedef enum chidb_dbm_seek_type
{
    SEEK,
    SEEKLE,
    SEEKGE,
    SEEKLT,
    SEEKGT
} chidb_dbm_seek_type_t;

typedef struct chidb_dbm_cursor_trail
{
    uint32_t depth; // 0 indexed from root

    BTreeNode *btn; // data structure representation w/ mempage

    int n_current_cell; // cell whose child page we're currently down
} chidb_dbm_cursor_trail_t;

typedef struct chidb_dbm_cursor
{
    BTreeCell current_cell; // access to data (current table cell the cursor is pointing to)

    npage_t root_page;      // root page of the table in case of reloading
    uint8_t root_type;      // type of page the root is (can be any of the four)

    ncol_t n_cols;          // number of columns in the table
    list_t trail;           // holds chidb_dbm_cursor_trail

    chidb_dbm_cursor_type_t type;

} chidb_dbm_cursor_t;

/* Cursor function definitions go here */
int chidb_dbm_cursor_print(chidb_dbm_cursor_t *c);

int chidb_dbm_cursor_trail_new(BTree *bt, chidb_dbm_cursor_trail_t **ct, npage_t npage, uint32_t depth);
int chidb_dbm_cursor_trail_list_destroy(BTree *bt, list_t *restrict l);
int chidb_dbm_cursor_trail_cpy(BTree *bt, list_t *restrict l1, list_t *restrict l2);
int chidb_dbm_cursor_clear_trail_from(BTree *bt, chidb_dbm_cursor_t *c, uint32_t depth); // clears everything NOT INCLUDING given depth
int chidb_dbm_cursor_trail_remove_at(BTree *bt, chidb_dbm_cursor_t *c, uint32_t depth);

int chidb_dbm_cursor_init(BTree *bt, chidb_dbm_cursor_t *c, npage_t root_page, ncol_t n_cols);
int chidb_dbm_cursor_destroy(BTree *bt, chidb_dbm_cursor_t *c);

int chidb_dbm_cursor_fwd(BTree *bt, chidb_dbm_cursor_t *c);
int chidb_dbm_cursorTable_fwd(BTree *bt, chidb_dbm_cursor_t *c);
int chidb_dbm_cursorTable_fwdUp(BTree *bt, chidb_dbm_cursor_t *c);
int chidb_dbm_cursorTable_fwdDwn(BTree *bt, chidb_dbm_cursor_t *c);
int chidb_dbm_cursorIndex_fwd(BTree *bt, chidb_dbm_cursor_t *c);
int chidb_dbm_cursorIndex_fwdUp(BTree *bt, chidb_dbm_cursor_t *c);
int chidb_dbm_cursorIndex_fwdDwn(BTree *bt, chidb_dbm_cursor_t *c);

int chidb_dbm_cursor_rev(BTree *bt, chidb_dbm_cursor_t *c);
int chidb_dbm_cursorTable_rev(BTree *bt, chidb_dbm_cursor_t *c);
int chidb_dbm_cursorTable_revUp(BTree *bt, chidb_dbm_cursor_t *c);
int chidb_dbm_cursorTable_revDwn(BTree *bt, chidb_dbm_cursor_t *c);
int chidb_dbm_cursorIndex_rev(BTree *bt, chidb_dbm_cursor_t *c);
int chidb_dbm_cursorIndex_revUp(BTree *bt, chidb_dbm_cursor_t *c);
int chidb_dbm_cursorIndex_revDwn(BTree *bt, chidb_dbm_cursor_t *c);

int chidb_dbm_cursor_seek(BTree *bt, chidb_dbm_cursor_t *c, chidb_key_t key, npage_t next, int depth, int seek_type);

#endif /* DBM_CURSOR_H_ */