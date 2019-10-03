/**
* @file bitstream.c
* @author Odeh, Morin, Pouget
* @date 7 Jun 2019
* @copyright 2019 Odeh, Morin, Pouget
* @brief constitute our own bitstream
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include "bitstream.h"

struct bitstream {
  /**
* @brief implemente structure for bitstream
* @details 4 parameters image as a file, current_byte, current_bit_offset, total_nb_of_byte, nb_of_byte_consumed
*/

    FILE *image;                       // pointer on image's data
    uint8_t current_byte;              // current byte pointed by image
    uint8_t current_bit_offset;        // position in current byte
    uint32_t total_nb_of_byte;         // number of byte in file
    uint32_t nb_of_byte_consumed;      // number of byte readen
};

struct bitstream *create_bitstream(const char *filename){
  /**
* @brief create a bitstream given a file
* @param [in] filename : name of file concerned by bitstream creation
* @return stream
*/
    FILE *image = fopen(filename, "r");
    // get size of the file
    fseek(image , 0 , SEEK_END);
    int size = ftell(image);
    rewind(image);
    if (size == 0) {
        fprintf(stderr, "ERROR IN BITSTREAM : The file given is empty\n");
        exit(EXIT_FAILURE);
    }
    struct bitstream *stream = malloc(sizeof(struct bitstream));
    // initialization of stream attributes
    fread(&stream->current_byte, sizeof(uint8_t), 1, image);
    stream->total_nb_of_byte = size + 1;
    stream->nb_of_byte_consumed = 0;
    stream->current_bit_offset = 0;
    stream->image = image;
    return stream;
}

void close_bitstream(struct bitstream *stream){
  /**
* @brief close file and free memory reserved for the stream
* @param [in] stream
*/
    fclose(stream->image);
    free(stream);
}

void next_byte(struct bitstream *stream){
  /**
* @brief update stream current byte
* @param [in] stream
* @details increment nb_of_byte_consumed
*/
    fread(&stream->current_byte, sizeof(uint8_t), 1, stream->image);
    stream->nb_of_byte_consumed++;
    stream->current_bit_offset = 0;
}

void read_bit(struct bitstream *stream, uint8_t *bit, bool discard_byte_stuffing, uint8_t *nb_of_bits_readen){
  /**
* @brief read a bit
* @param [in] stream
* @param [in] bit
* @param [in] discard_byte_stuffing
* @param [in] nb_of_bits_readen : to be incremented
*/
    // select the correct bit in current byte
    *bit = stream->current_byte >> (7 - stream->current_bit_offset) & 1;
    *nb_of_bits_readen += 1;
    stream->current_bit_offset += 1;
    uint8_t last_byte;
    if (stream->current_bit_offset == 8){
        // if current bit offset is maximum, lets look at the next byte
        last_byte = stream->current_byte;
        next_byte(stream);
        if (discard_byte_stuffing){
            if (last_byte == 0xff & stream->current_byte == 0x00){
                next_byte(stream);
            }
        }
    }
}

uint8_t read_bitstream(struct bitstream *stream, uint8_t nb_bits, uint32_t *dest, bool discard_byte_stuffing){
  /**
* @brief read a number of bits in the stream
* @param [in] stream
* @param [in] nb_bits
* @param [in] destination of readen bits : dest
* @param [in] discard_byte_stuffing
* @return nb_of_bits_readen
* @details calling read_bit and end_of_bitstream functions
*/

    uint8_t nb_of_bits_readen = 0;
    uint8_t bit;
    *dest = 0;
    // while there are still some bits to read
    while (nb_of_bits_readen != nb_bits){
        if (end_of_bitstream(stream)) {
            break;
        }
        // read 1 bit
        read_bit(stream, &bit, discard_byte_stuffing, &nb_of_bits_readen);
        // add this bit in dest
        *dest = (*dest << 1) + bit;
    }
    return nb_of_bits_readen;
}

bool end_of_bitstream(struct bitstream *stream){
  /**
* @brief answer the question : Is it the end of bitstream ?
* @param [in] stream
* @return boolean
*/
    return (stream->nb_of_byte_consumed == stream->total_nb_of_byte);
}

void skip_bitstream_until(struct bitstream *stream, uint8_t byte){
  /**
* @brief skip bits until an expected byte
* @param [in] stream
* @param [in] searched byte
*/
    while (stream->current_byte != byte){
        if (end_of_bitstream(stream)){
            fprintf(stderr, "ERROR in bitstream : byte not found by skip_bitstream\n");
            exit(EXIT_FAILURE);
        }
        next_byte(stream);
    }
}
