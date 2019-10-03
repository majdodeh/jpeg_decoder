#ifndef __IQZZ_H__
#define __IQZZ_H__

#include <stdint.h>
#include <stdbool.h>
#include "bitstream.h"
#include <stdio.h>


// inverse zig zag of an array (1 x 64), result in 1d array (1 x 64) passed in parameter
extern void zigzag_inverse( int16_t *vecteur, int16_t *freq_table);

// optimized version of ZigZag
extern void zigzag_inverse_optimized_1( int16_t *array, int16_t *freq_table);

// calculate the quant inverse of a table of frequences
extern void quantification_inverse(int16_t *freq_table, uint8_t *quant_table);

// print test function
extern void testiqzz(void);

#endif
