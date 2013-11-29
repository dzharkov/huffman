#include <queue>
#include <iostream>
#include "huffman.h"
#include "code_node.h"
#include "bstream.h"

#ifndef HUFFMAN_DECODER_H
#define HUFFMAN_DECODER_H

class huffman_decoder {
    public:
        huffman_decoder() {
        }

        ~huffman_decoder() {
        }
        
        void decode_file(std::istream & ins, std::ostream &outs);
    private:
        huffman_decoder(const huffman_decoder &a);
        huffman_decoder & operator=(const huffman_decoder &a);
        
        void free_resources();

        code_node* get_root() {
            return nodes[0];
        } 
        
        void decode_and_write(std::ostream &outs);
        bool read_next_char(code_char_t &c);
        void read_tree(std::istream & ins); 
        void read_tree(size_t node_offset);
       
        ibstream* inbs; 
        code_node** nodes;
        std::queue< code_char_t > chars;
        size_t chars_count;
        size_t nodes_count;
        size_t used_nodes_count;
};

#endif
