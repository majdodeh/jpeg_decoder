/**
* @file jpeg_reader.c
* @author Odeh, Morin, Pouget
* @date 7 Jun 2019
* @copyright 2019 Odeh, Morin, Pouget
* @brief implement our own jpeg_reader
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>

#include "jpeg_reader.h"
#include "bitstream.h"
#include "huffman.h"

#define MAX_NB_OF_QTABLES 4
#define MAX_NB_OF_HTABLES 4


struct component_desc{
  /**
* @brief components description
*/
    uint8_t component_id;
    uint8_t h_sampling_factor;
    uint8_t v_sampling_factor;
    uint8_t quant_index;
    uint8_t huff_index_DC;
    uint8_t huff_index_AC;
};


struct jpeg_desc{
  /**
* @brief jpeg description
*/

    char* filename;
    struct bitstream* stream;

    uint16_t height;
    uint16_t width;

    uint8_t nb_huff_table_DC ;
    uint8_t nb_huff_table_AC ;
    struct huff_table *huff_table_DC[MAX_NB_OF_HTABLES];        // array of all DC huffman tables in file
    struct huff_table  *huff_table_AC[MAX_NB_OF_HTABLES];       // array of all AC huffman tables in file

    uint8_t nb_quant_table;
    uint8_t *quant_table[MAX_NB_OF_QTABLES];         // array of all quantization tables in file

    uint8_t nb_components;
    struct component_desc *components[3];           // array of descriptor of all components in file

    uint8_t first_id_in_scan;
    uint8_t second_id_in_scan;
    uint8_t third_id_in_scan;
};


void read_SOFx(struct jpeg_desc *jdesc, uint32_t *buffer){
  /**
* @brief read header information from jpeg image
* @param [in] jdesc
* @param [in] buffer
*/

    // In this section, the order of appartion of components is always the same : Y, Cb, Cr
    read_bitstream(jdesc->stream, 24, buffer, false);
    read_bitstream(jdesc->stream, 16, buffer, false);
    jdesc->height = *buffer;
    if (jdesc->height <= 0){
        fprintf(stderr, "ERROR IN JPEG_READER : invalid height\n");
        exit(EXIT_FAILURE);
    }
    read_bitstream(jdesc->stream, 16, buffer, false);
    jdesc->width = *buffer;
    if (jdesc->width <= 0){
        fprintf(stderr, "ERROR IN JPEG_READER : invalid width\n");
        exit(EXIT_FAILURE);
    }
    read_bitstream(jdesc->stream, 8, buffer, false);
    jdesc->nb_components = *buffer;
    if (jdesc->nb_components > 3){
        fprintf(stderr, "ERROR IN JPEG_READER : number of components greater than 3\n");
        exit(EXIT_FAILURE);
    }
    for (uint8_t index = 0; index < jdesc->nb_components; index++) {
            struct component_desc *component = malloc(sizeof(struct component_desc));
            read_bitstream(jdesc->stream, 8, buffer, false);
            component->component_id = *buffer;
            read_bitstream(jdesc->stream, 4, buffer, false);
            component->h_sampling_factor = *buffer;
            if (component->h_sampling_factor != 1 && component->h_sampling_factor != 2){
                fprintf(stderr, "ERROR IN JPEG_READER : horizontal sampling factor is different than 1 or 2\n");
                exit(EXIT_FAILURE);
            }
            read_bitstream(jdesc->stream, 4, buffer, false);
            component->v_sampling_factor = *buffer;
            if (component->v_sampling_factor != 1 && component->v_sampling_factor != 2){
                fprintf(stderr, "ERROR IN JPEG_READER : vertical sampling factor is different than 1 or 2\n");
                exit(EXIT_FAILURE);
            }
            read_bitstream(jdesc->stream, 8, buffer, false);
            component->quant_index = *buffer;
            if (component->quant_index > 3){
                fprintf(stderr, "ERROR IN JPEG_READER : invalid quantization index\n");
                exit(EXIT_FAILURE);
            }
            jdesc->components[index] = component;
    }
}


