#include <stdlib.h>

#include "shellSort.h"
void insSortInt(int *array, int size) {
    for (int i = 1; i < size; i++) {
        int tmp = array[i];
        int j = i - 1;
        for (; j >= 0; j--) {
            if (tmp > array[j]) break;
            array[j + 1] = array[j];
        }
        array[j + 1] = tmp;
    }
}

void shellSortInt(int *array, int size) {
    // Последовательность Циура
    int steps[] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};
    for (int p = 8; p >= 0; p--) {
        for (int i = 0; i < size; i += steps[p]) {
            int tmp = array[i];
            int j = i - steps[p];
            for (; j >= 0 && tmp < array[j]; j -= steps[p]) array[j + steps[p]] = array[j];
            array[j + steps[p]] = tmp;
        }
    }
}

void *memcpy(void *dest, void *src, size_t size) {
    char *d = (char *)dest;
    char *s = (char *)src;

    while (size--) {
        *d++ = *s++;
    }
    return dest;
}

void insSort(void *array, size_t length, size_t sizeOfElement, int (*condition)(void *, void *)) {
    void *tmpP = malloc(sizeOfElement);
    for (size_t i = sizeOfElement; i < length * sizeOfElement; i += sizeOfElement) {
        memcpy(tmpP, array + i, sizeOfElement);
        size_t j = i;
        for (; j > 0 && condition(tmpP, array + j - sizeOfElement) < 0; j -= sizeOfElement) {
            memcpy(array + j, array + j - sizeOfElement, sizeOfElement);
        }
        memcpy(array + j, tmpP, sizeOfElement);
    }
    free(tmpP);
}

void insSortPassThrought(void *array, size_t length, size_t sizeOfElement, int (*condition)(void *, void *, void *), void* data) {
    void *tmpP = malloc(sizeOfElement);
    for (size_t i = sizeOfElement; i < length * sizeOfElement; i += sizeOfElement) {
        memcpy(tmpP, array + i, sizeOfElement);
        size_t j = i;
        for (; j > 0 && condition(tmpP, array + j - sizeOfElement, data) < 0; j -= sizeOfElement) {
            memcpy(array + j, array + j - sizeOfElement, sizeOfElement);
        }
        memcpy(array + j, tmpP, sizeOfElement);
    }
    free(tmpP);
}

void shellSort(void *array, size_t length, size_t sizeOfElement, int (*condition)(void *, void *)) {
    // Последовательность Циура
    int steps[] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};
    void *tmpP = malloc(sizeOfElement);
    size_t byteLength = length * sizeOfElement;
    for (int k = 8; k >= 0; k--) {
        size_t elementStep = steps[k] * sizeOfElement;
        for (size_t i = elementStep; i < byteLength; i += elementStep) {
            memcpy(tmpP, array + i, sizeOfElement);
            size_t j = i;
            for (; j <= byteLength && j > 0 && condition(tmpP, array + j - elementStep) < 0; j -= elementStep) {
                memcpy(array + j, array + j - elementStep, sizeOfElement);
            }
            memcpy(array + j, tmpP, sizeOfElement);
        }
    }
    free(tmpP);
}

void shellSortSteped(void *array, size_t length, size_t sizeOfElement, size_t step, int (*condition)(void *, void *)) {
    // Последовательность Циура
    int deltas[] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};
    void *tmpP = malloc(sizeOfElement);
    size_t byteLength = length * sizeOfElement;
    for (int k = 8; k >= 0; k--) {
        size_t elementStep = deltas[k] * sizeOfElement * step;
        for (size_t i = elementStep; i < byteLength; i += elementStep) {
            memcpy(tmpP, array + i, sizeOfElement);
            size_t j = i;
            for (; j <= byteLength && j > 0 && condition(tmpP, array + j - elementStep) < 0; j -= elementStep) {
                memcpy(array + j, array + j - elementStep, sizeOfElement);
            }
            memcpy(array + j, tmpP, sizeOfElement);
        }
    }
    free(tmpP);
}