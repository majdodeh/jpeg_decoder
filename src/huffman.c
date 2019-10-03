/**
* @file huffman.c
* @author Odeh, Morin, Pouget
* @date 7 Jun 2019
* @copyright 2019 Odeh, Morin, Pouget
* @brief execute our own implementation of Huffman representation
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include "bitstream.h"
#include "huffman.h"

struct huff_table{
  /**
* @brief definition of huffman structure
* @details 5 parameters father, 2 sons, symbol associated and leaf to know if it's a leaf !
*/

    struct huff_table *father;
    struct huff_table *left_son;
    struct huff_table *right_son;
    uint8_t symbol;
    bool leaf;
};

struct huff_table *add_node(struct huff_table *root){
  /**
* @brief add a new node to the table
* @param [in] root : father of the new node
* @return son : new node
*/
    struct huff_table *son=malloc(sizeof(struct huff_table));
    son->father = root;
    son->left_son = NULL;
    son->right_son = NULL;
    son->leaf = false;
    return son;
}

struct huff_table *init_huffman_table(){
  /**
* @brief initialization of a table
* @return huffman_table : new table
*/

    struct huff_table *huffman_table=malloc(sizeof(struct huff_table));
    huffman_table->father = NULL;
    huffman_table->left_son = NULL;
    huffman_table->right_son = NULL;
    huffman_table->leaf = false;
    return huffman_table;
}

struct huff_table *load_huffman_table(struct bitstream *stream, uint16_t *nb_byte_read){
/**
* @brief load an huffman table
* @param [in] stream <parameter_description>
* @param [in] nb_byte_read : number of byte read
* @return huffman_table : loaded huffman table
* @details use of function init_huffman_table to create a new table, function read_bitstream to read data and function add_node to implemente huffman table
*/

    // init Huffman Table
    struct huff_table *huffman_table = init_huffman_table();

    // declare array contains the number of symbols of lengths from 1 to 16
    uint32_t length[16];
    // init the sum of numbers to 0
    uint16_t sum = 0;
    for (uint8_t index = 0; index < 16; index++){
        *nb_byte_read += read_bitstream(stream, 8, &length[index], false);
        sum += length[index];
    }

    // declare array of symbols
    uint32_t symbols[sum];
    for (uint8_t index = 0; index < sum; index++) {
        // each symbol is coded using 1 byte
        *nb_byte_read += read_bitstream(stream, 8, &symbols[index], false);
    }

    // declare counter to count how many symbols we inserted in Huffman tree
    uint8_t counter = 0;

    // declare and init the current depth level in Huffman tree ==> ROOT = 0
    uint8_t current_depth_level = 0;

    // levels [index+1] ==> from 1 to 16
    for (uint8_t index = 0; index < 16; index++){
        // get symbols number in level [index]
        uint32_t symbols_nb = length[index];
        uint8_t depth_levels_left = (index + 1) - current_depth_level;

        // insert each level symbols in Huffman tree
        for (uint16_t symbol_id = 0; symbol_id < symbols_nb; symbol_id++){

            // INSERT IN HUFFMAN TREE
            // search for empty place in huffman tree while the depth level is not reached yet
            while (depth_levels_left > 0) {
                // add node if the left son is NULL
                if (huffman_table->left_son == NULL){
                    struct huff_table *huffman_table_left_son = add_node(huffman_table);
                    huffman_table->left_son = huffman_table_left_son;
                    huffman_table = huffman_table->left_son;
                    depth_levels_left -= 1;
                    current_depth_level +=1;
                }
                // add node if the right son is NULL and the left one IS NOT NULL
                else if (huffman_table->right_son == NULL){
                    struct huff_table *huffman_table_right_son = add_node(huffman_table);
                    huffman_table->right_son = huffman_table_right_son;
                    huffman_table = huffman_table->right_son;
                    depth_levels_left -= 1;
                    current_depth_level +=1;
                }
                // if none of the sons is NULL, we need to go up Huffman tree to the other side
                else {
                    huffman_table = huffman_table->father;
                    depth_levels_left += 1;
                    current_depth_level -=1;
                }
            }

            // after inserting the symbol, it's a leaf
            huffman_table->leaf = true;
            // attribute symbol code from the symbols array and using the counter variable
            huffman_table->symbol = symbols[counter];
            // increase counter after inserting a symbole in Huffman Tree
            counter += 1;
            // go back to the father of the inserted symbol
            huffman_table = huffman_table->father;
            depth_levels_left += 1;
            current_depth_level -=1;
        }
    }

    // AFTER INSERTING ALL symbols
    // go back to ROOT
    while (huffman_table->father != NULL) {
        huffman_table = huffman_table->father;
    }

    return huffman_table;
}


int8_t next_huffman_value(struct huff_table *huffman_table,struct bitstream *stream){
/**
* @brief iterate on huffman table to find a leaf and return its value
* @param [in] huffman_table
* @param [in] stream
* @return symbol : associated to a leaf
*/

    // find the Symbol in HUFFMAN TREE by reading from bitstream
    while (huffman_table->leaf == false){

        // declare current bit
        uint32_t current_bit;
        // read 1 bit in bitstream
        read_bitstream(stream, 1, &current_bit, true);

        // test if the next son in Huffman table exists, or print ERROR message and exit
        if (current_bit == 0){
            if (huffman_table->left_son == NULL){
                fprintf(stderr, "ERROR IN HUFFMAN : Huffman sequence is not valid in bit stream\n");
                exit(1);
            }
            huffman_table = huffman_table->left_son;
        } else {
            if (huffman_table->right_son == NULL){
                fprintf(stderr, "ERROR IN HUFFMAN : Huffman sequence is not valid in bit stream\n");
                exit(1);
            }
            huffman_table = huffman_table->right_son;
        }
    }

    // get symbol after reaching the target leaf
    int8_t symbol = (int8_t)huffman_table->symbol;

    // AFTER getting the symbol, we should go back to ROOT
    while (huffman_table->father != NULL) {
        huffman_table = huffman_table->father;
    }

    return symbol;
}

void free_huffman_table(struct huff_table *huffman_table){
  /**
* @brief free memory for huffman table
* @param [in] huffman_table
*/

    // RECURSIVE FREE MEMORY FOR HUFFMAN TABLE
    // check if Huffman tree is not NULL
    if (huffman_table != NULL) {
        // if element in Huffman tree is not leaf, we should free its left and right sons
        if (huffman_table->leaf == false) {
            free_huffman_table(huffman_table->left_son);
            free_huffman_table(huffman_table->right_son);
        }
        free(huffman_table);
    }
}
