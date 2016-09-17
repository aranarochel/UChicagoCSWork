#ifndef _ENCODE_H
#define _ENCODE_H

#include "huff.h"
#include "huffll.h"

/* return an array of length 26 */
int *letter_freq(char *m);

/* given a letter frequency, build list of corresponding leaves */
/* (singleton Huffman trees) */
huffll *leaves(int *freq);

/* build Huffman tree from message */
huff *huff_from_msg(char *m);

#endif /* _ENCODE_H */
