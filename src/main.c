/**
* @file main.c
* @author Odeh, Morin, Pouget
* @date 31 May 2019
* @copyright 2019 Odeh, Morin, Pouget
* @brief perform steps of transformation for a given jpeg file
*
* \mainpage JPEG Decodeur
* \section usage_sec Usage
* JPEG Decodeur is a program to decode jpeg images into ppm or pgm images, developed under Ubuntu and CentOS.
* To execute and run the program and tests simply open a terminal in its directory and type:
* \code
* make
* bin/jpeg2ppm image.jpeg\endcode
* for individual tests
* \code
* make tests
* bin/test_name_here \endcode
* To lunch the decodor on all images in images directoy you can use the shell program \b run_program in /scripts directory by typing in terminal in main directoy
* \code scripts/run_decoder \endcode
* To lunch all tests programs you can use the shell program \b run_tests in /scripts directory by typing in terminal in main directoy
* \code scripts/run_tests \endcode
* To lunch valgrind on all images in /images directory you can use the shell program \b run_valgrind in /scripts directory by typing in terminal in main directoy
* \code scripts/run_valgrind \endcode
* copyright © 2019 Lucas MORIN | Majd ODEH | Sylvain POUGET \n
* École nationale supérieure d'informatique et de mathématiques appliquées | \b Grenoble-INP \b ENSIMAG
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "jpeg_reader.h"
#include "bitstream.h"
#include "extracteur.h"
#include "mcu.h"
#include "ppm.h"
#include "conversion.h"
#include "iqzz.h"
#include "idct.h"
#include "rgb.h"
#include "image.h"


/*
 * Check if a file exist using fopen() function
 * return 1 if the file exist otherwise return 0
 */
static uint8_t check_file(const char * filename){
    /* try to open file to read */
    FILE *file;
    if ((file = fopen(filename, "r"))){
        fclose(file);
        return 1;
    }
    return 0;
}


int main(int argc, char **argv){

    if (argc != 2) {
		// error if image name not provided
		fprintf(stderr, "Usage: %s fichier.jpeg\n", argv[0]);
		return EXIT_FAILURE;
    }

    if (check_file(argv[1]) == 0){
        // error if image not found or doesn't exist
        fprintf(stderr, "ERROR IN MAIN : File not found or doesn't exist !\n");
        exit(EXIT_FAILURE);

    }

    // calculate execution time
    clock_t start = clock();
    clock_t process_start_time = start;

    // get image name
    const char *filename = argv[1];

    // create a jpeg_desc struct from image
    struct jpeg_desc *jdesc = read_jpeg(filename);

    // create a bitstream from the jpeg_desc
    struct bitstream *stream = get_bitstream(jdesc);

    // get the number of MCU in the image
    uint32_t mcu_nb = get_mcu_number_in_image(jdesc);
    // printf("Number of MCU in the image is %u\n \n", mcu_nb);

    // reserve an array for the image
    uint32_t * image = allocate_image(jdesc);

    // Get the sampling type (Horizontal, vertical...etc)
    enum sampling type = get_sampling_type(jdesc);

    /* For Terminal printing only */
    uint16_t percentage = mcu_nb/40;
    uint16_t counter = percentage;

    /* print progress bar loading in terminal */
    print_loading_terminal();

    /* define cos_sin_table, used to increase idct performances */

    float *cos_sin_table = malloc(6 * sizeof(float));
    define_cos_sin_table(cos_sin_table);

    for (uint32_t index=0; index<mcu_nb; index++){

        // progress bar in terminal
        print_progress_terminal(index, &counter, percentage);

        // get MCU from stream
        struct mcu mcu = extractor(jdesc, stream);
        update_process_time(&process_start_time,0);

        // Quantification inverse, ZigZag inverse and iDCT
        iqzz_idct(&mcu, jdesc, cos_sin_table);
        update_process_time(&process_start_time,1);

        // Upsampling
        struct components components = upsampling(&mcu, jdesc);
        update_process_time(&process_start_time,2);

        // Convert YCbCr to RGB and arrange pixels in image
        arrange_pixels_in_image(&components, jdesc, image, type, index);
        update_process_time(&process_start_time,3);

        // free MCU from memory
        free_mcu(mcu);
        free_components(&components, jdesc);

    }


    // troncature
    uint32_t *original_image = troncature(image, jdesc);
    update_process_time(&process_start_time,4);

    // free image array from memory
    free(image);

    // free tables
    free(cos_sin_table);

    // create ppm or pgm image
    create_image(original_image, jdesc);
    free(original_image);

    // clear jdesc
    close_jpeg(jdesc);

    // print execution time
    clock_t end = clock();
    print_execution_time(start, end);
    update_process_time(&process_start_time,5);

    // print [FINISHED WITH SUCCESS]
    print_finished_terminal();

    return EXIT_SUCCESS;
}
