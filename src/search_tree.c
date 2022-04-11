#include <stdlib.h>
#include <string.h>

#include <include/search_tree.h>

void search_tree_init(
    search_tree* self,
    unsigned_int_type value_size,
    int8_t(*comparator)(void*, void*)
)
{
    self->vtbl.deinit = search_tree_deinit;
    self->vtbl.find = search_tree_find;
    self->vtbl.insert = search_tree_insert;
    self->vtbl.remove = search_tree_remove;
    self->vtbl.traversal = search_tree_traversal;
    self->comparator = comparator;
    self->nodes_count = 0;
    self->root = NULL;
    self->value_size = value_size;
}

static void* search_tree_node_get_value(search_tree_node* self)
{
    return (void*)(self + 1);
}

search_tree_node* new_search_tree_node(
    search_tree* tree,
    unsigned_int_type value_size,
    void* value,
    search_tree_node* parent,
    search_tree_node* left,
    search_tree_node* right
)
{
    search_tree_node* result = (search_tree_node*)malloc(
        sizeof(search_tree_node) + tree->value_size
    );
    result->vtbl.get_value = search_tree_node_get_value;
    result->parent = parent;
    result->left = left;
    result->right = right;
    memcpy(search_tree_node_get_value(result), value, value_size);
}

void search_tree_deinit(search_tree* self);

void* search_tree_find(search_tree* self, void* value);

void search_tree_insert(search_tree* self, void* value);

bool search_tree_remove(search_tree* self, void* value);

void search_tree_traversal(
    search_tree*,
    void(*receiver)(void* value, void* params),
    void* receiver_params
);
