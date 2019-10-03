/**
* @file upsampling.c
* @author Odeh, Morin, Pouget
* @date 31 May 2019
* @copyright 2019 Odeh, Morin, Pouget
* @brief deal with upsamling
*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "jpeg_reader.h"
#include "upsampling.h"
#include "mcu.h"
#include "image.h"


enum sampling get_sampling_type(struct jpeg_desc* jdesc){
  /**
* @brief get sampling type
* @param [in] jdesc : contains information from jpeg  section headers
* @return type of sampling : horizontal, vertical, both or no sampling
* @details name type : NO_SAMPLING, SAMPLING_V, SAMPLING_H, SAMPLING_HV
*/

    uint8_t components_nb = get_nb_components(jdesc);
    if ( components_nb == 1){
        return NO_SAMPLING;
    }

    if ( components_nb == 3){
        uint8_t Y_HOR = get_frame_component_sampling_factor(jdesc, DIR_H, 0);
        uint8_t Y_VER = get_frame_component_sampling_factor(jdesc, DIR_V, 0);
        uint8_t CB_HOR = get_frame_component_sampling_factor(jdesc, DIR_H, 1);
        uint8_t CB_VER = get_frame_component_sampling_factor(jdesc, DIR_V, 1);
        uint8_t CR_HOR = get_frame_component_sampling_factor(jdesc, DIR_H, 2);
        uint8_t CR_VER = get_frame_component_sampling_factor(jdesc, DIR_V, 2);

        if (CB_HOR != CR_HOR || CB_VER != CR_VER){
            printf("ERROR SAMPLING FACTOER \n");
            return EXIT_FAILURE;
        }

        uint8_t Y_MULT = Y_HOR * Y_VER;
        uint8_t CB_MULT = CB_HOR * CB_VER;

        if (Y_MULT == CB_MULT){
            return NO_SAMPLING;
        }

        else if (Y_HOR == 2 && Y_VER == 2 && Y_MULT != CB_MULT) {
            return SAMPLING_HV;
        }

        else if (Y_HOR==CB_HOR) {
            return SAMPLING_V;
        }

        else if (Y_VER==CB_VER) {
            return SAMPLING_H;
        }

        else{
            return SAMPLING_NOT_FOUND;
        }
    }

    return SAMPLING_NOT_FOUND;
}

void upsampling_horizontal(struct components* components, struct components* new_components){

    uint16_t counter =0;

    for (uint8_t index=0; index < 64; index+=2){
        new_components->Cb[index] = components->Cb[counter];
        new_components->Cb[index+1] = components->Cb[counter];
        new_components->Cr[index] = components->Cr[counter];
        new_components->Cr[index+1] = components->Cr[counter];
        if ((counter+1)%8 == 4){
            counter+=4;
        }
        counter++;
    }
    counter = 4;
    for (uint8_t index=64; index < 128; index+=2){
        new_components->Cb[index] = components->Cb[counter];
        new_components->Cb[index+1] = components->Cb[counter];
        new_components->Cr[index] = components->Cr[counter];
        new_components->Cr[index+1] = components->Cr[counter];
        if ((counter+1)%8 == 0){
            counter+=4;
        }
        counter++;

    }
}


void upsampling_vertical(struct components* components, struct components* new_components){
  /**
  * @brief manage vertical upsampling for components
  * @param [in] components
  * @param [in] new_components : to get components after upsampling
  * @return components after vertical upsamling
  */

    for (uint8_t line=0; line<8; line++){
        for (uint8_t column=0; column<8; column++){
            new_components->Cb[line*16+column] = components->Cb[line*8+column];
            new_components->Cb[line*16+column+8] = components->Cb[line*8+column];
            new_components->Cr[line*16+column] = components->Cr[line*8+column];
            new_components->Cr[line*16+column+8] = components->Cr[line*8+column];

        }
    }
}

