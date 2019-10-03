#ifndef __PPM_H__
#define __PPM_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "jpeg_reader.h"


// ppm header
void create_ppm_header(FILE * file, uint16_t height, uint16_t width, uint16_t max_comp);

// pgm header
void create_pgm_header(FILE * file, uint16_t height, uint16_t width, uint16_t max_comp);

// ppm image body
void create_pgm_body(FILE * file, uint32_t * image, uint16_t height, uint16_t width);

// ppm image body
void create_ppm_body(FILE * file, uint32_t * image, uint16_t height, uint16_t width);

//create final image
void create_image(uint32_t * image, struct jpeg_desc * jdesc);

// print test function
extern void print_loading_terminal(void);
extern void print_progress_terminal(uint16_t index, uint16_t *counter, uint16_t percentage);
extern void print_finished_terminal(void);
void print_execution_time(clock_t start, clock_t end);
void update_process_time(clock_t *process_start_time, uint8_t process_id);


#endif
