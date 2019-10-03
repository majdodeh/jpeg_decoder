#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include "jpeg_reader.h"
#include "huffman.h"

int main(int argc, char **argv){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s fichier.jpeg\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("#### TEST JPEG_READER #### \n \n");

    const char *filename = argv[1];
	struct jpeg_desc *jdesc = read_jpeg(filename);

    const char *name = get_filename(jdesc);
    printf("filename : %s \n",name);

    struct bitstream *stream = get_bitstream(jdesc);

    uint32_t byte;
    read_bitstream(stream, 24, &byte, false);
	printf("\n-- test bitstream --\n");
    printf("read of the first 3 bytes in data : %x \n", byte);
    read_bitstream(stream, 6, &byte, false);
    printf("read of 6 bits in data : %x \n", byte);
	read_bitstream(stream, 2, &byte, false);
    printf("read of 2 bits in data : %x \n \n", byte);

    uint8_t a = get_nb_quantization_tables(jdesc);
    printf("number of quantization tables : %i \n", a);

    uint8_t *quant_table = get_quantization_table(jdesc, 0);
    printf("\nquantization table of index 0\n");
    for (size_t i = 0; i < 64; i++) {
        printf("%i ", quant_table[i]);
    }
    printf("\n");

	uint8_t b = get_nb_huffman_tables(jdesc, 0);
    printf("\nnumber of huffman DC tables : %i \n", b);

    uint8_t c = get_nb_huffman_tables(jdesc, 1);
    printf("number of huffman AC tables : %i \n", c);

	int8_t value;
	if (get_image_size(jdesc, DIR_H) != 8) {
		printf("\n--test DC huffman table of index 0 --\n");
	    struct huff_table *huff_table = get_huffman_table(jdesc, 0, 0);
	    value = next_huffman_value(huff_table, stream);
	    printf("next huffman value : %x ", value);
		printf("\n \n--test AC huffman table of index 0 --\n");
		struct huff_table *huff_table2 = get_huffman_table(jdesc, 0, 0);
		value = next_huffman_value(huff_table2, stream);
		printf("next huffman value : %x\n", value);
		value = next_huffman_value(huff_table2, stream);
		printf("next huffman value : %x \n", value);
		value = next_huffman_value(huff_table2, stream);
		printf("next huffman value : %x \n \n", value);
	}

    uint16_t size = get_image_size(jdesc, DIR_H);
    uint16_t size2 = get_image_size(jdesc, DIR_V);
    printf("number of pixels in image: %i x %i\n", size, size2);

	value = get_nb_components(jdesc);
    printf("number of components : %i \n", value);

	value = get_frame_component_id(jdesc, 0);
	printf("id of the first component, Y : %i \n", value);

	value = get_frame_component_sampling_factor(jdesc, DIR_V, 0);
	printf("vertical sampling factor of the first component, Y : %i \n", value);

	value = get_frame_component_sampling_factor(jdesc, DIR_H, 0);
	printf("horizontal sampling factor of the first component, Y : %i \n", value);

	value = get_frame_component_quant_index(jdesc, 0);
	printf("quantization index of the first component, Y : %i \n", value);

	value = get_scan_component_id(jdesc, 0);
	printf("id of the first component in scan : %i \n", value);

	value = get_scan_component_huffman_index(jdesc, 0, 0);
	printf("DC huffman index of the first component in scan: %i \n", value);

	value = get_scan_component_huffman_index(jdesc, 1, 0);
	printf("AC huffman index of the first component in scan: %i \n ", value);

    return EXIT_SUCCESS;
}
