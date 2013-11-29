#include <fstream>
#include <queue>
#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include "huffman.h"
#include "code_node.h"
#include "encoder.h"
#include "decoder.h"

size_t calculate_frequencies(std::vector< code_weight_t > &freq, std::ifstream &inf) {
    for (size_t i = 0; i < max_codes_count; i++) {
        freq[i] = 0;
    }
    
    size_t actual_codes_count = 0;

    code_char_t next = (code_char_t)inf.get();

    while (inf.good()) {
        if ((freq[next]++) == 0) {
            actual_codes_count++;
        }
        next = (code_char_t)inf.get();
    }

    return actual_codes_count;
}

struct dereference_comparator_for_weighted_node : public std::binary_function< weighted_code_node*, weighted_code_node*, bool> {
    bool operator()(const weighted_code_node* a, const weighted_code_node* b) const {
        return a->get_weight() > b->get_weight();
    }
};

code_node*  build_code_tree(weighted_code_node** nodes, size_t nodes_used) {
    std::priority_queue< weighted_code_node*, std::vector< weighted_code_node* >, dereference_comparator_for_weighted_node > q;
    
    for (size_t i = 0; i < nodes_used; i++) {
        q.push(nodes[i]);
    }

    while (q.size() > 1) {
        weighted_code_node* a = q.top(); q.pop();
        weighted_code_node* b = q.top(); q.pop();

        weighted_code_node* next = nodes[nodes_used++];
        next->set_children(a,b);

        q.push(next);
    }

    return q.top();
}

//returns leafs count
size_t build_tree_leafs(weighted_code_node** nodes, std::vector< code_weight_t > freq, size_t total_nodes_count) {
    size_t nodes_used = 0;

    for (size_t i = 0; i < max_codes_count; i++) {
        if (freq[i] > 0) {
            nodes[nodes_used++] = new weighted_code_node(i, freq[i]);
        }
    }

    for (size_t i = nodes_used; i < total_nodes_count; i++) {
        nodes[i] = new weighted_code_node();
    }

    return nodes_used;
}


int huffman_compress(char const * input_file, char const * output_file) {
    std::ifstream inf(input_file, std::ios::in | std::ios::binary);
    std::ofstream outf(output_file, std::ios::out | std::ios::trunc | std::ios::binary);
    
    std::vector< code_weight_t > freq(max_codes_count);
    
    size_t actual_codes_count = calculate_frequencies(freq, inf);
   
    //handling special case with empty file
    if (actual_codes_count == 0) {
        outf.put(0);
        outf.put(0);
        return 0;
    }
    
    //Nodes allocating
    size_t total_nodes_count = nodes_count_by_chars_count(actual_codes_count);
    weighted_code_node** nodes = new weighted_code_node*[total_nodes_count];
    
    //Leafs building
    size_t leafs_count = build_tree_leafs(nodes, freq, total_nodes_count);
    
    //Code tree building
    code_node* root = build_code_tree(nodes, leafs_count);
    
    huffman_encoder encoder;
    
    //Archiving input file by tree
    inf.clear();
    inf.seekg(0, std::ios::beg);
    
    encoder.encode_with_tree(root, inf, outf);

    //Nodes deleting
    for (size_t i = 0; i < total_nodes_count; i++) {
        delete nodes[i];
    }

    delete [] nodes;

    return 0;   
}

int huffman_decompress(char const * input_file, char const * output_file) {
    std::ifstream inf(input_file, std::ios::in | std::ios::binary);
    std::ofstream outf(output_file, std::ios::out | std::ios::trunc | std::ios::binary);
    
    huffman_decoder decoder;

    decoder.decode_file(inf, outf);

    return 0;
}
