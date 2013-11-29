#include <stdlib.h>
#include <stdint.h>
#include <iostream>

#ifndef HUFFMAN_H
#define HUFFMAN_H

#ifdef DEBUG
#define DBG(x) std::cerr << x << std::endl
#else
#define DBG(x)
#endif

typedef unsigned char code_char_t;
typedef unsigned char code_bit_t;
const size_t code_size = 8;
const size_t max_codes_count = 256;
typedef uint64_t code_weight_t;

inline size_t nodes_count_by_chars_count(size_t chars_count) {
    if (chars_count == 0) {
        return 0;
    }
    return 2 * chars_count - 1;
}


int huffman_compress(char const * input_file, char const * output_file);
int huffman_decompress(char const * input_file, char const * output_file);

#endif
