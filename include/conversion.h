#ifndef __CONVERSION_H__
#define __CONVERSION_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include "jpeg_reader.h"
#include "mcu.h"
#include "upsampling.h"


extern struct mcu extractor(struct jpeg_desc *jdesc, struct bitstream *stream);

extern void iqzz_idct(struct mcu *mcu, struct jpeg_desc *jdesc, float *cos_table);

extern uint32_t *troncature(uint32_t *image, struct jpeg_desc *jdesc);

extern void arrange_pixels_in_image(struct components *components, struct jpeg_desc *jdesc, uint32_t * image, enum sampling type, uint32_t index);

// print test function
extern void testconversion(void);

#endif
