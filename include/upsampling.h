#ifndef __UPSAMPLING_H__
#define __UPSAMPLING_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "jpeg_reader.h"
#include "mcu.h"


enum sampling { SAMPLING_V, SAMPLING_H, SAMPLING_HV, NO_SAMPLING, SAMPLING_NOT_FOUND };

extern enum sampling get_sampling_type(struct jpeg_desc* jdesc);

extern void upsampling_horizontal(struct components* components, struct components* new_components);

extern void upsampling_vertical(struct components* components, struct components* new_components);

extern void upsampling_vertical_after_horizontal(struct components* components, struct components* new_components);

extern struct components upsampling(struct mcu* mcu, struct jpeg_desc* jdesc);

// print test function
extern void testupsampling(void);

#endif
