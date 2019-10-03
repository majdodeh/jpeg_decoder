#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include "jpeg_reader.h"
#include "bitstream.h"
#include "huffman.h"
#include "extracteur.h"
#include "extracteur.h"
#include "iqzz.h"
#include "idct.h"
#include "ppm.h"


int main(int argc, char **argv){

	if (argc != 2) {
		fprintf(stderr, "Usage: %s fichier.jpeg\n", argv[0]);
		return EXIT_FAILURE;
    }

	printf("\n %s \n", "Extraction Test:");

	// 8x8 block
	int16_t block_table[64];


	// read image and get bitstream
	const char *filename = argv[1];
	struct jpeg_desc *jdesc = read_jpeg(filename);
	struct bitstream *stream = get_bitstream(jdesc);

	// get huffman tables for DC and AC
	struct huff_table *htableDC = get_huffman_table(jdesc, DC, 0);
	struct huff_table *htableAC = get_huffman_table(jdesc, AC, 0);

	// test magnitude

	printf("\n Get [magnitude] test: \n \n");
	printf(" get_magnitude(0, 0) = %i\n", get_magnitude(0, 0) );
	printf(" get_magnitude(1, 0) = %i\n", get_magnitude(1, 0) );
	printf(" get_magnitude(2, 3) = %i\n", get_magnitude(2, 3) );
	printf(" get_magnitude(3, 4) = %i\n", get_magnitude(3, 4) );
	printf(" get_magnitude(4, 7) = %i\n", get_magnitude(4, 7) );
	printf(" get_magnitude(11, 1024) = %i\n", get_magnitude(11, 1024) );
	printf(" get_magnitude(11, 1023) = %i\n", get_magnitude(11, 1023) );
	printf(" get_magnitude(11, 1025) = %i\n", get_magnitude(11, 1025) );

	// get DC and AC values, stored in block_table
	printf("\n Get [DC/AC] test: \n \n");
	get_dc(htableDC, stream, block_table);
	get_ac(htableAC, stream, block_table);

	printf(" Image name : %s\n \n", filename);
	// print block after converting from huffman and using RLE
	printf(" %s ", "[bloc]");
	for(int i = 0; i < 64; i++){
		printf("%x ", (uint16_t)block_table[i]);
	}

	// calculate and print [iquant]
	uint8_t *quant_table = get_quantization_table(jdesc, 0);
	quantification_inverse(block_table, quant_table);

	printf("\n \n %s ", "[iquant]");
	for(int i = 0; i < 64; i++){
		printf("%x ", (uint16_t)block_table[i]);
	}

	// calculate and print [izz]
	int16_t freq_table[64];
	zigzag_inverse(block_table, freq_table);
	printf("\n \n %s ", "[izz]");
	for(int i = 0; i < 64; i++){
		printf("%x ", (uint16_t)freq_table[i]);
	}

	float *cos_table = malloc(64 * sizeof(float));
	define_cos_table(cos_table);

	// print [idct]
	idct_optimized_1(freq_table, cos_table);
	printf("\n \n %s ", "[idct]");
	for (int8_t i = 0; i < 64; i++) {
		printf("%x ", freq_table[i]);
	}
	printf("\n \n");

	printf("\n Get [components blocks number] test: \n \n");
	uint8_t *blocks = get_components_blocks_number(jdesc);
	if (get_nb_components(jdesc) == 3){
		printf(" Y components number = %u\n", blocks[0]);
		printf(" Cb components number = %u\n", blocks[1]);
		printf(" Cr components number = %u\n \n", blocks[2]);
	} else {
		printf(" Y components number = %u\n \n", blocks[0]);
	}


	close_jpeg(jdesc);
    return EXIT_SUCCESS;
}
