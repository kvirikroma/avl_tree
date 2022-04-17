#include <stdlib.h>

#include "submodules/deque_in_array/include/deque.h"

#include "include/printable_tree.h"


typedef struct
{
    deque* destination;
    unsigned_int_type* item_counter;
}
item_adder_params;



printable_tree* add_printable_mixin(abstract_tree* tree)
{
    printable_tree_vtbl* printable_tree_vtbl_instance = (printable_tree_vtbl*)malloc(
        sizeof(printable_tree_vtbl)
    );
    *printable_tree_vtbl_instance = (printable_tree_vtbl){
        .super_vtbl = *(tree->vtbl),
        .super_deinit = ((abstract_tree*)tree)->vtbl->deinit,
        .print = printable_tree_print
    };
    tree->vtbl = (abstract_tree_vtbl*)printable_tree_vtbl_instance;
    tree->vtbl->deinit = (void (*)(abstract_tree*))printable_tree_deinit;
    return (printable_tree*)tree;
}

void printable_tree_deinit(printable_tree* tree)
{
    ((printable_tree_vtbl*)((abstract_tree*)tree)->vtbl)->super_deinit((abstract_tree*)tree);
    free(((abstract_tree*)tree)->vtbl);
}

static void add_item_to_storage(void* item, item_adder_params* parameters)
{
    if (!deque_can_pop(parameters->destination))
    {
        deque root_level;
        deque_init(&root_level, NULL, 0, sizeof(void*));
        deque_push_right(parameters->destination, (deque_item*)&root_level);
    }
    //...
}

void printable_tree_print(
    printable_tree* self,
    void(*to_string)(void*, char* dest, size_t max_length),
    size_t max_length_of_value
)
{
    deque tree_structure_storage;
    deque_init(&tree_structure_storage, NULL, 0, sizeof(deque));
    unsigned_int_type item_counter = 0;
    item_adder_params adder_params = (item_adder_params){
        .destination=&tree_structure_storage,
        .item_counter=&item_counter
    };
    ((abstract_tree*)self)->vtbl->traversal_by_level(
        (abstract_tree*)self,
        true,
        NULL,
        (void(*)(const void* value, void* params))add_item_to_storage,
        &adder_params
    );
    //...
}
