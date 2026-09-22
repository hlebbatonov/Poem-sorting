#include "comparators.h"

int Compar(const void* a, const void* b)
{
    size_t i = 0;
    size_t j = 0;

    const char* lhs = *(const char* const*) a;
    const char* rhs = *(const char* const*) b;

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
    int i = 0;
    int j = 0;

    const char* lhs = *(const char* const*) a;
    const char* rhs = *(const char* const*) b;
    while (lhs[i] != '\0') i++; //TODO: UBRAT CRINZH
    while (rhs[j] != '\0') j++;
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
