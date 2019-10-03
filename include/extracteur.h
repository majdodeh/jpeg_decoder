#ifndef __EXTRACTEUR_H__
#define __EXTRACTEUR_H__
#include "jpeg_reader.h"
#include "bitstream.h"

extern int16_t get_magnitude(uint16_t class, uint32_t index);
extern void get_dc(struct huff_table *htableDC, struct bitstream *stream, int16_t *block_table);
extern void get_ac(struct huff_table *htableAC, struct bitstream *stream, int16_t *bloc_table);

extern int16_t* read_block(struct jpeg_desc *jdesc, struct bitstream *stream, enum component component);
extern uint8_t *get_components_blocks_number(struct jpeg_desc *jdesc);

#endif
