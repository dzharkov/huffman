#include "encoder.h"

huffman_encoder::huffman_encoder() {
}

huffman_encoder::~huffman_encoder() {
}

void huffman_encoder::allocate_resources() {
    codes = new code_bit_t*[max_codes_count];
    
    for (size_t i = 0; i < max_codes_count; i++) {
        codes[i] = NULL;
    }

    codes_length = new unsigned char[max_codes_count];
}

void huffman_encoder::free_resources() {
    for (size_t i = 0; i < max_codes_count; i++) {
        if (codes[i] != NULL) {
            delete [] codes[i];
        }
    }
    delete [] codes;
    delete [] codes_length;
}

void huffman_encoder::encode_with_tree(code_node* tree, std::istream & ins, std::ostream & outs) {
    allocate_resources();

    build_table(tree);

    obstream obs(outs);

    write_header(outs, obs);
    
    write_body(ins, obs);

    obs.close();

    free_resources();
}

void huffman_encoder::build_table(code_node* root) { 
    code_bit_t* prefix = new code_bit_t[max_codes_count];
    prefix[0] = 0;
    build_table(root, prefix, 0); 
    delete [] prefix;
}

void huffman_encoder::write_header(std::ostream &outs, obstream &obs) {
    size_t size = ordered_chars.size();

    outs.put((unsigned char)( (size >> 8) & 0xff ));
    outs.put((unsigned char)( size & 0xff ));
    
    for (size_t i = 0; i < ordered_chars.size(); i++) {
        outs.put(ordered_chars[i]);
    }

    for (size_t i = 0; i < encoded_tree_bits.size(); i++) {
        obs.write_bit(encoded_tree_bits[i]);
    }
}

void huffman_encoder::write_encoded_char(code_char_t c, obstream &obs) {
   for (size_t i = 0; i < codes_length[c];  i++) {
        obs.write_bit(codes[c][i]);
   }
}

void huffman_encoder::write_body(std::istream &in, obstream &obs) {
    
    code_char_t next = in.get();

    while (in.good()) {
        write_encoded_char(next, obs);

        next = in.get();
    }
}

void huffman_encoder::build_table(code_node* root, code_bit_t* prefix, size_t prefix_length) {
    encoded_tree_bits.push_back(root->is_leaf());

    if (root->is_leaf()) {
        code_char_t cur_char = root->get_char();
        ordered_chars.push_back(cur_char);
        
        //For files with unique char
        if (prefix_length == 0) {
            prefix_length++;
        }

        codes[cur_char] = new code_bit_t[prefix_length];

        for (size_t i = 0; i < prefix_length; i++) {
            codes[cur_char][i] = prefix[i];
        }

        codes_length[cur_char] = prefix_length;
    } else {
        for (code_bit_t i = 0; i < 2; i++) {
            prefix[prefix_length] = i;
            build_table(root->child_by_bit(i), prefix, prefix_length + 1);
        }
    }
}
