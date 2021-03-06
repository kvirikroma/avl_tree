#include <stdlib.h>
#include <string.h>

#include "include/search_tree.h"
#include "submodules/deque_in_array/include/deque.h"


typedef struct
{
    void(*value_receiver)(const void* value, void* params);
    void* receiver_params;
}
node_receiver_params;


static void node_receiver_for_traversal(search_tree_node* node, node_receiver_params* params)
{
    params->value_receiver(node->vtbl->get_value(node), params->receiver_params);
}

static void node_receiver_for_deletion(search_tree_node* node, void* params)
{
    free(node);
}

void search_tree_init(
    search_tree* self,
    size_t value_size,
    signed_int_type(*comparator)(const void*, const void*)
)
{
    static abstract_tree_vtbl search_tree_vtbl_instance = {
        .find_first = (const void*(*)(const struct abstract_tree*, const void* value))search_tree_find_first,
        .insert = (void(*)(struct abstract_tree*, const void* value))search_tree_insert,
        .remove_first = (bool(*)(struct abstract_tree*, const void* value))search_tree_remove_first,
        .remove_all = (unsigned_int_type(*)(struct abstract_tree*, const void* value))search_tree_remove_all,
        .clear = (void(*)(struct abstract_tree*))search_tree_clear,
        .deinit = (void(*)(struct abstract_tree*))search_tree_deinit,
        .find_all = (void(*)(
            const struct abstract_tree*,
            const void* value,
            void(*receiver)(const void* value, void* params),
            void* receiver_params
        ))search_tree_find_all,
        .traversal = (void(*)(
            const struct abstract_tree*,
            traversal_method method,
            void(*receiver)(const void* value, void* params),
            void* receiver_params
        ))search_tree_traversal,
        .traversal_by_level = (size_t (*)(
            const struct abstract_tree*,
            bool include_empty,
            const void* value_for_empty_parts,
            void(*receiver)(const void* value, void* params),
            void* receiver_params
        ))search_tree_traversal_by_level
    };
    ((abstract_tree*)self)->vtbl = &search_tree_vtbl_instance;
    ((abstract_tree*)self)->comparator = comparator;
    self->nodes_count = 0;
    self->root = NULL;
    ((abstract_tree*)self)->value_size = value_size;
}

const void* search_tree_node_get_value(const search_tree_node* self)
{
    return (const void*)(self + 1);
}

size_t search_tree_node_get_depth(const search_tree_node* self)
{
    size_t result = 0;
    const search_tree_node* current_node = self;
    while (current_node->parent)
    {
        current_node = current_node->parent;
        result++;
    }
    return result;
}

search_tree_node* new_search_tree_node(
    size_t value_size,
    const void* value,
    search_tree_node* parent,
    search_tree_node* left,
    search_tree_node* right
)
{
    search_tree_node* result = (search_tree_node*)malloc(
        sizeof(search_tree_node) + value_size
    );
    search_tree_node_vtbl search_tree_node_vtbl_instance = (search_tree_node_vtbl){
        .get_value = search_tree_node_get_value,
        .get_depth = search_tree_node_get_depth
    };
    result->vtbl = &search_tree_node_vtbl_instance;
    result->parent = parent;
    result->left = left;
    result->right = right;
    memcpy((void*)(result + 1), value, value_size);
    return result;
}

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

void search_tree_clear(search_tree* self)
{
    traversal_for_nodes(self->root, TRAVERSAL_METHOD_POSTORDER, node_receiver_for_deletion, NULL);
    self->nodes_count = 0;
}

void search_tree_deinit(search_tree* self)
{
    ((abstract_tree*)self)->vtbl->clear((abstract_tree*)self);
    ((abstract_tree*)self)->value_size = 0;
    self->root = NULL;
    ((abstract_tree*)self)->comparator = NULL;
    ((abstract_tree*)self)->vtbl = NULL;
}

static search_tree_node* find_first_node(const search_tree* self, const void* value)
{
    search_tree_node* node = self->root;
    while (node)
    {
        signed_int_type comparing_result = ((abstract_tree*)self)->comparator(
            node->vtbl->get_value(node), value
        );
        if (comparing_result > 0)  // node.get_value() > value
        {
            node = node->left;
        }
        else if (comparing_result < 0)  // node.get_value() < value
        {
            node = node->right;
        }
        else
        {
            return node;
        }
    }
    return NULL;
}

const void* search_tree_find_first(const search_tree* self, const void* value)
{
    const search_tree_node* node = find_first_node(self, value);
    return node->vtbl->get_value(node);
}

static void search_traversal(
    const search_tree* self,
    const search_tree_node* node,
    const void* value,
    void(*receiver)(const void* value, void* params),
    void* receiver_params
)
{
    if (node && (((abstract_tree*)self)->comparator(node->vtbl->get_value(node), value) == 0))
    {
        search_traversal(self, node->left, value, receiver, receiver_params);
        receiver(node->vtbl->get_value(node), receiver_params);
        search_traversal(self, node->right, value, receiver, receiver_params);
    }
}

void search_tree_find_all(
    const search_tree* self,
    const void* value,
    void(*receiver)(const void* value, void* params),
    void* receiver_params
)
{
    const search_tree_node* first_node = find_first_node(self, value);
    search_traversal(self, first_node, value, receiver, receiver_params);
}

