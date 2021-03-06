#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "search_tree.h"


typedef struct avl_tree_node
{
    search_tree_node super;
    int8_t disbalance;
}
avl_tree_node;

typedef struct avl_tree
{
    avl_tree_node super;
}
avl_tree;


void avl_tree_init(
    avl_tree* self,
    unsigned_int_type value_size,
    signed_int_type(*comparator)(void*, void*)
);

avl_tree* new_avl_tree_node(
    avl_tree* tree,
    unsigned_int_type value_size,
    void* value,
    avl_tree_node* parent,
    avl_tree_node* left,
    avl_tree_node* right
);

void avl_tree_insert(avl_tree* self, void* value);

bool avl_tree_remove_first(avl_tree* self, void* value);

unsigned_int_type avl_tree_remove_all(avl_tree* self, void* value);

#endif
