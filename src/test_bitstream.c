#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include "bitstream.h"

void editor(const char *filename){
	FILE *file = fopen(filename, "w");
	uint8_t buff[22] = {0x00, 0x08, 0x01, 0x01, 0x00, 0x00, 0x3f, 0xff, 0x00, 0xf9, 0x2c, 0x41, 0x85, 0x28, 0x03, 0xd8, 0x6c, 0x3d, 0xb1, 0x0b, 0x97, 0xb8};
	fwrite(buff, 1, 22, file);
	fclose(file);
}

int main(int argc, char **argv){
	if (argc != 2) {
		fprintf(stderr, "Usage: %s fichier\n", argv[0]);
		return EXIT_FAILURE;
	}
	const char *filename = argv[1];
	editor(filename);

	printf("#### TEST BITSTREAM #### \n");
	printf("test file contains : 0x00, 0x08, 0x01, 0x01, 0x00, 0x00, 0x3f, 0xff, 0x00, 0xf9, 0x2c, 0x41, 0x85, 0x28, 0x03, 0xd8, 0x6c, 0x3d, 0xb1, 0x0b, 0x97, 0xb8 \n");


	struct bitstream *stream = create_bitstream(filename);
	uint32_t byte;
	read_bitstream(stream, 8, &byte, false);
	printf("\nReading of the test file first byte : %x ", byte);


	read_bitstream(stream, 4, &byte, false);
	printf("\nReading of 4 more bits : %x ", byte);

	read_bitstream(stream, 4, &byte, false);
	printf("\nReading of 4 more bits : %x ", byte);

	read_bitstream(stream, 2, &byte, false);
	printf("\nReading of 2 more bits : %x ", byte);

	read_bitstream(stream, 6, &byte, false);
	printf("\nReading of 6 more bits : %x ", byte);

	read_bitstream(stream, 32, &byte, false);
	printf("\nReading of 4 more byte : %x \n", byte);

	printf("\nByte stuffing discard = true \n");
	read_bitstream(stream, 6, &byte, true);
	printf("\nReading of 6 more bits : %x ", byte);

	read_bitstream(stream, 6, &byte, true);
	printf("\nReading of 6 more bits : %x \n", byte);


	close_bitstream(stream);
	return 0;


}