void read_DQT(struct jpeg_desc *jdesc, uint32_t *buffer, uint8_t *length){
  /**
* @brief read quantification tables
* @param [in] jdesc
* @param [in] buffer
* @param [in] length
*/

    read_bitstream(jdesc->stream, 16, buffer, false);
    *length = *buffer;
    *length -= 2;
    uint8_t nb_of_tables = *length/65;
    jdesc->nb_quant_table += nb_of_tables;
    if (nb_of_tables > 4){
        fprintf(stderr, "ERROR IN JPEG_READER : invalid quantization index\n");
        exit(EXIT_FAILURE);
    }
    for (uint8_t i = 0; i < nb_of_tables; i++) {
        uint8_t* quant_table = malloc(64*sizeof(uint8_t));
        read_bitstream(jdesc->stream, 4, buffer, false);
        read_bitstream(jdesc->stream, 4, buffer, false);
        uint8_t index = *buffer;
        if (index > 3){
            fprintf(stderr, "ERROR IN JPEG_READER : invalid quantization index\n");
            exit(EXIT_FAILURE);
        }
        for(uint8_t index = 0; index < 64 ; index++) {
            read_bitstream(jdesc->stream, 8, buffer, false);
            quant_table[index] = *buffer;
        }
        jdesc->quant_table[index] = quant_table;
    }
}


void read_DHT(struct jpeg_desc *jdesc, uint32_t *buffer){
  /**
* @brief read Huffman informations
* @param [in] jdesc
* @param [in] buffer
*/

    read_bitstream(jdesc->stream, 16, buffer, false);
    uint16_t length = *buffer - 2;
    uint16_t bytes_read = 0;
    while (bytes_read < length) {
        read_bitstream(jdesc->stream, 3, buffer, false);
        read_bitstream(jdesc->stream, 1, buffer, false);
        enum acdc type = *buffer;
        if (type != 1 && type != 0) {
            fprintf(stderr, "ERROR IN JPEG_READER : incorrect type of huffman table\n");
            exit(EXIT_FAILURE);
        }
        uint16_t i = 0;
        uint16_t *nb_byte_read = &i;
        if (type == DC){
            jdesc->nb_huff_table_DC += 1;
            read_bitstream(jdesc->stream, 4, buffer, false);
            bytes_read += 1;
            uint8_t index = *buffer;
            if (index > 3){
                fprintf(stderr, "ERROR IN JPEG_READER : invalid DC huffman index\n");
                exit(EXIT_FAILURE);
            }
            jdesc->huff_table_DC[index] = load_huffman_table(jdesc->stream, nb_byte_read);
            bytes_read += *nb_byte_read;
        }
        else{
            jdesc->nb_huff_table_AC += 1;
            read_bitstream(jdesc->stream, 4, buffer, false);
            bytes_read += 1;
            uint8_t index = *buffer;
            if (index > 3){
                fprintf(stderr, "ERROR IN JPEG_READER : invalid AC huffman index\n");
                exit(EXIT_FAILURE);
            }
            *nb_byte_read = 0;
            jdesc->huff_table_AC[index] = load_huffman_table(jdesc->stream, nb_byte_read);
            bytes_read += *nb_byte_read;
        }
    }
}


