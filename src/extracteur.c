/**
* @file extracteur.c
* @author Odeh, Morin, Pouget
* @date 31 May 2019
* @copyright 2019 Odeh, Morin, Pouget
* @brief extract DC and AC components
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "huffman.h"
#include "jpeg_reader.h"
#include "bitstream.h"
#include "mcu.h"


// calculate the magnitude value using its class and range
int16_t get_magnitude(int16_t class, uint32_t index){
  /**
* @brief calculate the magnitude value using its class and range
* @param [in] class
* @param [in] index
* @return magnitude value
*/

    assert(index<pow(2,class));
    if (index<pow(2,class-1)){
        return -pow(2,class)+1+index;
    } else {
        return index;
    }
}

// get the DC value
void get_dc(struct huff_table *htableDC, struct bitstream *stream, int16_t *block_table){
  /**
* @brief get DC value
* @param [in] htableDC : table DC
* @param [in] stream
* @param [in] block_table : block
* @return DC value in first position of block
*/

    uint32_t byte = 0;
    int8_t huff_value = next_huffman_value(htableDC, stream);
    read_bitstream(stream, huff_value, &byte, true);
    block_table[0] = get_magnitude(huff_value, byte);
}


// get the 63 values of AC
void get_ac(struct huff_table *htableAC, struct bitstream *stream, int16_t * bloc_table){
  /**
* @brief get the 63 values of AC
* @param [in] htableAC : AC table
* @param [in] stream
* @param [in] bloc_table : block
* @return AC values in block
* @details use all cases defined in topic
*/

    bool end_of_block = false;
    uint32_t byte = 0;
	uint16_t counter = 1;
	uint16_t tmp;
	while(!end_of_block && counter < 64){
		uint16_t huff_value = (uint16_t)next_huffman_value(htableAC, stream);

		if(huff_value == 0x00){
            tmp = counter;
            for(uint16_t index = tmp; index < 64; index++){
                bloc_table[index] = 0;
                counter++;
            }
			end_of_block = true;
			// terminate the loop using break statement
			break;
		}
		else if(huff_value == 0xF0){
			tmp = counter;
			for(uint16_t index = tmp; index < tmp+16; index++){
                // no need to store 0 in bloc_table (already initialized with zeros)
                bloc_table[index] = 0;
				counter++;
			}
		} else if (huff_value << 12 == 0) {
            printf("RLE Code not valid");
            exit(EXIT_FAILURE);
        }
        else{

            uint8_t msb = (uint8_t)huff_value >> 4;
            uint8_t lsb = (uint8_t)huff_value & 0x0F;

            if( msb > 0){
                tmp = counter;
                for(uint16_t index =tmp; index < tmp+msb; index++){
                    bloc_table[index] = 0;
                    counter++;
                }
            }
            read_bitstream(stream, lsb, &byte, true);
            bloc_table[counter] = get_magnitude(lsb, byte);

    		counter++;
        }
        if(counter == 64){
            end_of_block = true;
            // terminate the loop using break statement
            break;
        }

	}
}


int16_t *read_block(struct jpeg_desc *jdesc, struct bitstream *stream, enum component component){
  /**
* @brief produce block
* @param [in] jdesc : contains information from jpeg  section headers
* @param [in] stream
* @param [in] component : Y, Cb, Cr
* @return an 64 values array representing a block by using get_dc and get_ac functions
* @details reads a 8x8 block using get_dc and get_ac functions, also stores previous DC values for each component.
*/

    static int16_t previous_DC[3] = {0};

    int16_t *block_table = calloc(64, sizeof(int16_t));

    uint8_t index = get_frame_component_quant_index(jdesc, component);

    // get huffman tables for DC and AC
    struct huff_table *htableDC = get_huffman_table(jdesc, DC, index);
    struct huff_table *htableAC = get_huffman_table(jdesc, AC, index);

    // get DC and AC values, stored in block_table
    get_dc(htableDC, stream, block_table);
    get_ac(htableAC, stream, block_table);

    // add previous_DC to DC
    block_table[0] = block_table[0] + previous_DC[component];
    previous_DC[component] = block_table[0];
    return block_table;
}

uint8_t *get_components_blocks_number(struct jpeg_desc *jdesc){
    uint8_t nbcomp = get_nb_components(jdesc);

    uint8_t *result = malloc(nbcomp*sizeof(int8_t));
    for (uint8_t j = 0; j < nbcomp; j++) {
        uint8_t sampV = get_frame_component_sampling_factor(jdesc, DIR_V, j!=0);
        uint8_t sampH = get_frame_component_sampling_factor(jdesc, DIR_H, j!=0);
        uint8_t nbblocs = sampV * sampH;
        result[j] = nbblocs;
    }
    return result;
}
