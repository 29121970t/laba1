#include <stdlib.h>

void insSortInt(int *array, int size);

void shellSortInt(int *array, int size);

void *memcpy(void *dest, void *src, size_t size);

void insSort(void *array, size_t length, size_t sizeOfElement, int (*condition)(void *, void *));
void shellSort(void *array, size_t length, size_t sizeOfElement, int (*condition)(void *, void *));

void shellSortSteped(void *array, size_t length, size_t sizeOfElement, size_t step, int (*condition)(void *, void *));
void insSortPassThrought(void *array, size_t length, size_t sizeOfElement, int (*condition)(void *, void *, void *), void* data);