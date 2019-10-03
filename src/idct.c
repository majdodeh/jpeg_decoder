/**
* @file idct.c
* @author Odeh, Morin, Pouget
* @date 31 May 2019
* @copyright 2019 Odeh, Morin, Pouget
* @brief implement idct
*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "idct.h"
#include <math.h>
#include <search.h>
#ifndef PI
#define PI 3.14159265358979323846
#endif


// static function for iDCT
float coeff(uint8_t epsilon)
{/**
* @brief choose factor value in idct
* @param [in] epsilon
* @return factor value in idct
*/

    if (epsilon == 0) {
        return 1/sqrt(2);
    } else {
        return 1;
    }
}

// static function for iDCT
uint8_t fixed_values(float value){
  /**
* @brief manage cases when value after idct is not between 0 and 255
* @param [in] value
* @return 0, 255 or value
*/

    float value_tmp = roundf(value);
    if (value_tmp > 255)
        value_tmp = 255;
    if (value_tmp < 0)
        value_tmp = 0;
    return (uint8_t)value_tmp;
}

// calculate the iDCT S(x,y) for all values in freq_table
void idct(int16_t *freq_table){
    // make a copy of freq_table
    int16_t tab_copy[64];
    for (uint8_t index=0; index<64; index++){
        tab_copy[index] = freq_table[index];
    }
    // calculate S(x,y)
    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            float result = 0;
            for (uint8_t j = 0; j < 8; j++) {
                for (uint8_t i = 0; i < 8; i++) {
                    result += coeff(j)*coeff(i)*cos((2*x+1)*j*PI/16)*cos((2*y+1)*i*PI/16)*tab_copy[8*j+i];
                }
            }
            // final result
            result = result/4 + 128;
            // check and return a value between 0 and 255
            freq_table[8*x+y] = fixed_values(result);
        }
    }
}


void define_cos_table(float *cos_table){
  /**
* @brief build a table of used cos values
* @param [in] cos_table : stored values of cos
* @details cos_table : stored values of cos
*/

    // We could define the table by hand :
    // cos_table[0] = 0,51235
    // cos_table[1] = 0,12363
    // ...
    for (uint8_t k = 0; k < 8; k++){
        for (uint8_t j = 0; j < 8; j++) {
            cos_table[8*k+j] = cos((2*k+1)*j*PI/16);
        }
    }
}

void define_cos_sin_table(float *cos_sin_table){
  /**
* @brief build a table of used cos and sin values
* @param [in] cos_sin_table : stored values of cos and sin
* @details cos_sin_table : stored values of cos and sin
*/
    /* COS VALUES : index 0 -> cos(1 * pi / 16) // index 1 -> cos(3 * pi / 16) // index 2 -> cos(6 * pi / 16) */
    cos_sin_table[0] = 0.9807852804;
    cos_sin_table[1] = 0.8314696123;
    cos_sin_table[2] = 0.3826834324;
    /* SIN VALUES : index 3 -> sin(1 * pi / 16) // index 4 -> sin(3 * pi / 16) // index 5 -> sin(6 * pi / 16) */
    cos_sin_table[3] = 0.195090322;
    cos_sin_table[4] = 0.555570233;
    cos_sin_table[5] = 0.9238795325;

}

void idct_optimized_1(int16_t *freq_table, float *cos_table){
    /**
* @brief implement idct
* @param [in] freq_table
* @param [in] cos_table : stored values of cos
* @details transformed freq_1D_table
*/
    // make a copy of freq_table
    int16_t tab_copy[64];
    for (uint8_t index=0; index<64; index++){
        tab_copy[index] = freq_table[index];
    }
    // calculate S(x,y)
    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            float result = 0;
            for (uint8_t j = 0; j < 8; j++) {
                for (uint8_t i = 0; i < 8; i++) {
                    result += coeff(j)*coeff(i)*cos_table[x*8+j]*cos_table[y*8+i]*tab_copy[8*j+i];
                }
            }
            // final result
            result = result/4 + 128;
            // check and return a value between 0 and 255
            freq_table[8*x+y] = fixed_values(result);
        }
    }
}

