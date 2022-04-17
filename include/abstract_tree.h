#ifndef ABSTRACT_TREE_H
#define ABSTRACT_TREE_H

#include <stdint.h>
#include <stddef.h>

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

typedef struct
{
    const void*(*find_first)(const struct abstract_tree*, const void* value);
    void(*insert)(struct abstract_tree*, const void* value);
    bool(*remove_first)(struct abstract_tree*, const void* value);
    unsigned_int_type(*remove_all)(struct abstract_tree*, const void* value);
    void(*clear)(struct abstract_tree*);
    void(*deinit)(struct abstract_tree*);
    void (*find_all)(
        const struct abstract_tree*,
        const void* value,
        void(*receiver)(const void* value, void* params),
        void* receiver_params
    );
    void(*traversal)(
        const struct abstract_tree*,
        traversal_method method,
        void(*receiver)(const void* value, void* params),
        void* receiver_params
    );
    size_t (*traversal_by_level)(
        const struct abstract_tree*,
        bool include_empty,
        const void* value_for_empty_parts,
        void(*receiver)(const void* value, void* params),
        void* receiver_params
    );
}
abstract_tree_vtbl;

typedef struct abstract_tree
{
    size_t value_size;
    // comparator should return n where:
    // n>0 if item1>item2,
    // n==0 if item1==item2 and
    // n<0 if item1<item2
    signed_int_type(*comparator)(const void*, const void*);
    abstract_tree_vtbl* vtbl;
}
abstract_tree;


#endif
