/**
* @file ppm.c
* @author Odeh, Morin, Pouget
* @date 31 May 2019
* @copyright 2019 Odeh, Morin, Pouget
* @brief define ppm and pgm parameters and create animation for loading
*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "jpeg_reader.h"
#include "ppm.h"



void create_ppm_header(FILE * file, uint16_t height, uint16_t width, uint16_t max_comp){
	/**
* @brief create ppm header given international standards
* @param [in] file
* @param [in] height : height of examinated image
* @param [in] width : width of examinated image
* @param [in] max_comp
* @return printing informations in the given file
*/

	fprintf(file, "P6\n");
	fprintf(file, "%u %u\n", width, height);
	fprintf(file, "%i\n", max_comp);
}

void create_pgm_header(FILE * file, uint16_t height, uint16_t width, uint16_t max_comp){
	/**
* @brief create pgm header given international standards
* @param [in] file
* @param [in] height : height of examinated image
* @param [in] width : width of examinated image
* @param [in] max_comp
* @return printing informations in the given file
*/
	fprintf(file, "P5\n");
	fprintf(file, "%u %u\n", width, height);
	fprintf(file, "%i\n", max_comp);
}

void create_pgm_body(FILE *file, uint32_t *image, uint16_t height, uint16_t width){
	/**
* @brief create pgm body given international standards
* @param [in] file
* @param [in] image : image as an array
* @param [in] height : height of examinated image
* @param [in] width : width of examinated image
* @return printing informations in the given file
*/
    // uint32_t *image_copy = image;
    // create_pgm_header(file, height, width, 255);
    for(uint32_t index = 0; index < height * width; index++) {
        uint8_t color = image[index] & 0xFF;
        uint8_t colors[] = {color};
        fwrite(colors, 1, 1, file);
        // image_copy++;
    }
}

void create_ppm_body(FILE *file, uint32_t *image, uint16_t height, uint16_t width){
	/**
* @brief create ppm body given international standards
* @param [in] file
* @param [in] image : image as an array
* @param [in] height : height of examinated image
* @param [in] width : width of examinated image
* @return printing informations in the given file
*/
    // uint32_t *image_copy = image;
    // create_ppm_header(file, height, width, 255);
	for(uint32_t index = 0; index < height * width; index++) {
		uint8_t red = (image[index] >> 16) & 0xFF;
		uint8_t green = (image[index] >> 8) & 0xFF;
		uint8_t blue = image[index] & 0xFF;
		uint8_t colors[] = {red, green, blue};
		fwrite(colors, 1, 3, file);
	}
}

char *change_extension(char *file_name, char *extension){
	// calculate the name length without the last letter (\0)
    int8_t file_length = strlen(file_name) - 1;
    int8_t extension_length = strlen(extension);
    // read from the end until we find a point (.) = 46 in ASCII
    while (file_length >= 0 && file_name[file_length] != 46) {
        file_length --;
    }
	// point the counter to the element after the point (.)
    file_length ++;

	// allocate memory space for the full file name
    char *name_with_new_extension = malloc((file_length+extension_length+1)*sizeof(char));
	// copy file name
    strncpy(name_with_new_extension, file_name, file_length);
	// copy extension
    for (uint8_t index = 0; index < extension_length; index++) {
        name_with_new_extension[file_length+index] = extension[index];
    }
	// add (\0) to the end
    name_with_new_extension[file_length+extension_length] = '\0';
    return name_with_new_extension;
}

void create_image(uint32_t * image, struct jpeg_desc * jdesc){
	/**
* @brief generate image in a pgm or ppm shape
* @param [in] image : image as an array
* @param [in] jdesc : contains information from jpeg  section headers
* @details use header and body functions
*/

	char *file_name = get_filename(jdesc);
	uint16_t height = get_image_size(jdesc, DIR_V);
	uint16_t width = get_image_size(jdesc, DIR_H);
	uint8_t components_number = get_nb_components(jdesc);

	if ( components_number == 1){
		// char * name = "image.pgm";

		char * name = change_extension(file_name, "pgm");
		FILE* file = fopen(name, "w");
		create_pgm_header(file, height, width, 255);
		create_pgm_body(file, image, height, width);
        fclose(file);
		free(name);

	} else {
		// char * name = "image.ppm";
		char * name = change_extension(file_name, "ppm");
		FILE * file = fopen(name, "w");
		create_ppm_header(file, height, width, 255);
		create_ppm_body(file, image, height, width);
        fclose(file);
		free(name);
	}
}


