#include <stdio.h>

#include "submodules/deque_in_array/include/deque.h"

#include "include/avl_tree.h"

#define ITEM_TYPE uint64_t
#define COMPARATOR uint64_comparator


int uint8_comparator(const void* item1, const void* item2)
{
    return (*(uint8_t*)item1 >= *(uint8_t*)item2) - (*(uint8_t*)item1 <= *(uint8_t*)item2);
}

int uint16_comparator(const void* item1, const void* item2)
{
    return (*(uint16_t*)item1 >= *(uint16_t*)item2) - (*(uint16_t*)item1 <= *(uint16_t*)item2);
}

int uint32_comparator(const void* item1, const void* item2)
{
    return (*(uint32_t*)item1 >= *(uint32_t*)item2) - (*(uint32_t*)item1 <= *(uint32_t*)item2);
}

int uint64_comparator(const void* item1, const void* item2)
{
    return (*(uint64_t*)item1 >= *(uint64_t*)item2) - (*(uint64_t*)item1 <= *(uint64_t*)item2);
}

void get_random_data(void* storage, uint32_t n_bytes)
{
    FILE* source = fopen("/dev/urandom", "rb");
    fread(storage, 1, n_bytes, source);
    fclose(source);
}

bool check_sorted_sequence(
    void* storage, uint64_t items_count, uint32_t item_size,
    int(*comparator)(const void*, const void*), bool ascending
)
{
    if (items_count == 0)
    {
        return true;
    }
    for (uint64_t i = 0; i < items_count - 1; i++)
    {
        int comparing_result = comparator(
            (void*)((uint8_t*)storage + (item_size * i)),
            (void*)((uint8_t*)storage + (item_size * (i + 1)))
        );
        if (((comparing_result < 0) && !ascending) || ((comparing_result > 0) && ascending))
        {
            return false;
        }
    }
    return true;
}

int main()
{
    search_tree tree;
    search_tree_init(&tree, sizeof(ITEM_TYPE), COMPARATOR);
    for (uint32_t i = 0; i < 24; i++)
    {
        ITEM_TYPE new_item;
        get_random_data(&new_item, sizeof(new_item));
        tree.vtbl.insert(&tree, &new_item);
    }
    deque tree_structure_storage;
    //...
}
