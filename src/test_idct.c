#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "idct.h"
#include "iqzz.h"



int main(void){

	//  quantification table
	uint8_t quant_table[64];
	for (int8_t i = 0; i<64; i++){
		quant_table[i] = 2;
	}
	printf("\n %s\n", "### quantification Inverse Test : ###");
	printf("\n%s\n", "[quant_table] 1D array (1 x 64):");
	for (int8_t i = 0; i < 64; i++) {
		printf("%i ", quant_table[i]);
	}

	// init vecteur from 0 to 63 (1 x 64)
	int16_t vecteur[64];
	for(int16_t i = 0; i<64; i++){
		vecteur[i]=i;
	}

	// print initiale vecteur
	printf("\n \n%s\n", "[bloc] 1D array (1 x 64):");
	for (int8_t i = 0; i < 64; i++) {
		printf("%i ", vecteur[i]);
	}

	// calculate and print quantification Inverse
	quantification_inverse(vecteur, quant_table);
	printf("\n \n%s\n", "[iquant] 1D array (1 x 64):");
	for (int8_t i = 0; i < 64; i++) {
		printf("%i ", vecteur[i]);
	}

	// calcuqte ZigZag Inverse
	printf("\n \n%s\n", "### ZigZag Inverse Test : ###");
	int16_t freq_table[64];
	zigzag_inverse(vecteur, freq_table);



	// print [iqzz] 1d array  (1 x 64)
	printf("\n \n%s\n", "[ZigZag] 1D array (1 x 64)(iZigZag):");
	for (int8_t i = 0; i < 64; i++) {
		printf("%i ", freq_table[i]);
    }

	// print [iqzz] 2d array (8 x 8)
	printf("\n \n%s\n", "[ZigZag] 1D array (8 x 8)(ZigZag):");
	for(int l = 0; l<8; l++){
		for (int m = 0; m<8; m++){
			printf("%i ", freq_table[(l*8)+m]);
			if (freq_table[(l*8)+m] < 10){
				printf("  ");
			} else if (freq_table[(l*8)+m] < 100){
				printf(" ");
			}
		}
		printf("\n");
	}

	float *cos_table = malloc(64 * sizeof(float));
	define_cos_table(cos_table);

	float *cos_sin_table = malloc(6 * sizeof(float));
	define_cos_sin_table(cos_sin_table);

	// print [iDCT] 1d array  (1 x 64)
	printf("\n \n%s\n", "[iDCT] ");
	idct_optimized_2(freq_table, cos_sin_table);
	printf("\n \n%s\n", "[iDCT] 1D array (1 x 64):");
	for (int8_t i = 0; i < 64; i++) {
		printf("%i ", freq_table[i]);
    }

	// print [iDCT] 2d array (8 x 8)
	printf("\n \n%s\n", "[iDCT] 2D array (8 x 8):");
	for(int l = 0; l<8; l++){
	    for (int m = 0; m<8; m++){
	 		printf("%i ", freq_table[(l*8)+m]);
			if (freq_table[(l*8)+m] < 10){
				printf("  ");
			} else if (freq_table[(l*8)+m] < 100){
				printf(" ");
			}
	 	}
	 	printf("\n");
	}
	printf("\n");


    return EXIT_SUCCESS;
}
