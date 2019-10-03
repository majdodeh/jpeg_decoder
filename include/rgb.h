#ifndef __RGB_H__
#define __RGB_H__

#include <stdint.h>
#include <stdbool.h>
#include "bitstream.h"
#include "mcu.h"
#include <stdio.h>

extern uint8_t *convert_to_rgb(uint8_t y, uint8_t Cb, uint8_t Cr);

extern uint32_t *convert_to_rgb_tab(uint8_t *y, uint8_t *Cb, uint8_t *Cr);

void convert_components_to_rgb(struct components *components, uint32_t *pixels_rgb, struct jpeg_desc* jdesc);


#endif
