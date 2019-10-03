/**
* @file conversion.c
* @author Odeh, Morin, Pouget
* @date 31 May 2019
* @copyright 2019 Odeh, Morin, Pouget
* @brief handle conversion issues
*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "conversion.h"
#include "mcu.h"
#include "iqzz.h"
#include "idct.h"
#include "extracteur.h"
#include "image.h"
#include "rgb.h"


void *order_components(struct jpeg_desc *jdesc, uint8_t *order){
    
    // get Y and Cb id(s) in frame
    uint8_t Y_id = get_frame_component_id(jdesc, COMP_Y);
    uint8_t Cb_id = get_frame_component_id(jdesc, COMP_Cb);

    // get the first and second place id(s) in SOS section
    uint8_t first_place = get_scan_component_id(jdesc, 0);
    uint8_t second_place = get_scan_component_id(jdesc, 1);

    if (first_place == Y_id) {
        // first component is Y
        order[0] = 0;
        if (second_place == Cb_id){
            order[1] = 1;
            order[2] = 2;
        } else {
            order[1] = 2;
            order[2] = 1;
        }
    } else if (first_place == Cb_id) {
        // first component Cb
        order[0] = 1;
        if (second_place == Y_id){
            order[1] = 0;
            order[2] = 2;
        } else {
            order[1] = 2;
            order[2] = 0;
        }

    } else {
        // first component Cr
        order[0] = 2;
        if (second_place == Cb_id){
            order[1] = 1;
            order[2] = 0;
        } else {
            order[1] = 0;
            order[2] = 1;
        }
    }
    return order;
}

// first step
struct mcu extractor(struct jpeg_desc *jdesc, struct bitstream *stream){
/**
* @brief extracts MCU
* @param [in] jdesc : contains information from jpeg  section headers
* @param [in] stream
* @return mcu according to struct mcu
*/
    uint8_t components_nb = get_nb_components(jdesc);
    uint8_t *blocks_nb = get_components_blocks_number(jdesc);
    int16_t ***res = calloc(3, sizeof(int16_t **));

    uint8_t *order = malloc(components_nb*sizeof(uint8_t));
    if (components_nb == 3){
        order_components(jdesc, order);
    } else {
        order[0] = 0;
    }
    for (uint8_t i = 0; i < components_nb; i++) {
        res[i] = malloc(blocks_nb[i] * sizeof(int16_t *));
        for (uint8_t j = 0; j < blocks_nb[i]; j++) {
            res[order[i]][j] = read_block(jdesc, stream, i);
        }
    }
    struct components_tab compo = {res[0], res[1], res[2]};
    struct mcu mcu = {compo, blocks_nb, components_nb};
    free(res);
    free(order);
    return mcu;
}

// second and third steps
void iqzz_idct(struct mcu *mcu, struct jpeg_desc *jdesc, float *cos_table){
  /**
* @brief implement iqzz and idct
* @param [in] mcu
* @param [in] jdesc : contains information from jpeg  section headers
* @param [in] cos_table : stored values of cos and sin
* @return transformed MCU
*/
	uint8_t *size_components = mcu->size_components;
	int16_t **pcomponents[3] = {mcu->components.Y, mcu->components.Cb, mcu->components.Cr};
	// For every component (Y Cb Cr)
	for (uint8_t component = 0; component < mcu->nb_components; component++) {
		// For every block in each component (i.e. 2Y, 1Cb, 1Cr)
		for (uint8_t comp_block = 0; comp_block < size_components[component]; comp_block++) {
			// quantification inverse
			uint8_t *quant_table = get_quantization_table(jdesc, component != 0);
			quantification_inverse(pcomponents[component][comp_block], quant_table);
			// zigzag inverse
			int16_t freq_table[64];
			zigzag_inverse_optimized_1(pcomponents[component][comp_block], freq_table);
			/* idct */
			idct_optimized_2(freq_table, cos_table);
			// copy freq_table to the component block
			for (int16_t i=0; i<64; i++){
				pcomponents[component][comp_block][i] = freq_table[i];
			}
			// free(freq_table);

		}
	}
}


uint32_t *troncature(uint32_t *image, struct jpeg_desc *jdesc){
    /**
* @brief truncate image to manage information
* @param [in] image : image as an array
* @param [in] jdesc : contains information from jpeg  section headers
* @return truncated image
* @details handle cases where image size can not be divided by 8
*/
    // image dimensions
    uint16_t width_overflow = get_pixels_nb_in_direction(jdesc, DIR_H);
    uint16_t width = get_image_size(jdesc, DIR_H);
    uint16_t height = get_image_size(jdesc, DIR_V);
    uint32_t size = height*width;

    // create an array to store the selected pixels
    uint32_t *result = malloc(size*sizeof(uint32_t));
    for (uint16_t j=0; j<height; j++) {
        for (uint16_t i=0; i<width; i++) {
            result[j*width+i] = image[j*width_overflow+i];
        }
    }
    return result;
}

