#include "decoder.h"

void huffman_decoder::decode_file(std::istream & ins, std::ostream & outs) {
    inbs = new ibstream(ins);
    
    chars_count = (((size_t)ins.get()) << 8) | ((size_t)ins.get());
    if (chars_count == 0) {
        return;
    }

    read_tree(ins);
    
    decode_and_write(outs);   
    
    free_resources();
}

void huffman_decoder::decode_and_write(std::ostream &outs) {
    code_char_t c = 0;
    while (read_next_char(c)) {
        outs.put(c);
    }
}

bool huffman_decoder::read_next_char(code_char_t &c) {
    if (inbs->eof()) {
        return false;
    }

    code_node* node = get_root();
    
    if (node->is_leaf()) {
        inbs->read_bit(); 
    } else {
        while (!node->is_leaf()) {
            bool bit = inbs->read_bit();
            node = node->child_by_bit( bit );
        }
    }

    c = node->get_char();

    return true;
}

void huffman_decoder::read_tree(std::istream & ins) {
    nodes_count = nodes_count_by_chars_count(chars_count);
    chars = std::queue< code_char_t >();
    
    for (size_t i = 0; i < chars_count; i++) {
        chars.push(ins.get());
    }

    nodes = new code_node*[nodes_count];
    
    used_nodes_count = 1;
    read_tree(0);
}

void huffman_decoder::read_tree(size_t node_offset) {
    code_node* node;
    bool is_leaf = inbs->read_bit();

    if (is_leaf) {
        node = new code_node(chars.front());
        chars.pop();
    } else {
        size_t l = used_nodes_count++;
        read_tree(l);
        
        size_t r = used_nodes_count++;
        read_tree(r);
        node = new code_node(nodes[l], nodes[r]);
    }

    nodes[node_offset] = node;
}

void huffman_decoder::free_resources() {
    if (nodes != NULL) {
        for (size_t i = 0; i < nodes_count; i++) {
            delete nodes[i];
        }

        delete [] nodes;
    }
    if (inbs != NULL) {
        delete inbs;
    }
}
 
