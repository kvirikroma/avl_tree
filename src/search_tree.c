#include <stdlib.h>
#include <string.h>

#include "include/search_tree.h"
#include "submodules/deque_in_array/include/deque.h"


typedef struct
{
    void(*value_receiver)(void* value, void* params);
    void* receiver_params;
}
node_receiver_params;


static void node_receiver_for_traversal(search_tree_node* node, node_receiver_params* params)
{
    params->value_receiver(node->vtbl.get_value(node), params->receiver_params);
}

static void node_receiver_for_deletion(search_tree_node* node, void* params)
{
    free(node);
}

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

static void traversal_for_nodes(
    search_tree_node* node,
    traversal_method method,
    void(*receiver)(search_tree_node* node, void* params),
    void* receiver_params
)
{
    if (!node)
    {
        return;
    }
    switch (method)
    {
        case TRAVERSAL_METHOD_INORDER:
        {
            traversal_for_nodes(node->left, method, receiver, receiver_params);
            receiver(node, receiver_params);
            traversal_for_nodes(node->right, method, receiver, receiver_params);
            break;
        }
        case TRAVERSAL_METHOD_PREORDER:
        {
            receiver(node, receiver_params);
            traversal_for_nodes(node->left, method, receiver, receiver_params);
            traversal_for_nodes(node->right, method, receiver, receiver_params);
            break;
        }
        case TRAVERSAL_METHOD_POSTORDER:
        {
            traversal_for_nodes(node->left, method, receiver, receiver_params);
            traversal_for_nodes(node->right, method, receiver, receiver_params);
            receiver(node, receiver_params);
            break;
        }
        default:
        {
            break;
        }
    }
}

void search_tree_traversal(
    search_tree* self,
    traversal_method method,
    void(*receiver)(void* value, void* params),
    void* receiver_params
)
{
    node_receiver_params params = (node_receiver_params){
        .receiver_params=receiver_params,
        .value_receiver=receiver
    };
    traversal_for_nodes(self->root, method, node_receiver_for_traversal, &params);
}

void search_tree_traversal_by_level(
    search_tree*,
    bool include_empty,
    void* value_for_empty_parts,
    void(*receiver)(void* value, void* params),
    void* receiver_params
);
