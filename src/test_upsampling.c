#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "upsampling.h"

int main(void){

	struct components comp1;
	struct components comp2;

	comp1.Y = malloc(64*sizeof(uint16_t));
	comp1.Cb = malloc(64*sizeof(uint16_t));
	comp1.Cr = malloc(64*sizeof(uint16_t));
	comp2.Y = malloc(128*sizeof(uint16_t));
	comp2.Cb = malloc(128*sizeof(uint16_t));
	comp2.Cr = malloc(128*sizeof(uint16_t));

	printf("\n\n###########################");
	printf("\n####### Init Blocks #######\n");
	printf("###########################\n");

	printf("\n [Y]  ");
	for (uint8_t i=0; i < 64; i++){
		comp1.Y[i] = i;
		printf("%u ", comp1.Y[i]);
	}
	printf("\n [Cb]  ");
	for (uint8_t i=0; i < 64; i++){
		comp1.Cb[i] = i;
		printf("%u ", comp1.Cb[i]);
	}
	printf("\n [Cr]  ");
	for (uint8_t i=0; i < 64; i++){
		comp1.Cr[i] = i;
		printf("%u ", comp1.Cr[i]);
	}

	printf("\n");
	printf("\n##################################");
	printf("\n###### Upsamling Horizontal ######\n");
	printf("##################################\n");

	printf("\n [Cb]\n ");
	upsampling_horizontal(&comp1, &comp2);
	for(int l = 0; l<16; l++){
		for (int m = 0; m<8; m++){
			if( l == 8 && m == 0){
				printf("\n");
			}
			printf("%i ", comp2.Cb[(l*8)+m]);
			if (comp2.Cb[(l*8)+m] < 10){
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("\n\n [Cr]\n \n ");
	for(int l = 0; l<16; l++){
		for (int m = 0; m<8; m++){
			if( l == 8 && m == 0){
				printf("\n");
			}
			printf("%i ", comp2.Cb[(l*8)+m]);
			if (comp2.Cb[(l*8)+m] < 10){
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("\n");
	printf("\n\n###########################");
	printf("\n####### Init Blocks #######\n");
	printf("###########################\n");

	struct components comp3;
	struct components comp4;

	comp3.Y = malloc(64*sizeof(uint16_t));
	comp3.Cb = malloc(64*sizeof(uint16_t));
	comp3.Cr = malloc(64*sizeof(uint16_t));
	comp4.Y = malloc(128*sizeof(uint16_t));
	comp4.Cb = malloc(128*sizeof(uint16_t));
	comp4.Cr = malloc(128*sizeof(uint16_t));


	printf("\n [Y 3]  ");
	for (uint8_t i=0; i < 64; i++){
		comp3.Y[i] = i;
		printf("%u ", comp3.Y[i]);
	}
	printf("\n [Cb 3]  ");
	for (uint8_t i=0; i < 64; i++){
		comp3.Cb[i] = i;
		printf("%u ", comp3.Cb[i]);
	}
	printf("\n [Cr 3]  ");
	for (uint8_t i=0; i < 64; i++){
		comp3.Cr[i] = i;
		printf("%u ", comp3.Cr[i]);
	}

	printf("\n");
	printf("\n################################");
	printf("\n###### Upsamling vertical ######\n");
	printf("################################\n");

	printf("\n [Cb] \n");
	upsampling_vertical(&comp3, &comp4);
	for(int l = 0; l<16; l++){
		for (int m = 0; m<8; m++){
			if( l == 8 && m == 0){
				printf("\n");
			}
			printf("%i ", comp4.Cb[(l*8)+m]);
			if (comp4.Cb[(l*8)+m] < 10){
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("\n [Cr] \n");
	for(int l = 0; l<16; l++){
		for (int m = 0; m<8; m++){
			if( l == 8 && m == 0){
				printf("\n");
			}
			printf("%i ", comp4.Cb[(l*8)+m]);
			if (comp4.Cb[(l*8)+m] < 10){
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("\n");
	printf("\n\n###########################");
	printf("\n####### Init Blocks #######\n");
	printf("###########################\n \n");
	printf(" We will use the same components structure of Upsampling Horizontal (size 2x1 blocks) \n \n ");

	printf("\n###### Upsamling Horizontal ######\n");

	printf("\n [Cb]\n ");
	for(int l = 0; l<16; l++){
		for (int m = 0; m<8; m++){
			if( l == 8 && m == 0){
				printf("\n");
			}
			printf("%i ", comp2.Cb[(l*8)+m]);
			if (comp2.Cb[(l*8)+m] < 10){
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("\n\n [Cr]\n \n ");
	for(int l = 0; l<16; l++){
		for (int m = 0; m<8; m++){
			if( l == 8 && m == 0){
				printf("\n");
			}
			printf("%i ", comp2.Cb[(l*8)+m]);
			if (comp2.Cb[(l*8)+m] < 10){
				printf(" ");
			}
		}
		printf("\n");
	}

	struct components comp5;

	comp5.Y = malloc(256*sizeof(uint16_t));
	comp5.Cb = malloc(256*sizeof(uint16_t));
	comp5.Cr = malloc(256*sizeof(uint16_t));


	printf("\n");
	printf("\n##########################");
	printf("\n###### Upsamling HV ######\n");
	printf("##########################\n");

	upsampling_vertical_after_horizontal(&comp2, &comp5);

	printf("\n [Cb] \n");
	for(int l = 0; l<32; l++){
		for (int m = 0; m<8; m++){
			if( l%8==0 && m == 0){
				printf("\n");
			}
			printf("%i ", comp5.Cb[(l*8)+m]);
			if (comp5.Cb[(l*8)+m] < 10){
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("\n [Cr] \n");
	for(int l = 0; l<32; l++){
		for (int m = 0; m<8; m++){
			if( l%8==0 && m == 0){
				printf("\n");
			}
			printf("%i ", comp5.Cb[(l*8)+m]);
			if (comp5.Cb[(l*8)+m] < 10){
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("\n");

    return EXIT_SUCCESS;
}