void read_SOS(struct jpeg_desc *jdesc, uint32_t *buffer, uint8_t *length){
  /**
* @brief read image data
* @param [in] jdesc
* @param [in] buffer
* @param [in] length
*/

    read_bitstream(jdesc->stream, 16, buffer, false);
    *length = *buffer;
    read_bitstream(jdesc->stream, 8, buffer, false);
    for (uint8_t i = 0; i < jdesc->nb_components; i++) {
        uint32_t tmp;
        read_bitstream(jdesc->stream, 8, &tmp, false);
        if (tmp > 255) {
            fprintf(stderr, "ERROR IN JPEG_READER : invalid id for components\n");
            exit(EXIT_FAILURE);
        }
        //first_id_in_scan store the id on the first component appearing in scan
        if (i == 0){
            jdesc->first_id_in_scan = tmp;
        }
        //second_id_in_scan store the id on the second component appearing in scan
        else if (i == 1){
            jdesc->second_id_in_scan = tmp;
        }
        //third_id_in_scan store the id on the third component appearing in scan
        else{
            jdesc->third_id_in_scan = tmp;
        }
        for (size_t index = 0; index < jdesc->nb_components; index++) {
            if (tmp == jdesc->components[index]->component_id){
                read_bitstream(jdesc->stream, 4, buffer, false);
                if (*buffer > 3){
                    fprintf(stderr, "ERROR IN JPEG_READER : invalid DC huffman index\n");
                    exit(EXIT_FAILURE);
                }
                jdesc->components[index]->huff_index_DC = *buffer;
                read_bitstream(jdesc->stream, 4, buffer, false);
                if (*buffer > 3){
                    fprintf(stderr, "ERROR IN JPEG_READER : invalid AC huffman index\n");
                    exit(EXIT_FAILURE);
                }
                jdesc->components[index]->huff_index_AC = *buffer;
            }
        }
    }
    read_bitstream(jdesc->stream, 24, buffer, false);
}

void read_jfif(struct jpeg_desc *jdesc, uint32_t *buffer){
  /**
* @brief check if jfif is in header
* @param [in] jdesc
* @param [in] buffer
*/

    read_bitstream(jdesc->stream, 16, buffer, false);
    read_bitstream(jdesc->stream, 8, buffer, false);
    if (*buffer != 74){
        fprintf(stderr, "ERROR IN JPEG_READER : application APP is not handled by this decorder : JFIF missing in header\n");
        exit(EXIT_FAILURE);
    }
    read_bitstream(jdesc->stream, 8, buffer, false);
    if (*buffer != 70){
        fprintf(stderr, "ERROR IN JPEG_READER : application APP is not handled by this decorder : JFIF missing in header\n");
        exit(EXIT_FAILURE);
    }
    read_bitstream(jdesc->stream, 8, buffer, false);
    if (*buffer != 73){
        fprintf(stderr, "ERROR IN JPEG_READER : application APP is not handled by this decorder : JFIF missing in header\n");
        exit(EXIT_FAILURE);
    }
    read_bitstream(jdesc->stream, 8, buffer, false);
    if (*buffer != 70){
        fprintf(stderr, "ERROR IN JPEG_READER : application APP is not handled by this decorder : JFIF missing in header\n");
        exit(EXIT_FAILURE);
    }
    read_bitstream(jdesc->stream, 8, buffer, false);
    if (*buffer != 00){
        fprintf(stderr, "ERROR IN JPEG_READER : application APP is not handled by this decorder : JFIF missing in header\n");
        exit(EXIT_FAILURE);
    }
}

void initialize(struct jpeg_desc *jdesc, const char *filename){
  /**
* @brief initialization of components
* @param [in] jdesc
* @param [in] filename
*/

    jdesc->filename = (char*)filename;
    jdesc->nb_huff_table_AC = 0;
    jdesc->nb_huff_table_DC = 0;
    for(uint8_t index = 0; index < MAX_NB_OF_HTABLES; index++){
        jdesc->huff_table_AC[index] = NULL;
        jdesc->huff_table_DC[index] = NULL;
    }
    for(uint8_t index = 0; index < MAX_NB_OF_QTABLES; index++){
        jdesc->quant_table[index] = NULL;
    }
    for(uint8_t index = 0; index < 3; index++){
        jdesc->components[index] = NULL;
    }
    jdesc->nb_quant_table = 0;
    struct bitstream *stream;
    stream = create_bitstream(filename);
    jdesc->stream = stream;
}

