#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "include/abstract_tree.h"


typedef struct search_tree_node_vtbl search_tree_node_vtbl;

typedef struct search_tree_node
{
    struct search_tree_node* parent;
    struct search_tree_node* left;
    struct search_tree_node* right;
    search_tree_node_vtbl* vtbl;
}
search_tree_node;

typedef struct search_tree_node_vtbl
{
    const void*(*get_value)(const struct search_tree_node*);
    size_t(*get_depth)(const struct search_tree_node*);
}
search_tree_node_vtbl;

typedef struct search_tree
{
    abstract_tree super;
    search_tree_node* root;
    unsigned_int_type nodes_count;
}
search_tree;


void search_tree_init(
    search_tree* self,
    size_t value_size,
    signed_int_type(*comparator)(const void*, const void*)
);

search_tree_node* new_search_tree_node(
    size_t value_size,
    const void* value,
    search_tree_node* parent,
    search_tree_node* left,
    search_tree_node* right
);

const void* search_tree_node_get_value(const search_tree_node* self);

size_t search_tree_node_get_depth(const search_tree_node* self);

const void* search_tree_find_first(const search_tree* self, const void* value);

void search_tree_find_all(
    const search_tree* self,
    const void* value,
    void(*receiver)(const void* value, void* params),
    void* receiver_params
);

void search_tree_insert(search_tree* self, const void* value);

bool search_tree_remove_first(search_tree* self, const void* value);

unsigned_int_type search_tree_remove_all(search_tree* self, const void* value);

void search_tree_traversal(
    const search_tree* self,
    traversal_method method,
    void(*receiver)(const void* value, void* params),
    void* receiver_params
);

// @return number of levels in tree
size_t search_tree_traversal_by_level(
    const search_tree* self,
    bool include_empty,
    const void* value_for_empty_parts,
    void(*receiver)(const void* value, void* params),
    void* receiver_params
);

void search_tree_clear(search_tree* self);

void search_tree_deinit(search_tree* self);


#endif