/*
*	PRINT PROGRESS BAR IN TERMINAL FUNCTIONS
*/

void print_loading_terminal(){
	/**
* @brief print header of loading animation
* @return header information
* @details print loading word in terminal
*/

	printf("\033[0;33m");
	printf("\n [LOADING]  ");
	printf("\033[0m");
	printf("Decoding image in process:  ");
	fflush(stdout);

}

void print_progress_terminal(uint16_t index, uint16_t * counter, uint16_t percentage){
	/**
* @brief print loading animation
* @param [in] index
* @param [in] counter
* @param [in] percentage
* @details print green progress bar in terminal
*/

	if(percentage < 1){
		for (uint8_t i=0; i<40; i++){
			printf("\033[1;32m");
			printf("\u2588");
			printf("\033[0m");
			fflush(stdout);
		}
	} else if (index == *counter){
		printf("\033[1;32m");
		printf("\u2588");
		printf("\033[0m");
		fflush(stdout);
		*counter += percentage;
	}
}

void print_finished_terminal(){
	printf("\033[1;34m");
	printf("\n [FINISHED WITH SUCCESS]\n \n");
	printf("\033[0m");
}

void print_execution_time(clock_t start, clock_t end){
	double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("\033[1;32m");
	printf("\n\n Execution time: %0.4f s\n", cpu_time_used);
	printf("\033[0m");
}

/*
* process id = (0 extractor), (1 iqzz_idct) (2 upsamling) (3 arrange_pixels) (4 troncature) (5 print in terminal)
*/
void update_process_time(clock_t *process_start_time, uint8_t process_id){
	/**
	* @brief print execution time for each process
	* @param [in] process_start_time
	* @param [in] process_id
	* @details this function update and print the time for each process, for process id = (0 extractor), (1 iqzz_idct) (2 upsamling) (3 arrange_pixels) (4 troncature) (5 print in terminal)
	*/
	static double extractor, iqzz_idct, upsamling, arrange_pixels, troncature = 0;
	clock_t current_time = clock();
	switch (process_id) {
		case 0:
			extractor += ((double) (current_time - *process_start_time)) / CLOCKS_PER_SEC;
			break;
		case 1:
			iqzz_idct += ((double) (current_time - *process_start_time)) / CLOCKS_PER_SEC;
			break;
		case 2:
			upsamling += ((double) (current_time - *process_start_time)) / CLOCKS_PER_SEC;
			break;
		case 3:
			arrange_pixels += ((double) (current_time - *process_start_time)) / CLOCKS_PER_SEC;
			break;
		case 4:
			troncature += ((double) (current_time - *process_start_time)) / CLOCKS_PER_SEC;
			break;
		case 5:
			printf("\033[0;36m");
			printf("\n Extractor        time: %0.4f s\n", extractor);
			if (iqzz_idct > 2*(extractor+upsamling+arrange_pixels+troncature)){
				printf("\033[0;33m");
				printf(" iqzz_idct        time: %0.4f s\n", iqzz_idct);
				printf("\033[0m");
				printf("\033[0;36m");
			} else if (iqzz_idct > (extractor+upsamling+arrange_pixels+troncature)){
				printf("\033[1;36m");
				printf(" iqzz_idct        time: %0.4f s\n", iqzz_idct);
				printf("\033[0m");
				printf("\033[0;36m");
			} else {
				printf(" iqzz_idct        time: %0.4f s\n", iqzz_idct);
			}
			printf(" upsamling        time: %0.4f s\n", upsamling);
			printf(" arrage pixels    time: %0.4f s\n", arrange_pixels);
			printf(" troncature       time: %0.4f s\n", troncature);
			printf("\033[0m");
			break;
		default:
			printf("ERROR, process not recognized! \n");
			break;
	}

	*process_start_time = clock();
}
