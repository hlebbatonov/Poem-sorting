#ifndef COMPARATORS_H
#define COMPARATORS_H
#include <cstddef>
#include <ctype.h>

int Compar(const void* a, const void* b);
int Compar_Reverse(const void* a, const void* b);

struct Line {
    char* str_index;
    unsigned int len_of_str;
};

struct Text
{
    char* buffer;
    Line* lines;
    ssize_t text_size;
    ssize_t num_of_strs;
};


#endif
