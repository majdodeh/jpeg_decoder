#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "iqzz.h"
#include "idct.h"

int main(void){

	//  quantification table
	uint8_t quant_table[64];
	for (int8_t i = 0; i<64; i++){
		quant_table[i] = 2;
	}
	printf("\n%s\n \n", "### quantification Inverse Test : ###");
	printf("\n%s\n", "[quant_table] 1D array (1 x 64):");
	for (int8_t i = 0; i < 64; i++) {
		printf("%i ", quant_table[i]);
	}

	// init vecteur from 0 to 63 (1 x 64)
	int16_t vecteur[64];
	for(int16_t i = 0; i<64; i++){
		vecteur[i]=i;
	}
	// vecteur test 1
	int16_t vecteur_test_1[64];
	for(int16_t i = 0; i<64; i++){
		vecteur_test_1[i]=rand()%100;
	}

	// vecteur test 2
	int16_t vecteur_test_2[64];
	for(int16_t i = 0; i<64; i++){
		vecteur_test_2[i]=0;}

	// vecteur test 3
	int16_t vecteur_test_3[64];
	for(int16_t i = 0; i<64; i++){
		vecteur_test_3[i]=rand()%255;
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

	// calculate ZigZag Inverse
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

	printf("\n \n%s\n", "### DCT Inverse Test : ###");
	// print [iDCT] 1d array  (1 x 64)
	idct(freq_table);
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
	printf("\n##########################");
	printf("\n%s\n", "##### END First Test #####");
	printf("##########################");

	// print initiale vecteur
	printf("\n \n%s\n", "[bloc] 1D array (1 x 64):");
	for (int8_t i = 0; i < 64; i++) {
		printf("%i ", vecteur_test_1[i]);
	}

	// calculate and print quantification Inverse
	quantification_inverse(vecteur_test_1, quant_table);
	printf("\n \n%s\n", "[iquant] 1D array (1 x 64):");
	for (int8_t i = 0; i < 64; i++) {
		printf("%i ", vecteur_test_1[i]);
	}

	// calculate ZigZag Inverse
	printf("\n \n%s\n", "### ZigZag Inverse Test : ###");
	zigzag_inverse(vecteur_test_1, freq_table);



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
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("\n \n%s\n", "### DCT Inverse Test : ###");
	// print [iDCT] 1d array  (1 x 64)
	idct(freq_table);
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
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("\n##########################");
	printf("\n%s\n", "##### END Second Test #####");
	printf("##########################");

	// print initiale vecteur
	printf("\n \n%s\n", "[bloc] 1D array (1 x 64):");
	for (int8_t i = 0; i < 64; i++) {
		printf("%i ", vecteur_test_2[i]);
	}

	// calculate and print quantification Inverse
	quantification_inverse(vecteur_test_2, quant_table);
	printf("\n \n%s\n", "[iquant] 1D array (1 x 64):");
	for (int8_t i = 0; i < 64; i++) {
		printf("%i ", vecteur_test_2[i]);
	}

	// calculate ZigZag Inverse
	printf("\n \n%s\n", "### ZigZag Inverse Test : ###");
	zigzag_inverse(vecteur_test_2, freq_table);

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
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("\n \n%s\n", "### DCT Inverse Test : ###");
	// print [iDCT] 1d array  (1 x 64)
	idct(freq_table);
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
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("\n##########################");
	printf("\n%s\n", "##### END Third Test #####");
	printf("##########################");

	// print initiale vecteur
	printf("\n \n%s\n", "[bloc] 1D array (1 x 64):");
	for (int8_t i = 0; i < 64; i++) {
		printf("%x ", (uint16_t)vecteur_test_3[i]);
	}

	// calculate and print quantification Inverse
	quantification_inverse(vecteur_test_3, quant_table);
	printf("\n \n%s\n", "[iquant] 1D array (1 x 64):");
	for (int8_t i = 0; i < 64; i++) {
		printf("%i ", vecteur_test_3[i]);
	}

	// calculate ZigZag Inverse
	printf("\n \n%s\n", "### ZigZag Inverse Test : ###");
	zigzag_inverse(vecteur_test_3, freq_table);



	// print [iqzz] 1d array  (1 x 64)
	printf("\n \n%s\n", "[ZigZag] 1D array (1 x 64)(iZigZag):");
	for (int8_t i = 0; i < 64; i++) {
		printf("%x ", (uint16_t)freq_table[i]);
    }

	// print [iqzz] 2d array (8 x 8)
	printf("\n \n%s\n", "[ZigZag] 1D array (8 x 8)(ZigZag):");
	for(int l = 0; l<8; l++){
		for (int m = 0; m<8; m++){
			printf("%x ", (uint8_t)freq_table[(l*8)+m]);
			if (freq_table[(l*8)+m] < 16){
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("\n \n%s\n", "### DCT Inverse Test : ###");
	// print [iDCT] 1d array  (1 x 64)
	idct(freq_table);
	printf("\n \n%s\n", "[iDCT] 1D array (1 x 64):");
	for (int8_t i = 0; i < 64; i++) {
		printf("%x ", freq_table[i]);
    }

	// print [iDCT] 2d array (8 x 8)
	printf("\n \n%s\n", "[iDCT] 2D array (8 x 8):");
	for(int l = 0; l<8; l++){
		for (int m = 0; m<8; m++){
			printf("%x ", freq_table[(l*8)+m]);
			if (freq_table[(l*8)+m] < 16){
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("\n");

    return EXIT_SUCCESS;
}
