#ifndef __BITSTREAM_H__
#define __BITSTREAM_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>


struct bitstream;

extern struct bitstream *create_bitstream(const char *filename);

extern void close_bitstream(struct bitstream *stream);

extern uint8_t read_bitstream(struct bitstream *stream,
                              uint8_t nb_bits,
                              uint32_t *dest,
                              bool discard_byte_stuffing);

extern void skip_bitstream_until(struct bitstream *stream, uint8_t byte);

extern bool end_of_bitstream(struct bitstream *stream);

#endif