void add_and_sub(float **freq_1D_table, uint8_t a, uint8_t b){
  /**
* @brief Implementation of Löffler algorithm operation named butterfly
* @param [in] array of pointers 1x8 : freq_1D_table
* @param [in] index in array : a
* @param [in] index in array : b
* @details transformed freq_1D_table
*/

    float temp_a = *freq_1D_table[a];
    *freq_1D_table[a] = (*freq_1D_table[a] + *freq_1D_table[b]) / 2;
    *freq_1D_table[b] = (temp_a - *freq_1D_table[b]) / 2;
}

void rotation(float **freq_1D_table, uint8_t a, uint8_t b, float k, float *cos_sin_table, uint8_t i){
  /**
* @brief Implementation of Löffler algorithm operation named rotation
* @param [in] array of pointers 1x8 : freq_1D_table
* @param [in] index in array : a
* @param [in] index in array : b
* @param [in] k
* @param [in] cosinus and sinus table
* @param [in] index in cosinus and sinus table
* @details transformed freq_1D_table
*/
    //it is possible to calculate this sum in order to reduce by 1 the number of multiplication required for rotation operation
    float sum = (*freq_1D_table[a] / k) * (cos_sin_table[i] + cos_sin_table[3 + i]);
    float temp = -(*freq_1D_table[a] + *freq_1D_table[b]) * cos_sin_table[3 + i] / k + sum;
    float tempo = (*freq_1D_table[b] - *freq_1D_table[a])  * cos_sin_table[i] / k + sum;
    *freq_1D_table[a] = temp;
    *freq_1D_table[b] = tempo;
}

void mult(float **freq_1D_table, uint8_t a){
 /**
* @brief Implementation of Löffler algorithm operation
* @param [in] array of pointers 1x8 : freq_1D_table
* @param [in] index in array : a
* @details transformed freq_1D_table
*/
    float temp = (*freq_1D_table[a]) / sqrt(2);
    *freq_1D_table[a] = temp;
}


void re_order(float **freq_1D_table){
  /**
* @brief reorder factors
* @param [in] array of pointers 1x8 : freq_1D_table
* @details transformed freq_1D_table
*/
    float temp_table[8];
    // float *temp_table = malloc(8 * sizeof(float));
    for (uint8_t index = 0; index < 8; index++) {
        temp_table[index] = *freq_1D_table[index];
    }
    *freq_1D_table[0] = temp_table[0];
    *freq_1D_table[1] = temp_table[4];
    *freq_1D_table[2] = temp_table[2];
    *freq_1D_table[3] = temp_table[6];
    *freq_1D_table[4] = temp_table[7];
    *freq_1D_table[5] = temp_table[3];
    *freq_1D_table[6] = temp_table[5];
    *freq_1D_table[7] = temp_table[1];
}

void re_order_plus(float **freq_1D_table){
 /**
* @brief reorder factors and multiply them 8
* @param [in] array of pointers 1x8 : freq_1D_table
* @details transformed freq_1D_table
*/
    float temp_table[8];
    // float *temp_table = malloc(8 * sizeof(float));
    for (uint8_t index = 0; index < 8; index++) {
        temp_table[index] = *freq_1D_table[index];
    }
    *freq_1D_table[0] = temp_table[0] * 8;
    *freq_1D_table[1] = temp_table[4] * 8;
    *freq_1D_table[2] = temp_table[2] * 8;
    *freq_1D_table[3] = temp_table[6] * 8;
    *freq_1D_table[4] = temp_table[7] * 8;
    *freq_1D_table[5] = temp_table[3] * 8;
    *freq_1D_table[6] = temp_table[5] * 8;
    *freq_1D_table[7] = temp_table[1] * 8;
}