struct jpeg_desc *read_jpeg(const char *filename){
  /**
* @brief read header data
* @param [in] filename
*/
    // allocate space for jpeg descriptor
    struct jpeg_desc* jdesc = malloc(sizeof(struct jpeg_desc));
    // initialization of jdesc descriptor's attributes
    initialize(jdesc, filename);
    uint8_t length;
    uint32_t buffer;

    skip_bitstream_until(jdesc->stream, 0xe0);
    read_bitstream(jdesc->stream, 8, &buffer, false);
    // buffer is initialized to 0xe0
    bool end = false;

    while(!end){
        // the value of the current marker tells which section has to be readen
        switch (buffer) {
            case 0xe0:
            read_jfif(jdesc, &buffer);
            // skip bytes until next marker
            skip_bitstream_until(jdesc->stream, 0xff);
            read_bitstream(jdesc->stream, 8, &buffer, false);
            read_bitstream(jdesc->stream, 8, &buffer, false);
            break;

            case 0xfe:
            skip_bitstream_until(jdesc->stream, 0xff);
            read_bitstream(jdesc->stream, 8, &buffer, false);
            read_bitstream(jdesc->stream, 8, &buffer, false);
            break;

            case 0xdb:
            read_DQT(jdesc, &buffer, &length);
            skip_bitstream_until(jdesc->stream, 0xff);
            read_bitstream(jdesc->stream, 8, &buffer, false);
            read_bitstream(jdesc->stream, 8, &buffer, false);
            break;

            case 0xc0:
            read_SOFx(jdesc, &buffer);
            skip_bitstream_until(jdesc->stream, 0xff);
            read_bitstream(jdesc->stream, 8, &buffer, false);
            read_bitstream(jdesc->stream, 8, &buffer, false);
            break;

            case 0xc4:
            read_DHT(jdesc, &buffer);
            skip_bitstream_until(jdesc->stream, 0xff);
            read_bitstream(jdesc->stream, 8, &buffer, false);
            read_bitstream(jdesc->stream, 8, &buffer, false);
            break;

            case 0xda:
            read_SOS(jdesc, &buffer, &length);
            end = true;
            break;

            default:
            // if a marker is not recognized, the program is stopped
            fprintf(stderr, "ERROR IN JPEG_READER : marker not recognized \n");
            exit(EXIT_FAILURE);
            break;
        }
    }
    return jdesc;
}

void close_jpeg(struct jpeg_desc *jdesc) {
  /**
* @brief close jpeg
* @param [in] jdesc
*/

    for(uint8_t index = 0; index < MAX_NB_OF_HTABLES; index++){
        if(jdesc->huff_table_AC[index] != NULL){
            free_huffman_table(jdesc->huff_table_AC[index]);
        }
        if(jdesc->huff_table_DC[index] != NULL){
            free_huffman_table(jdesc->huff_table_DC[index]);
        }
    }
    for(uint8_t index = 0; index < MAX_NB_OF_QTABLES; index++){
        if(jdesc->quant_table[index] != NULL){
            free(jdesc->quant_table[index]);
        }
    }
    for(uint8_t index = 0; index < jdesc->nb_components; index++){
        if(jdesc->components[index] != NULL){
            free(jdesc->components[index]);
        }
    }
    close_bitstream(jdesc->stream);
    free(jdesc);
}

char *get_filename(const struct jpeg_desc *jdesc){
  /**
* @brief get filenme
* @param [in] jdesc
* @return filename
*/

    return jdesc->filename;
}


struct bitstream *get_bitstream(const struct jpeg_desc *jdesc){
  /**
* @brief get bitstream
* @param [in] jdesc
* @return stream
*/

    return jdesc->stream;
}



uint8_t get_nb_quantization_tables(const struct jpeg_desc *jdesc){
  /**
* @brief get number of quantization tables
* @param [in] jdesc
* @return number of quantization tables
*/

    return jdesc->nb_quant_table;
}
uint8_t *get_quantization_table(const struct jpeg_desc *jdesc, uint8_t index){
  /**
* @brief get quantization tables
* @param [in] jdesc
* @return quantization tables
*/
    return jdesc->quant_table[index];
}



