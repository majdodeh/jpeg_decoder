/**
* @file rgb.c
* @author Odeh, Morin, Pouget
* @date 31 May 2019
* @copyright 2019 Odeh, Morin, Pouget
* @brief manage rgb conversion and get rgb values
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <math.h>
#include "rgb.h"
#include "mcu.h"
#include "jpeg_reader.h"


uint8_t fixed_values_rgb(float value){
  /**
* @brief manage cases when value is not between 0 and 255
* @param [in] value
* @return 0, 255 or value
*/

    float value_tmp = roundf(value);
    if (value > 255){
        value_tmp = 255;
    }
    if (value < 0){
        value_tmp = 0;
    }
    return (uint8_t)value_tmp;
}


uint8_t *convert_to_rgb(uint8_t y, uint8_t Cb, uint8_t Cr)
{/**
* @brief convert int representation to rgb system
* @param [in] y : as an uint8_t
* @param [in] Cb : as an uint8_t
* @param [in] Cr : as an uint8_t
* @return rgb containing 3 elements for red blue and green
*/

    uint8_t *rgb = malloc(3 * sizeof(uint8_t));
    float red = y - 0.0009267*(Cb - 128) + 1.4016868*(Cr - 128);
    float blue = y - 0.3436954*(Cb - 128) - 0.7141690*(Cr - 128);
    float green = y + 1.7721604*(Cb - 128) + 0.0009902*(Cr - 128);
    rgb[0] = fixed_values_rgb(red);
    rgb[1] = fixed_values_rgb(blue);
    rgb[2] = fixed_values_rgb(green);
    return rgb;
}


uint32_t *convert_to_rgb_tab(uint8_t *y, uint8_t *Cb, uint8_t *Cr){
/**
* @brief convert int representation to rgb system in a tab
* @param [in] y : as an uint8_t
* @param [in] Cb : as an uint8_t
* @param [in] Cr : as an uint8_t
* @return tab which any element contains red, blue and green information
*/
    uint32_t *rgb_t = malloc(sizeof(uint8_t));

    for (size_t index = 0; index < 64; index++) {
        uint8_t *rgb = convert_to_rgb(y[index], Cb[index], Cr[index]);
        rgb_t[index] = (rgb[0] << 16) + (rgb[1] << 8) + rgb[2];
    }
    return rgb_t;

}


void convert_components_to_rgb(struct components* components, uint32_t *pixels_rgb, struct jpeg_desc* jdesc){
  /**
  * @brief convert components representation to rgb system in a tab
  * @param [in] components
  * @param [in] jdesc : contains information from jpeg  section headers
  * @param [in] pixels_rgb : tab for pixels
  * @return tab which any element contains red, blue and green information
  */
    uint8_t Y_HOR = get_frame_component_sampling_factor(jdesc, DIR_H, 0);
    uint8_t Y_VER = get_frame_component_sampling_factor(jdesc, DIR_V, 0);
    uint8_t blocks_number = Y_HOR*Y_VER;
    float red, blue, green;
    int16_t y, Cb, Cr;

    for (uint8_t block=0; block<blocks_number; block++){
        for (uint8_t index = 0; index < 64; index++) {
            y = components->Y[block*64+index];
            Cb = components->Cb[block*64+index];
            Cr = components->Cr[block*64+index];
            red = (float)y - 0.0009267*((float)Cb - 128) + 1.4016868*((float)Cr - 128);
            blue = (float)y - 0.3436954*((float)Cb - 128) - 0.7141690*((float)Cr - 128);
            green = (float)y + 1.7721604*((float)Cb - 128) + 0.0009902*((float)Cr - 128);
            uint8_t rgb_r = fixed_values_rgb(red);
            uint8_t rgb_g = fixed_values_rgb(blue);
            uint8_t rgb_b = fixed_values_rgb(green);
            pixels_rgb[block*64+index] = (rgb_r << 16) + (rgb_g << 8) + rgb_b;
        }
    }
}
