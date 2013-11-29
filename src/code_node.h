#include "huffman.h"

#ifndef CODE_NODE_H
#define CODE_NODE_H

class code_node {
    public:
        
        code_node() { init(); }
        
        code_node(code_char_t c_) : c(c_) { init(); }
        
        
        code_node(code_node* l, code_node* r);
        
        virtual ~code_node() {
            delete [] children;
        }
        
        virtual void set_children(code_node* l, code_node* r);

        code_char_t get_char() const { return c; }
        
        bool is_leaf() { return children[0] == NULL; }

        code_node * child_by_bit(bool bit) const {
            return children[bit];
        }
    private:
        code_node(const code_node &a);
        code_node & operator=(const code_node &a);
        void init(); 
        
        code_node** children;
        code_char_t c;
};

class weighted_code_node: public code_node {
    public:
        weighted_code_node() : code_node(), weight(0) { }
        weighted_code_node(code_char_t c_, code_weight_t weight_) : code_node(c_), weight(weight_) {}
        weighted_code_node(weighted_code_node* l, weighted_code_node* r); 
        
        code_weight_t get_weight() const {
            return weight;
        }

        virtual void set_children(weighted_code_node* l, weighted_code_node* r);
    private:
        code_weight_t weight;
};

#endif