void upsampling_vertical_after_horizontal(struct components* components, struct components* components_hv){
  /**
  * @brief manage horizontal and vertical upsampling for components
  * @param [in] components
  * @param [in] components_hv : to get components after upsampling
  * @return components after horizontal and vertical upsamling
  */

    // for (uint8_t element=0; element<128; element++){
    for (uint16_t element=0; element<256; element++){
        components_hv->Y[element] = components->Y[element];
    }
    for (uint8_t line=0; line<4; line++){
        for (uint8_t column=0; column<8; column++){
            components_hv->Cb[line*16+column] = components->Cb[line*8+column];
            components_hv->Cb[line*16+column+8] = components->Cb[line*8+column];
            components_hv->Cb[64+line*16+column] = components->Cb[64+line*8+column];
            components_hv->Cb[64+line*16+column+8] = components->Cb[64+line*8+column];
            components_hv->Cr[line*16+column] = components->Cr[line*8+column];
            components_hv->Cr[line*16+column+8] = components->Cr[line*8+column];
            components_hv->Cr[64+line*16+column] = components->Cr[64+line*8+column];
            components_hv->Cr[64+line*16+column+8] = components->Cr[64+line*8+column];
        }
    }
    for (uint8_t line=0; line<4; line++){
        for (uint8_t column=0; column<8; column++){
            components_hv->Cb[128+line*16+column] = components->Cb[32+line*8+column];
            components_hv->Cb[128+line*16+column+8] = components->Cb[32+line*8+column];
            components_hv->Cb[192+line*16+column] = components->Cb[96+line*8+column];
            components_hv->Cb[192+line*16+column+8] = components->Cb[96+line*8+column];
            components_hv->Cr[128+line*16+column] = components->Cr[32+line*8+column];
            components_hv->Cr[128+line*16+column+8] = components->Cr[32+line*8+column];
            components_hv->Cr[192+line*16+column] = components->Cr[96+line*8+column];
            components_hv->Cr[192+line*16+column+8] = components->Cr[96+line*8+column];
        }
    }
}



struct components upsampling(struct mcu* mcu, struct jpeg_desc* jdesc){
  /**
* @brief realize upsambling for mcu
* @param [in] mcu
* @param [in] jdesc : contains information from jpeg  section headers
* @return components after upsampling
*/

    enum sampling type = get_sampling_type(jdesc);
    uint8_t components_number = get_nb_components(jdesc);

    struct components components_copy_from_mcu = convert_mcu_to_components(mcu, jdesc);
    struct components components = create_components(jdesc);

    uint8_t Y_HOR = get_frame_component_sampling_factor(jdesc, DIR_H, 0);
    uint8_t Y_VER = get_frame_component_sampling_factor(jdesc, DIR_V, 0);
    uint8_t blocks_number = Y_HOR*Y_VER;

    for (uint8_t block=0; block<blocks_number; block++){
        for (uint8_t element=0; element<64; element++){
            components.Y[block*64+element] = mcu->components.Y[block][element];
        }
    }

    if (type == SAMPLING_H){
        upsampling_horizontal(&components_copy_from_mcu, &components);
        free_components(&components_copy_from_mcu, jdesc);
        return components;
    }

    else if (type == SAMPLING_V){
        upsampling_vertical(&components_copy_from_mcu, &components);
        free_components(&components_copy_from_mcu, jdesc);
        return components;
    }
    else if (type == SAMPLING_HV){
        upsampling_horizontal(&components_copy_from_mcu, &components);
        free_components(&components_copy_from_mcu, jdesc);
        struct components components_hv = create_components(jdesc);
        upsampling_vertical_after_horizontal(&components, &components_hv);
        free_components(&components, jdesc);
        return components_hv;
    }
    else if (type == NO_SAMPLING){
        if ( components_number == 3){
            for (uint8_t element=0; element<64; element++){
                components.Cb[element] = components_copy_from_mcu.Cb[element];
                components.Cr[element] = components_copy_from_mcu.Cr[element];
            }
        }
        free_components(&components_copy_from_mcu, jdesc);
        return components;
    }

    return components;
}

void testupsampling(){
    printf("%s\n", "upsampling inner test");
}
