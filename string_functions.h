#ifndef STRING_FUNCTIONS_H
#define STRING_FUNCTIONS_H

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

size_t  strlen  (const char* str);
int     puts    (const char* str);
char*   strcpy  (char* dest, const char* src);
char*   strncpy (char* dest, const char* src, size_t num_of_syms);
char*   strcat  (char* dest, const char* src);
char*   strncat (char* dest, const char* src, size_t num_of_syms);
int     strcmp  (const char* lhs, const char* rhs);
int     strncmp (const char* lhs, const char* rh, size_t num_of_syms);
char*   strchr  (const char* s, int c);
char*   strrchr (const char* s, int c);
ssize_t getline (char** lineptr, size_t* n, FILE* stream);
void*   memcpy  (void* dest, const void* src, size_t n);

#endif
