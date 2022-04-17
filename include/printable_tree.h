#ifndef PRINTABLE_TREE_H
#define PRINTABLE_TREE_H

#include "include/abstract_tree.h"


typedef struct
{
    abstract_tree super;
}
printable_tree;


typedef struct
{
    abstract_tree_vtbl super_vtbl;
    void(*super_deinit)(abstract_tree* tree);
    void(*print)(
        printable_tree* self,
        void(*to_string)(void*, char* dest, size_t max_length),
        size_t max_length_of_value
    );
}
printable_tree_vtbl;


/// dont use it twice on the same object!
/// otherwise it will cause memory leak
printable_tree* add_printable_mixin(abstract_tree* tree);

void printable_tree_deinit(printable_tree* tree);

void printable_tree_print(
    printable_tree* self,
    void(*to_string)(void*, char* dest, size_t max_length),
    size_t max_length_of_value
);


#endif
