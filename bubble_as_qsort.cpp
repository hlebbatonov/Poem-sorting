#include "bubble_sort.h"


void BubbleSorting(void* data, size_t nmemb, size_t size, int(*compar)(const void*, const void*))
{
    char* byte_data = (char*)(data);

    for (size_t nums_to_sort = nmemb - 1; nums_to_sort > 0; nums_to_sort--)
    {
        for (size_t i = 0; i < nums_to_sort; i++)
        {
            void* first_elem = (char*)(byte_data + i * size);
            void* second_elem = (char*)(byte_data + (i + 1) * size );

            if (compar(first_elem, second_elem) > 0)
                ReplaceValues(first_elem, second_elem, size);
        }
    }
}

void ReplaceValues(void* x, void* y, size_t size)
{
    uint64_t* curr_block_x = (uint64_t*)x;
    uint64_t* curr_block_y = (uint64_t*)y;
    size_t num_of_blocks = size/sizeof(uint64_t);

    for (size_t i = 0; i < num_of_blocks; i++)
    {
        uint64_t temp = curr_block_x[i];
        curr_block_x[i] = curr_block_y[i];
        curr_block_y[i] = temp;
    }

    uint8_t* curr_x = (uint8_t*)x + num_of_blocks * sizeof(uint64_t);
    uint8_t* curr_y = (uint8_t*)y + num_of_blocks * sizeof(uint64_t);


    for (size_t i = 0; i < size % sizeof(uint64_t); i++)
    {
        uint8_t temp = curr_x[i];
        curr_x[i] = curr_y[i];
        curr_y[i] = temp;
    }
}
