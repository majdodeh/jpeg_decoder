#ifndef __MCU_H__
#define __MCU_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include "jpeg_reader.h"
// #include "upsampling.h"


// each component Y, Cb, Cr contains all its own blocks
struct components {
    int16_t *Y;
    int16_t *Cb;
    int16_t *Cr;
};

 // Components table of a MCU, each component Y, Cb and Cr may contain multiple arrays.
struct components_tab {
    int16_t **Y;
    int16_t **Cb;
    int16_t **Cr;
};

// structure to store all MCU components
struct mcu {
    struct components_tab components;
    uint8_t *size_components;
    uint8_t nb_components;
};



extern struct components create_components(struct jpeg_desc* jdesc);

extern struct components convert_mcu_to_components(struct mcu *mcu, struct jpeg_desc* jdesc);

extern uint8_t free_mcu(struct mcu mcu);

extern void free_components(struct components *components, struct jpeg_desc* jdesc);

// print test function
extern void testmcu(void);

#endif