void search_tree_insert(search_tree* self, const void* value)
{
    search_tree_node* node = self->root;
    while (node)
    {
        signed_int_type comparing_result = ((abstract_tree*)self)->comparator(node->vtbl->get_value(node), value);
        if (comparing_result > 0)  // node.get_value() > value
        {
            if (node->left)
            {
                node = node->left;
            }
            else
            {
                node->left = new_search_tree_node(((abstract_tree*)self)->value_size, value, node, NULL, NULL);
                node = NULL;
            }
        }
        else  // node.get_value() <= value
        {
            if (node->right)
            {
                node = node->right;
            }
            else
            {
                node->right = new_search_tree_node(
                    ((abstract_tree*)self)->value_size, value, node, NULL, NULL
                );
                node = NULL;
            }
        }
    }
}

static void remove_node_from_tree(search_tree* self, search_tree_node* node, bool free_memory)
{
    if (node->right)
    {
        if (node->left)  // node has both children
        {
            search_tree_node* replacement = node->right;
            while (replacement->left)
            {
                replacement = replacement->left;
            }
            remove_node_from_tree(self, replacement, false);
            replacement->left = node->left;
            replacement->right = node->right;
            replacement->parent = node->parent;
            if (node->left)
            {
                node->left->parent = replacement;
            }
            if (node->right)
            {
                node->right->parent = replacement;
            }
            if (node->parent)
            {
                if (node->parent->left == node)
                {
                    node->parent->left = replacement;
                }
                else
                {
                    node->parent->right = replacement;
                }
            }
            else
            {
                self->root = replacement;
            }
        }
        else  // node has only right child
        {
            if (node->parent)
            {
                if (node->parent->left == node)
                {
                    node->parent->left = node->right;
                }
                else
                {
                    node->parent->right = node->right;
                }
            }
            else
            {
                self->root = node->right;
            }
            node->right->parent = node->parent;
        }
    }
    else  // node has only left child or none of them
    {
        if (node->parent)
        {
            if (node->parent->left == node)
            {
                node->parent->left = node->left;
            }
            else
            {
                node->parent->right = node->left;
            }
        }
        else
        {
            self->root = node->left;
        }
        if (node->left)  // node has only left child
        {
            node->left->parent = node->parent;
        }
    }
    node->left = node->right = node->parent = NULL;
    if (free_memory)
    {
        free(node);
    }
}

bool search_tree_remove_first(search_tree* self, const void* value)
{
    search_tree_node* node_to_remove = find_first_node(self, value);
    if (!node_to_remove)
    {
        return false;
    }
    remove_node_from_tree(self, node_to_remove, true);
    return true;
}

static void deletion_traversal(
    search_tree* self,
    search_tree_node* node,
    const void* value,
    unsigned_int_type* counter
)
{
    if (node && (((abstract_tree*)self)->comparator(node->vtbl->get_value(node), value) == 0))
    {
        deletion_traversal(self, node->left, value, counter);
        deletion_traversal(self, node->right, value, counter);
        remove_node_from_tree(self, node, true);
        counter++;
    }
}

unsigned_int_type search_tree_remove_all(search_tree* self, const void* value)
{
    unsigned_int_type counter = 0;
    deletion_traversal(self, find_first_node(self, value), value, &counter);
    return counter;
}

void search_tree_traversal(
    const search_tree* self,
    traversal_method method,
    void(*receiver)(const void* value, void* params),
    void* receiver_params
)
{
    node_receiver_params params = (node_receiver_params){
        .value_receiver=receiver,
        .receiver_params=receiver_params
    };
    traversal_for_nodes(
        self->root, method,
        (void(*)(search_tree_node* node, void* params))node_receiver_for_traversal,
        &params
    );
}

size_t search_tree_traversal_by_level(
    const search_tree* self,
    bool include_empty,
    const void* value_for_empty_parts,
    void(*receiver)(const void* value, void* params),
    void* receiver_params
)
{
    if (!self->root)
    {
        return 0;
    }
    search_tree_node* null_node = NULL;
    search_tree_node* current_node = NULL;
    search_tree_node* last_node = NULL;
    deque nodes_queue;
    deque_init(&nodes_queue, NULL, 0, sizeof(search_tree_node*));
    deque_push_right(&nodes_queue, (deque_item*)&(self->root));
    while (deque_can_pop(&nodes_queue))
    {
        deque_pop_left(&nodes_queue, (deque_item*)&current_node);
        if (current_node)
        {
            last_node = current_node;
            if (current_node->left || include_empty)
            {
                deque_push_right(&nodes_queue, (deque_item*)&(current_node->left));
            }
            if (current_node->right || include_empty)
            {
                deque_push_right(&nodes_queue, (deque_item*)&(current_node->right));
            }
            receiver(current_node->vtbl->get_value(current_node), receiver_params);
        }
        else if (include_empty)
        {
            deque_push_right(&nodes_queue, &null_node);
            receiver(value_for_empty_parts, receiver_params);
        }
    }
    deque_delete(&nodes_queue);
    if (!last_node)
    {
        return 0;
    }
    return last_node->vtbl->get_depth(last_node) + 1;
}
