#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "ppm.h"


int main(void){

	// create a file with write mode
	char * name = "images/ppm_test.ppm";
	FILE* test_file = fopen(name, "w");

	// add ppm header to file
	create_ppm_header(test_file, 8, 8, 255);
	fclose(test_file);
	if (test_file != NULL){
		printf("\nSuccessfully created an image with PPM header: %s\n \n", name);
		system("more images/ppm_test.ppm");
		printf("\n \n");
	}

	// create a file with write mode
	char * name2 = "images/pgm_test.pgm";
	FILE* test_file2 = fopen(name2, "w");

	// add pgm header to file
	create_pgm_header(test_file2, 8, 8, 255);
	fclose(test_file2);
	if (test_file2 != NULL){
		printf("Successfully created an image with PGM header: %s\n \n", name2);
		system("more images/pgm_test.pgm");
		printf("\n");
	}


    return EXIT_SUCCESS;
}