void arrange_pixels_in_image(struct components *components, struct jpeg_desc *jdesc, uint32_t * image, enum sampling type, uint32_t index){
/**
* @brief  Convert YCbCr to RGB and arrange pixels in image
* @param [in]  components : Y, Cb, Cr
* @param [in]  jdesc  : contains information from jpeg  section headers
* @param [in]  image  : image as an array
* @param [in]  type  : type of sampling horizontal, vertical, horizontal and vertical or no sampling
* @param [in]  index
* @return  reshaped image
*/

    uint8_t Y_HOR = get_frame_component_sampling_factor(jdesc, DIR_H, 0);
    uint8_t Y_VER = get_frame_component_sampling_factor(jdesc, DIR_V, 0);
    uint8_t blocks_number = Y_HOR*Y_VER;
    uint8_t components_number = get_nb_components(jdesc);
    uint16_t width = get_pixels_nb_in_direction(jdesc, DIR_H);
    uint16_t nb_mcu_horizontal = width/(Y_HOR*8);

    if (components_number == 3){

        /* [COLORED] i.e FOR INVADER, GRIS and BISOU */
        uint32_t pixels_rgb[64*blocks_number];

        convert_components_to_rgb(components, pixels_rgb, jdesc);

        if (type == SAMPLING_H){
            for (uint32_t l=0; l<8; l++) {
                for (uint32_t c=0; c<8; c++) {
                    image[(width*l)+c+((Y_HOR*8)*((int32_t)index%nb_mcu_horizontal))+(((int32_t)index/nb_mcu_horizontal) * width*(Y_VER*8))] = pixels_rgb[(l*8)+c];
                    image[(width*l)+c+((Y_HOR*8)*((int32_t)index%nb_mcu_horizontal))+(((int32_t)index/nb_mcu_horizontal) * width*(Y_VER*8)+8)] = pixels_rgb[64+(l*8)+c];
                }
            }
        }
        else if (type == SAMPLING_V){
            for (uint32_t l=0; l<16; l++) {
                for (uint32_t c=0; c<8; c++) {
                    image[(width*l)+c+((Y_HOR*8)*((int32_t)index%nb_mcu_horizontal))+(((int32_t)index/nb_mcu_horizontal) * width*(Y_VER*8))] = pixels_rgb[(l*8)+c];
                }
            }
        }
        else if (type == SAMPLING_HV){
            for (uint32_t l=0; l<8; l++) {
                for (uint32_t c=0; c<8; c++) {
                    image[(width*l)+c+((Y_HOR*8)*((int32_t)index%nb_mcu_horizontal))+(((int32_t)index/nb_mcu_horizontal) * width*(Y_VER*8))] = pixels_rgb[(l*8)+c];
                    image[(width*l)+c+((Y_HOR*8)*((int32_t)index%nb_mcu_horizontal))+(((int32_t)index/nb_mcu_horizontal) * width*(Y_VER*8)+8)] = pixels_rgb[64+(l*8)+c];
                    image[(width*8)+(width*l)+c+((Y_HOR*8)*((int32_t)index%nb_mcu_horizontal))+(((int32_t)index/nb_mcu_horizontal) * width*(Y_VER*8))] = pixels_rgb[128+(l*8)+c];
                    image[(width*8)+(width*l)+c+((Y_HOR*8)*((int32_t)index%nb_mcu_horizontal))+(((int32_t)index/nb_mcu_horizontal) * width*(Y_VER*8)+8)] = pixels_rgb[192+(l*8)+c];
                }
            }
        }
        else if (type == NO_SAMPLING){
            for (uint32_t l=0; l<8; l++) {
                for (uint32_t c=0; c<8; c++) {
                    image[(width*l)+c+((Y_HOR*8)*((int32_t)index%nb_mcu_horizontal))+(((int32_t)index/nb_mcu_horizontal) * width*(Y_VER*8))] = pixels_rgb[(l*8)+c];
                }
            }
        }

    } else {
        /* [BLACK AND WHITE] i.e FOR INVADER, GRIS and BISOU */
        for (uint8_t block=0; block<blocks_number; block++){
            for (uint32_t l=0; l<8; l++) {
                for (uint32_t c=0; c<8; c++) {
                    image[(width*l)+c+(8*((int32_t)index%nb_mcu_horizontal))+(((int32_t)index/nb_mcu_horizontal) * width*8 )] = components->Y[(block*64)+(l*8)+c];

                }
            }
        }
    }
}

void testconversion(){
    printf("%s\n", "conversion inner test");
}
