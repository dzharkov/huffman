#include "code_node.h"

code_node::code_node(code_node* l, code_node* r) {
    init();
    set_children(l, r);
}

void code_node::init() {
    children = new code_node*[2];
    children[0] = children[1] = NULL;
}

void code_node::set_children(code_node* l, code_node* r) {
    children[0] = l;
    children[1] = r;
}

weighted_code_node::weighted_code_node(weighted_code_node* l, weighted_code_node* r) : code_node(l, r) {
    weight = l->get_weight() + r->get_weight();
}

void weighted_code_node::set_children(weighted_code_node* l, weighted_code_node* r) {
    code_node::set_children(l, r);
    weight = l->get_weight() + r->get_weight();
}
