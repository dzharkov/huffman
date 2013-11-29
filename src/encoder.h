#include <iostream>
#include <vector>
#include "huffman.h"
#include "code_node.h"
#include "bstream.h"

#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

class huffman_encoder {
    public:
        huffman_encoder();
        ~huffman_encoder();
        
        void encode_with_tree(code_node* tree, std::istream & ins, std::ostream & outs); 
        
    private:
        huffman_encoder(const huffman_encoder &a);
        huffman_encoder & operator=(const huffman_encoder &a);
        
        void build_table(code_node* root); 
        
        void allocate_resources();
        void free_resources();

        void write_header(std::ostream &out, obstream &obs);
        void write_body(std::istream &in, obstream &obs);
        void build_table(code_node* root, code_bit_t* prefix, size_t prefix_length);
        void write_encoded_char(code_char_t c, obstream &obs);
        
        std::vector< code_char_t > ordered_chars;
        std::vector< bool > encoded_tree_bits;
        code_bit_t** codes;
        unsigned char* codes_length;
};

#endif
