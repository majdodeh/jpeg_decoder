#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "huffman.h"

struct huff_table{
    struct huff_table *father;
    struct huff_table *left_son;
    struct huff_table *right_son;
    uint8_t symbol;
    bool leaf;
};

static struct huff_table *add_node(struct huff_table *root){
    struct huff_table *son=malloc(sizeof(struct huff_table));
    son->father = root;
    son->left_son = NULL;
    son->right_son = NULL;
    son->leaf = false;
    return son;
}

static struct huff_table *init_huffman_table(){
    struct huff_table *huffman_table=malloc(sizeof(struct huff_table));
    huffman_table->father = NULL;
    huffman_table->left_son = NULL;
    huffman_table->right_son = NULL;
    huffman_table->leaf = false;
    return huffman_table;
}

static struct huff_table *load_huffman_table_test(uint32_t *length, uint32_t *symbols){

    // init Huffman Table
    struct huff_table *huffman_table = init_huffman_table();

    // init the sum of numbers to 0
    uint16_t sum = 0;
    for (uint8_t index = 0; index < 16; index++){
        sum += length[index];
    }

    // declare counter to count how many symbols we inserted in Huffman tree
    uint8_t counter = 0;

    // delcare and init the current depth level in Huffman tree ==> ROOT = 0
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


int main(){
    uint32_t length[16] = {0,3,2,0,0,0,0,0,0,0,0,0,0,0,0,0};

    for (uint8_t i = 0; i < 16; i++) {
        printf(" There is %x symbols of length : %d\n", length[i], i);
    }
    uint32_t symbols[5] = {1,2,3,4,5};

    printf("\n List of symbols :");
    for (uint8_t i = 0; i < 5; i++) {
        printf(" %x ", symbols[i]);
    }
    printf("\n");

    struct huff_table *huffman = load_huffman_table_test(length, symbols);

    printf("\n \n Huffman Tree :\n\n");
    printf(" Huffman Tree -> left_son -> left_son -> symbol = %d\n\n", huffman->left_son->left_son->symbol);
    printf(" Huffman Tree -> left_son -> right_son ->symbol = %d\n\n", huffman->left_son->right_son->symbol);
    printf(" Huffman Tree -> right_son -> left_son ->left_son -> symbol = %d\n\n", huffman->right_son->left_son->symbol);
    printf(" Huffman Tree -> right_son -> left_son ->right_son -> symbol = %d\n\n", huffman->right_son->right_son->left_son->symbol);
    printf(" Huffman Tree -> right_son -> right_son ->left_son -> left_son -> symbol = %d\n\n", huffman->right_son->right_son->right_son->symbol);

    printf("\n");
    free_huffman_table(huffman);

    return EXIT_SUCCESS;
}
