#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "jpeg_reader.h"

extern uint16_t get_pixels_nb_in_direction(struct jpeg_desc* jdesc, enum direction DIR);

extern uint32_t get_mcu_number_in_image(struct jpeg_desc* jdesc);

extern uint32_t *allocate_image(struct jpeg_desc *jdesc);

// print test function
extern void testimage(void);

#endif
