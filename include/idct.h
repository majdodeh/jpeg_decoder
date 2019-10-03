#ifndef __IDCT_H__
#define __IDCT_H__

// #include "bitstream.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


extern void idct(int16_t *freq_table);
// calculate the inverse Discrete Cosine Transform
extern void idct_optimized_1(int16_t *freq_table, float *cos_table);

extern void idct_optimized_2(int16_t *freq_table, float *cos_sin_table);

extern void define_cos_table(float *cos_table);

extern void define_cos_sin_table(float *cos_table);

#endif
