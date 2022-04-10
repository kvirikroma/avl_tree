#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H

#include <stdint.h>
#include <stdbool.h>

#define MACHINE_WORD_64  // comment it out if your machine is 32-bit

#ifdef MACHINE_WORD_64
typedef uint64_t unsigned_int_type;
typedef int64_t signed_int_type;
#else
typedef uint32_t unsigned_int_type;
typedef int32_t signed_int_type;
#endif

typedef enum 
{
    PREORDER,
    POSTORDER,
    INORDER,
    BY_LEVEL
}
traversal_method;


typedef struct search_tree_node
{
    struct search_tree_node* parent;
    struct search_tree_node* left;
    struct search_tree_node* right;
    struct {
        void*(*get_value)(struct search_tree_node*);
    } vtbl;
    uint8_t _value[];
}
search_tree_node;


typedef struct search_tree
{
    search_tree_node* root;
    unsigned_int_type value_size;
    unsigned_int_type nodes_count;
    int8_t(*comparator)(void*, void*);
    struct {
        void*(*find)(struct search_tree*, void* value);
        void(*insert)(struct search_tree*, void* value);
        bool(*remove)(struct search_tree*, void* value);
        void(*traversal)(struct search_tree*, traversal_method method, void(*receiver)(void* value, void* params), void* receiver_params);
        void(*deinit)(struct search_tree*);
    } vtbl;
}
search_tree;


void* find(search_tree*, void*);

void insert(search_tree*, void*);

bool remove(search_tree*, void*);

void traversal(search_tree*, void(*receiver)(void* value, void* params), void* receiver_params);

void deinit(search_tree*);


#endif
