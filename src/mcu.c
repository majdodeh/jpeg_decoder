/**
* @file mcu.c
* @author Odeh, Morin, Pouget
* @date 31 May 2019
* @copyright 2019 Odeh, Morin, Pouget
* @brief define parameters of a mcu
*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mcu.h"
#include "jpeg_reader.h"


uint8_t free_mcu(struct mcu mcu){
/**
* @brief free memory for a mcu
* @param [in] mcu
* @return no return
*/
    int16_t **ptcompos[3] = {mcu.components.Y, mcu.components.Cb, mcu.components.Cr};
    for (uint8_t i = 0; i < mcu.nb_components; i++) {
        for (uint8_t j = 0; j < mcu.size_components[i]; j++) {
            free(ptcompos[i][j]);
        }
        free(ptcompos[i]);
    }
    free(mcu.size_components);
    return 0;
}

void free_components(struct components *components, struct jpeg_desc* jdesc){
  /**
* @brief free memory for components
* @param [in] components
* @return no return
*/
    uint8_t nbr_of_comps = get_nb_components(jdesc);

    free(components->Y);
    if (nbr_of_comps == 3){
        free(components->Cb);
        free(components->Cr);
    }
}

struct components convert_mcu_to_components(struct mcu* mcu, struct jpeg_desc* jdesc){
  /**
* @brief convert mcu to components
* @param [in] mcu
* @param [in] jdesc :  contains information from jpeg  section headers
* @return components
*/

    struct components comp;

    uint8_t Y_HOR = get_frame_component_sampling_factor(jdesc, DIR_H, 0);
    uint8_t Y_VER = get_frame_component_sampling_factor(jdesc, DIR_V, 0);
    // uint8_t C_HOR = get_frame_component_sampling_factor(jdesc, DIR_H, 1);
    // uint8_t C_VER = get_frame_component_sampling_factor(jdesc, DIR_V, 1);

    uint8_t nbr_of_comps = get_nb_components(jdesc);
    // printf("nbr_of_comps %u\n", nbr_of_comps);

    comp.Y = malloc(Y_HOR*Y_VER*64*sizeof(uint16_t));
    comp.Cb = NULL;
    comp.Cr = NULL;

    if ( nbr_of_comps == 3){
        comp.Cb = malloc(Y_HOR*Y_VER*64*sizeof(uint16_t));
        comp.Cr = malloc(Y_HOR*Y_VER*64*sizeof(uint16_t));
    }

    // uint8_t nbr_of_blocks_Y = Y_HOR*Y_VER;
    // uint8_t nbr_of_blocks_C = C_HOR*_VER;
    // printf("mcu->size_components[0] %u\n", mcu->size_components[0]);
    for (uint8_t block=0; block<mcu->size_components[0]; block++){
        for (uint8_t element=0; element<64; element++){
            comp.Y[block*64+element] = mcu->components.Y[block][element];
        }
    }
    // printf("yyyyy \n");
    // printf("mcu->size_components[1] %u\n", mcu->size_components[1]);
    if (nbr_of_comps == 3){
        for (uint8_t block=0; block<mcu->size_components[1]; block++){
            for (uint8_t element=0; element<64; element++){
                comp.Cb[block*64+element] = mcu->components.Cb[block][element];
                comp.Cr[block*64+element] = mcu->components.Cr[block][element];
            }
        }
        // printf("ccccccccc \n");
    }
    return comp;
}

struct components create_components(struct jpeg_desc* jdesc){
  /**
* @brief create component given
* @param [in] mcu
* @param [in] jdesc :  contains information from jpeg section headers
* @return components
*/
    struct components comp;

    uint8_t Y_HOR = get_frame_component_sampling_factor(jdesc, DIR_H, 0);
    uint8_t Y_VER = get_frame_component_sampling_factor(jdesc, DIR_V, 0);

    uint8_t nbr_of_comps = get_nb_components(jdesc);

    comp.Y = malloc(Y_HOR*Y_VER*64*sizeof(uint16_t));
    if ( nbr_of_comps == 3){
        comp.Cb = malloc(Y_HOR*Y_VER*64*sizeof(uint16_t));
        comp.Cr = malloc(Y_HOR*Y_VER*64*sizeof(uint16_t));
    }

    // valgrind --track-origins=yes bin/jpeg2ppm images/biiiiiig.jpg

    return comp;
}

void testmcu(){
    printf("%s\n", "mcu inner test");
}
