#include "string_functions.h"


size_t strlen(const char* str)
{
    size_t i = 0;
    while (str[i] != '\0') i++;
    return i;

}

int puts(const char* str)
{
    int i = 0;
    while (str[i] != '\0') {
        if (putchar(str[i]) == EOF) return EOF;
        i++;
    }
    if (putchar('\n') == EOF) return EOF;
    return str[--i];
}

char* strcpy(char* dest, const char* src)
{
    char* first_sym_of_new = dest;
    size_t i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';

    return first_sym_of_new;
}

char* strncpy(char* dest, const char* src, size_t num_of_syms)
{
    char* first_sym_of_new = dest;
    bool was_zero_sym = false;

    for (size_t i = 0; i < num_of_syms; i++)
    {
        if (src[i] == '\0')
            was_zero_sym = true;
        if (!was_zero_sym)
            dest[i] = src[i];
        else
            dest[i] = '\0';
    }

    return first_sym_of_new;
}

char* strcat(char* dest, const char* src)
{
    char* first_sym_of_new = dest;

    while (*dest != '\0')
        dest++;
    size_t i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return first_sym_of_new;
}

char* strncat(char* dest, const char* src, size_t num_of_syms)
{
    char* first_sym_of_new = dest;

    while (*dest != '\0')
        dest++;
    size_t i = 0;
    while (src[i] != '\0' && num_of_syms > 0)
    {
        dest[i] = src[i];
        i++;
        num_of_syms--;
    }
    dest[i] = '\0';
    return first_sym_of_new;
}

int strcmp(const char* lhs, const char* rhs)
{
    int res = 0;
    size_t i = 0;

    while (res == 0 && lhs[i] != '\0' && rhs[i] != '\0')
    {
        if (lhs[i] < rhs[i]) res = -1;
        else if (lhs[i] > rhs[i]) res = 1;
        else res = 0;
        i++;
    }

    if (lhs[i] < rhs[i]) res = -1;
    else if (lhs[i] > rhs[i]) res = 1;
    else res = 0;

    return lhs[i] - rhs[i];
}

int strncmp(const char* lhs, const char* rhs, size_t num_of_syms)
{
    int res = 0;
    size_t i = 0;

    while (res == 0 && lhs[i] != '\0' && rhs[i] != '\0' && num_of_syms > 0)
    {
        if (lhs[i] < rhs[i]) res = -1;
        else if (lhs[i] > rhs[i]) res = 1;
        else res = 0;
        i++;
        num_of_syms--;
    }
    if (res == 0 && num_of_syms > 0)
    {
        if (lhs[i] < rhs[i]) res = -1;
        else if (lhs[i] > rhs[i]) res = 1;
        else res = 0;
    }
    return res;
}

char* strchr(const char* s, int c)
{
    char* res = NULL;
    size_t i = 0;

    while (res == NULL && s[i] != '\0')
    {
        if (s[i] == c) res = (char*)&s[i];
        i++;
    }

    return res;
}

char* strrchr(const char* s, int c)
{
    char* res = NULL;
    size_t i = 0;
    while (s[i] != '\0') i++;
    i++;

    while (res == NULL && i-- > 0)
        if (s[i] == c) res = (char*)&s[i];

    return res;
}

ssize_t getline(char** lineptr, size_t* n, FILE* stream)
{
    ssize_t curr_sym = 0;
    size_t num_of_syms = 0;

    if (*n == 0)
    {
        *n = 128;
        *lineptr = (char*)malloc(*n);
    }

    size_t i = 0;
    ssize_t res = 0;

    while (res == 0)
    {
        if ((curr_sym = getc(stream)) != EOF)
        {
            if (num_of_syms == *n - 1)
            {
                *n *= 2;

                char* temp_lineptr = (char*)realloc(*lineptr, *n);

                if (temp_lineptr) *lineptr = temp_lineptr;
                else return -1;
            }

            (*lineptr)[i] = curr_sym;

            if (curr_sym == '\n') res = i + 1;

            num_of_syms++;
            i++;
        }
        else res = i;
    }
    if (num_of_syms == *n - 1)
        {
            *n *= 2;

            char* temp_lineptr = (char*)realloc(*lineptr, *n);

            if (temp_lineptr) *lineptr = temp_lineptr;
            else return -1;
        }
    (*lineptr)[i] = '\0';
    return res;
}

char* strstr(const char* string, const char* substring)
{
    size_t i = 0;

    while (string[i] != '\0')
    {
        if (string[i] == substring[0])
        {
            size_t j = 0;
            bool curr_strs_are_equal = true;
            while(substring[j] != '\0')
            {
                if (string[i + j] == '\0' || string[i + j] != substring[j])
                    curr_strs_are_equal = false;
                j++;
            }
            if (curr_strs_are_equal)
                return (char*)&string[i];
        }
        i++;
    }
    return NULL;
}

void* memcpy(void* dest, const void* src, size_t n)
{
    char* curr_dest = (char*)dest;
    char* curr_src = (char*)src;

    for (size_t i = 0; i < n; i++) curr_dest[i] = curr_src[i];

    return dest;
}

void* memmove(void* dest, const void* src, size_t n)
{
    char* curr_dest = (char*)dest;
    char* curr_src = (char*)src;

    if (curr_src < curr_dest && curr_dest < curr_src + n)
        for (size_t i = n; i > 0; i--) curr_dest[i - 1] = curr_src[i - 1];
    else
        for (size_t i = 0; i < n; i++) curr_dest[i] = curr_src[i];
    return dest;
}
