#ifndef BUBBLESORT_H
#define BUBBLESORT_H

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void BubbleSorting(void* data, size_t nmemb, size_t size, int(*compar)(const void*, const void*));
void ReplaceValues(void* x, void* y, size_t size);

#endif