void idct_transform_1D_line(int16_t **freq_1D_table, float *cos_sin_table){
  /**
* @brief Löffler algorithm for 1x8 array. Here it is applied on a line of the matrix.
* @param [in] array of pointers 1x8 : freq_1D_table
* @param [in] table of cosinus and sinus values : cos_sin_table
* @details transformed freq_1D_table
*/
    // make a copy of freq_1D_table
    float *temp_1D_table[8];
    for (uint8_t i = 0; i < 8; i++){
        temp_1D_table[i] = malloc(sizeof(float));
        *temp_1D_table[i] = *freq_1D_table[i];
    }
    add_and_sub(temp_1D_table, 1, 7);
    mult(temp_1D_table, 3);
    mult(temp_1D_table, 5);
    add_and_sub(temp_1D_table, 7, 5);
    add_and_sub(temp_1D_table, 1, 3);
    add_and_sub(temp_1D_table, 0, 4);
    rotation(temp_1D_table, 2, 6, sqrt(2), cos_sin_table, 2);
    add_and_sub(temp_1D_table, 0, 6);
    add_and_sub(temp_1D_table, 4, 2);
    rotation(temp_1D_table, 7, 1, 1, cos_sin_table, 1);
    rotation(temp_1D_table, 3, 5, 1, cos_sin_table, 0);
    add_and_sub(temp_1D_table, 6, 7);
    add_and_sub(temp_1D_table, 2, 3);
    add_and_sub(temp_1D_table, 4, 5);
    add_and_sub(temp_1D_table, 0, 1);
    re_order(temp_1D_table);
    // copy and round results of the transformation in freq_1D_table
    for (uint8_t i = 0; i < 8; i++){
        *freq_1D_table[i] = (int16_t)*temp_1D_table[i];
    }
    for (uint8_t i = 0; i < 8; i++){
        free(temp_1D_table[i]);
    }
}

void idct_transform_1D_column(int16_t **freq_1D_table, float *cos_sin_table){
  /**
* @brief Löffler algorithm for 1x8 array. Here it is applied on a column of the matrix
* @param [in] array of pointers 1x8 : freq_1D_table
* @param [in] table of cosinus and sinus values : cos_sin_table
* @details transformed freq_1D_table
*/
    // make a copy of freq_1D_table
    float *temp_1D_table[8];
    for (uint8_t i = 0; i < 8; i++){
        temp_1D_table[i] = malloc(sizeof(float));
        *temp_1D_table[i] = *freq_1D_table[i];
    }
    add_and_sub(temp_1D_table, 1, 7);
    mult(temp_1D_table, 3);
    mult(temp_1D_table, 5);
    add_and_sub(temp_1D_table, 7, 5);
    add_and_sub(temp_1D_table, 1, 3);
    add_and_sub(temp_1D_table, 0, 4);
    rotation(temp_1D_table, 2, 6, sqrt(2), cos_sin_table, 2);
    add_and_sub(temp_1D_table, 0, 6);
    add_and_sub(temp_1D_table, 4, 2);
    rotation(temp_1D_table, 7, 1, 1, cos_sin_table, 1);
    rotation(temp_1D_table, 3, 5, 1, cos_sin_table, 0);
    add_and_sub(temp_1D_table, 6, 7);
    add_and_sub(temp_1D_table, 2, 3);
    add_and_sub(temp_1D_table, 4, 5);
    add_and_sub(temp_1D_table, 0, 1);
    re_order_plus(temp_1D_table);
    // copy and round results of the transformation in freq_1D_table
    for (uint8_t i = 0; i < 8; i++){
        *freq_1D_table[i] = (int16_t)*temp_1D_table[i];
    }
    for (uint8_t i = 0; i < 8; i++){
        free(temp_1D_table[i]);
    }
}

void idct_optimized_2(int16_t *freq_table, float *cos_sin_table){
  /**
* @brief implement idct
* @param [in] freq_table
* @param [in] cos_table_SIN : stored values of cos and sin
* @details transformed freq_table
*/
    /* line is an array of pointers, each pointer refers to a cell in the given matrix
    the point is to transform cells in the given matrix, by using line */
    int16_t *line[8];
    for (uint8_t l = 0; l < 8; l++) {
        line[0] = freq_table + 8*l;
        for (uint8_t index = 1; index < 8; index++) {
            line[index] = line[0] + index;

        }
        idct_transform_1D_line(line, cos_sin_table);
    }

    /* column is an array of pointers, each pointer refers to a cell in the given matrix
    the point is to transform cells in the given matrix, by using line */
    int16_t *column[8];
    for (uint8_t c = 0; c < 8; c++) {
        column[0] = freq_table + c;
        for (uint8_t index = 1; index < 8; index++) {
            column[index] = column[0] + 8*index;

        }
        idct_transform_1D_column(column, cos_sin_table);
    }

    // saturation and offset
    for (uint8_t i = 0; i < 64; i++){
        freq_table[i] = fixed_values(freq_table[i] + 128);
    }

}
