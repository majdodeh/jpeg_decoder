#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "mcu.h"
#include "jpeg_reader.h"
#include "bitstream.h"
#include "image.h"

int main(int argc, char **argv){

	if (argc != 2) {
		fprintf(stderr, "Usage: %s fichier.jpeg\n", argv[0]);
		return EXIT_FAILURE;
	}


	// read image and get bitstream
	const char *filename = argv[1];
	struct jpeg_desc *jdesc = read_jpeg(filename);

	printf("MCU Test for %s \n", filename);
	uint32_t mcu_nb = get_mcu_number_in_image(jdesc);
	printf("\nMCUs Number in image = %u\n", mcu_nb);
	uint16_t width = get_pixels_nb_in_direction(jdesc, DIR_H);
	printf("\nImage Width = %u\n", width);
	uint16_t heigth = get_pixels_nb_in_direction(jdesc, DIR_H);
	printf("\nImage heigth = %u\n", heigth);

	uint8_t Y_HOR = get_frame_component_sampling_factor(jdesc, DIR_H, 0);
	uint8_t Y_VER = get_frame_component_sampling_factor(jdesc, DIR_V, 0);
	printf("\nSampling factor H = %u\n", Y_HOR);
	printf("\nSampling factor V = %u\n\n", Y_VER);

	uint16_t size = Y_HOR*Y_VER*64;
	printf("Create components with [%u] elements:\n \n", size);

	struct components comps = create_components(jdesc);
	// printf("Y_HOR*Y_VER*64 %u\n", Y_HOR*Y_VER*64);
	for (uint8_t i=0; i<size; i++){
		comps.Y[i] = i;
	}

	printf("component [Y]:\n");
	for (uint8_t i = 0; i < size; i++) {
		printf("%u ", comps.Y[i]);
	}
	printf("\n\n");

	free_components(&comps, jdesc);

    return EXIT_SUCCESS;
}