uint8_t get_nb_huffman_tables(const struct jpeg_desc *jdesc, enum acdc acdc){
/**
* @brief get number of huffman tables
* @param [in] jdesc
* @return number of huffman tables
*/

    if(acdc == AC){
        return jdesc->nb_huff_table_AC;
    }
    else{
        return jdesc->nb_huff_table_DC;
    }
}


struct huff_table *get_huffman_table(const struct jpeg_desc *jdesc,enum acdc acdc, uint8_t index){
  /**
  * @brief get  huffman tables
  * @param [in] jdesc
  * @return huffman tables
  */
    if(acdc == AC){
        return jdesc->huff_table_AC[index];
    }
    else{
        return jdesc->huff_table_DC[index];
    }
}


uint16_t get_image_size(struct jpeg_desc *jdesc, enum direction dir){
    if(dir == DIR_H){
        return jdesc->width;
    }
    else{
        return jdesc->height;
    }
}

uint8_t get_nb_components(const struct jpeg_desc *jdesc){
/**
* @brief get number of components
* @param [in] jdesc
* @return number of components
*/

    return jdesc->nb_components;
}

uint8_t get_frame_component_id(const struct jpeg_desc *jdesc, uint8_t frame_comp_index){
  /**
* @brief get frame component id
* @param [in] jdesc
* @param [in] frame_comp_index
* @return frame component id
*/
    return (jdesc->components[frame_comp_index]->component_id);

}

uint8_t get_frame_component_sampling_factor(const struct jpeg_desc *jdesc, enum direction dir, uint8_t frame_comp_index){
  /**
* @brief  get frame component sampling factor
* @param [in] jdesc
* @param [in] dir
* @param [in] frame_comp_index
* @return frame component sampling factor
*/
    if(dir == DIR_H){
        return jdesc->components[frame_comp_index]->h_sampling_factor;
    }
    else{
        return jdesc->components[frame_comp_index]->v_sampling_factor;
    }
}
uint8_t get_frame_component_quant_index(const struct jpeg_desc *jdesc, uint8_t frame_comp_index){
  /**
* @brief get frame component quant index
* @param [in] jdesc
* @param [in] frame_comp_index
* @return get frame component quant index
*/

    return (jdesc->components[frame_comp_index]->quant_index);


}

uint8_t get_scan_component_id(const struct jpeg_desc *jdesc, uint8_t scan_comp_index){
/**
* @brief get scan component id
* @param [in] jdesc
* @param [in] scan_comp_index
* @return scan component id
*/

    if(scan_comp_index == 0){
        return jdesc->first_id_in_scan;
    }
    else if(scan_comp_index == 1){
        return jdesc->second_id_in_scan;
    }
    else{
        return jdesc->third_id_in_scan;
    }

}
uint8_t get_scan_component_huffman_index(const struct jpeg_desc *jdesc,enum acdc acdc, uint8_t scan_comp_index){
  /**
* @brief get scan component huffman index
* @param [in] jdesc
* @param [in] acdc
* @param [in] scan_comp_index
* @return scan component huffman index
*/

    return (jdesc->components[scan_comp_index]->quant_index);
    if(scan_comp_index == 0){
        if(acdc == AC){
            return (jdesc->components[jdesc->first_id_in_scan]->huff_index_AC);
        }
        else{
            return (jdesc->components[jdesc->first_id_in_scan]->huff_index_DC);
        }
    }
    else if(scan_comp_index == 1){
        if(acdc == AC){
            return (jdesc->components[jdesc->second_id_in_scan]->huff_index_AC);
        }
        else{
            return (jdesc->components[jdesc->second_id_in_scan]->huff_index_DC);
        }
    }
    else{
        if(acdc == AC){
            return (jdesc->components[jdesc->third_id_in_scan]->huff_index_AC);
        }
        else{
            return (jdesc->components[jdesc->third_id_in_scan]->huff_index_DC);
        }
    }
}
