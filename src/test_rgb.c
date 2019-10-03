#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "iqzz.h"
#include "idct.h"
#include "rgb.h"


int main(void){

	//  quantification table
	uint8_t quant_table[64];
	for (int8_t i = 0; i<64; i++){
		quant_table[i] = 2;
	}
	printf("%s\n \n", "### quantification Inverse Test : ###");
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

	// cqlcuqte ZigZag Inverse
	printf("\n \n%s\n", "### ZigZag Inverse Test : ###");
	int16_t freq_table[64];
	zigzag_inverse(vecteur, freq_table);




	// print [iqzz] 2d array (8 x 8)
	printf("\n %s\n", "[ZigZag] 1D array (8 x 8)(ZigZag):");
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
	// print [iDCT] 1d array  (1 x 64)
	idct(freq_table);

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


    printf("\n%s\n", "######## Test RGB Simple 1 #######");
    uint8_t y = (int8_t)235;
    uint8_t Cb = (int8_t)128;
		uint8_t Cr = (int8_t)128;
		printf("\ny  %i \nCb %i \nCr %i \n", y, Cb, Cr);
		printf("\n");
    uint8_t *test = convert_to_rgb(y, Cb, Cr);
    printf("R %u \nG %u \nB %u \n", test[0], test[1], test[2]);

    printf("\n%s\n", "######## Test RGB Simple 2 #######");
		uint8_t y2 = (int8_t)100;
    uint8_t Cb2 = (int8_t)30;
		uint8_t Cr2 = (int8_t)30;
		printf("\ny  %i \nCb %i \nCr %i \n", y2, Cb2, Cr2);
		printf("\n");
    uint8_t *test2 = convert_to_rgb(y2, Cb2, Cr2);
    printf("R %u \nG %u \nB %u \n", test2[0], test2[1], test2[2]);

		printf("\n%s\n", "########  Test RGB Block  ########");
		printf("\n");
		uint8_t y_tab[64]= {255, 100, 255, 41, 186, 93, 150, 125, 0, 255, 76, 164, 100, 141, \
			 113, 127, 255, 76, 154, 110, 142, 123, 135, 132, 40, 164, 110, 138, 119, \
			 131, 122, 126, 186, 101, 143, 120, 135, 126, 131, 134, 92, 140, 122, 131, \
			 124, 129, 124, 123, 152, 116, 136, 125, 132, 129, 130, 144, 122, 127, 126, \
			 129, 123, 132, 112, 128};
	  uint8_t Cb_tab[64]= {128, 30, 255, 41, 186, 93, 150, 125, 0, 255, 76, 164, 100, 141, \
 		   113, 127, 255, 76, 154, 110, 142, 123, 135, 132, 40, 164, 110, 138, 119, \
 		   131, 122, 126, 186, 101, 143, 120, 135, 126, 131, 134, 92, 140, 122, 131, \
 		   124, 129, 124, 123, 152, 116, 136, 125, 132, 129, 130, 144, 122, 127, 126, \
 		   129, 123, 132, 112, 128};
	  uint8_t Cr_tab[64]= {128, 30, 255, 41, 186, 93, 150, 125, 0, 255, 76, 164, 100, 141, \
			 113, 127, 255, 76, 154, 110, 142, 123, 135, 132, 40, 164, 110, 138, 119, \
			 131, 122, 126, 186, 101, 143, 120, 135, 126, 131, 134, 92, 140, 122, 131, \
			 124, 129, 124, 123, 152, 116, 136, 125, 132, 129, 130, 144, 122, 127, 126, \
			 129, 123, 132, 112, 128};
		printf(" [y] ");
	  for (int8_t i = 0; i < 64; i++) {
			 printf("%i ", y_tab[i]);
	     }
		printf("\n \n [Cb] ");
	  for (int8_t i = 0; i < 64; i++) {
			 printf("%i ", Cb_tab[i]);
	     }
		printf("\n \n [Cr] ");
	  for (int8_t i = 0; i < 64; i++) {
			 printf("%i ", Cr_tab[i]);
	     }
		printf("\n\n\n [RGB Block] ");
    uint32_t *test_tab = convert_to_rgb_tab(y_tab, Cb_tab, Cr_tab);
		for (int8_t i = 0; i < 64; i++) {
			 printf("%x ", test_tab[i]);
	     }
		printf("\n \n");




    return EXIT_SUCCESS;
}
