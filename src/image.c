/**
* @file image.c
* @author Odeh, Morin, Pouget
* @date 31 May 2019
* @copyright 2019 Odeh, Morin, Pouget
* @brief define parameters of an image
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "image.h"
#include "jpeg_reader.h"


uint16_t get_pixels_nb_in_direction(struct jpeg_desc* jdesc, enum direction DIR){
  /**
* @brief get length in a specific direction
* @param [in]  jdesc  : contains information from jpeg  section headers
* @param [in]  DIR  :   DIR_H,DIR_V, DIR_NB (sentinelle)
* @return length in a specific direction
* @details manage case when dimension can not be divided by 8
*/

    uint16_t length = get_image_size(jdesc, DIR);
    uint8_t length_mcu = get_frame_component_sampling_factor(jdesc, DIR, 0);
    uint16_t sampling_fact = length_mcu*8;

    // normal case : the dimension is multiple of sampling_fact
    if (length % sampling_fact == 0){
        return length;
    }else{
        // if the dimension is not multiple of fact
        return ((length/sampling_fact) + 1) * sampling_fact;
    }
}


uint32_t get_mcu_number_in_image(struct jpeg_desc* jdesc){
/**
* @brief get mcu number in image
* @param [in] jdesc : contains information from jpeg  section headers
* @return mcu numbers
*/

    uint16_t height = get_pixels_nb_in_direction(jdesc, DIR_V);
    uint16_t width = get_pixels_nb_in_direction(jdesc, DIR_H);

    uint16_t height_mcu = get_frame_component_sampling_factor(jdesc, DIR_V, 0);
    uint16_t width_mcu = get_frame_component_sampling_factor(jdesc, DIR_H, 0);

    uint32_t nb_mcu = (height*width)/(height_mcu*width_mcu*64);

    return nb_mcu;
}


uint32_t *allocate_image(struct jpeg_desc *jdesc){
  /**
* @brief allocate image (malloc) according to its width and height
* @param [in] jdesc : contains information from jpeg  section headers
* @return malloc for image according to its width and height
*/

    uint16_t width = get_pixels_nb_in_direction(jdesc, DIR_H);
    uint16_t height = get_pixels_nb_in_direction(jdesc, DIR_V);

    return malloc(height*width*sizeof(uint32_t));
}


void testimage(){
    printf("%s\n", "mcu inner test");
}
