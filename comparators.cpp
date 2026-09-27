#include "comparators.h"

int Compar(const void* a, const void* b)
{
    size_t i = 0;
    size_t j = 0;

    const Line* line_l = (const Line*) a;
    const Line* line_r = (const Line*) b;
    const char* lhs = line_l->str_index;
    const char* rhs = line_r->str_index;

    while (lhs[i] != '\0' && rhs[j] != '\0')
    {
        if (isalpha(lhs[i]) == 0)
        {
            i++;
            continue;
        }
        if (isalpha(rhs[j]) == 0)
        {
            j++;
            continue;
        }
        if (tolower(lhs[i]) != tolower(rhs[j])) return tolower(lhs[i]) - tolower(rhs[j]);
        i++;
        j++;
    }
    while(lhs[i] != '\0' && !isalpha(lhs[i])) i++;
    while(rhs[j] != '\0' && !isalpha(rhs[j])) j++;
    return lhs[i] - rhs[j];
}

int Compar_Reverse(const void* a, const void* b)
{


    const Line* line_l = (const Line*) a;
    const Line* line_r = (const Line*) b;
    const char* lhs = line_l->str_index;
    const char* rhs = line_r->str_index;

    int i = (int)line_l->len_of_str - 1;
    int j = (int)line_r->len_of_str - 1;

    while (i >= 0 && j >= 0)
    {
        if (isalpha(lhs[i]) == 0)
        {
            i--;
            continue;
        }
        if (isalpha(rhs[j]) == 0)
        {
            j--;
            continue;
        }
        if (tolower(lhs[i]) != tolower(rhs[j])) return tolower(lhs[i]) - tolower(rhs[j]);

        i--;
        j--;
    }
    while(i >= 0 && !isalpha(lhs[i])) i--;
    while(j >= 0 && !isalpha(rhs[j])) j--;
    if (i < 0 && j < 0) return 0;
    else return (i < 0 ? -1: 1);
}
