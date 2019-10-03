/**
* @file iqzz.c
* @author Odeh, Morin, Pouget
* @date 31 May 2019
* @copyright 2019 Odeh, Morin, Pouget
* @brief implement reverse zigzag and reverse quantification
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <math.h>
#define PI 3.14159265

void zigzag_inverse( int16_t *array, int16_t *freq_table){
  /**
* @brief realize reverse zigzag
* @param [in] array : array of 64 values
* @param [in] freq_table : 64 differents values
* @return freq_table after zigzag reverse operation
*/

    int16_t tab[8][8];
    uint8_t num_diagonal = 0;
    uint8_t i = 0;
    while ((i<64) && (num_diagonal<=14))
    { if (num_diagonal<7){
        for(int k = 0; k<=num_diagonal; k+= 1){
            for (int j = 0; j <=num_diagonal; j++)
            {
                if(num_diagonal%2 == 0){
                    if (k+j == num_diagonal){
                        tab[j][k] = array[i];
                        i++;
                    }
                }
                else{
                    if (k+j == num_diagonal){
                        tab[k][j] = array[i];
                        i++;
                    }
                }
            }
        }
        num_diagonal += 1;
    }else {
        for(int k = num_diagonal - 7; k<=7; k+= 1){
            for (int j = num_diagonal - 7; j <=7; j++)
            {if(num_diagonal%2 == 0){
                if (k+j == num_diagonal){
                    tab[j][k] = array[i];
                    i++;
                }
            }
            else{
                if (k+j == num_diagonal){
                    tab[k][j] = array[i];
                    i++;}
                }
            }
        }
        num_diagonal += 1;
    }
    for(int l = 0; l<8; l++){
        for (int m = 0; m<8; m++){
            freq_table[(l*8)+m] = tab[l][m];
        }
    }
    // free(array);
    // free(tab);
}

}

void zigzag_inverse_optimized_1( int16_t *array, int16_t *freq_table){
    /**
    * @brief realize reverse zigzag (optimized version)
    * @param [in] array : array of 64 values
    * @param [in] freq_table : 64 differents values
    * @return freq_table after zigzag reverse operation
    */

    // store the zigzag index values
    uint8_t index_zigzag[64] = { 0, 1, 5, 6,14,15,27,28,
                                 2, 4, 7,13,16,26,29,42,
                                 3, 8,12,17,25,30,41,43,
                                 9,11,18,24,31,40,44,53,
                                10,19,23,32,39,45,52,54,
                                20,22,33,38,46,51,55,60,
                                21,34,37,47,50,56,59,61,
                                35,36,48,49,57,58,62,63};


    // reorder values of the array in freq_table using the array of index
    for (uint8_t index = 0; index < 64; index++){
        freq_table[index] = array[index_zigzag[index]];
    }
}

void quantification_inverse(int16_t *freq_table, uint8_t *quant_table){
  /**
* @brief reverse quantification
* @param [in] freq_table
* @param [in] quant_table
* @return transformed freq_table after multiplication of elements from both arguments one by one
*/

    for (int8_t i = 0; i < 64; i++) {
        freq_table[i] = freq_table[i]*quant_table[i];
    }
}

void testiqzz(){
    printf("%s\n", "inner test");
}
