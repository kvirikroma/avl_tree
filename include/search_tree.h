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
    TRAVERSAL_METHOD_PREORDER,
    TRAVERSAL_METHOD_POSTORDER,
    TRAVERSAL_METHOD_INORDER
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
}
search_tree_node;

typedef struct search_tree
{
    search_tree_node* root;
    unsigned_int_type value_size;
    unsigned_int_type nodes_count;
    // comparator should return n where:
    // n>0 if item1>item2,
    // n==0 if item1==item2 and
    // n<0 if item1<item2
    signed_int_type(*comparator)(void*, void*);
    struct {
        void*(*find)(struct search_tree*, void* value);
        void(*insert)(struct search_tree*, void* value);
        bool(*remove_first)(struct search_tree*, void* value);
        unsigned_int_type(*remove_all)(struct search_tree*, void* value);
        void(*clear)(struct search_tree*);
        void(*deinit)(struct search_tree*);
        void (*find_all)(
            struct search_tree*,
            void* value,
            void(*receiver)(void* value, void* params),
            void* receiver_params
        );
        void(*traversal)(
            struct search_tree*,
            traversal_method method,
            void(*receiver)(void* value, void* params),
            void* receiver_params
        );
        void (*traversal_by_level)(
            struct search_tree*,
            bool include_empty,
            void* value_for_empty_parts,
            void(*receiver)(void* value, void* params),
            void* receiver_params
        );
    } vtbl;
}
search_tree;


void search_tree_init(
    search_tree* self,
    unsigned_int_type value_size,
    signed_int_type(*comparator)(void*, void*)
);

search_tree_node* new_search_tree_node(
    unsigned_int_type value_size,
    void* value,
    search_tree_node* parent,
    search_tree_node* left,
    search_tree_node* right
);

void* search_tree_find(search_tree* self, void* value);

void search_tree_find_all(
    search_tree* self,
    void* value,
    void(*receiver)(void* value, void* params),
    void* receiver_params
);

void search_tree_insert(search_tree* self, void* value);

bool search_tree_remove_first(search_tree* self, void* value);

unsigned_int_type search_tree_remove_all(search_tree* self, void* value);

void search_tree_traversal(
    search_tree* self,
    traversal_method method,
    void(*receiver)(void* value, void* params),
    void* receiver_params
);

void search_tree_traversal_by_level(
    search_tree* self,
    bool include_empty,
    void* value_for_empty_parts,
    void(*receiver)(void* value, void* params),
    void* receiver_params
);

void search_tree_clear(search_tree* self);

void search_tree_deinit(search_tree* self);


#endif
