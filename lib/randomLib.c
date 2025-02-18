#include <stdlib.h>
#include <stdio.h>


double map(double fromL, double fromH, double toL, double toH, double value) {
    return (value - fromH) * (toH - toL) / (fromH - fromL) + toH;
}
unsigned MrandomUInt(unsigned n) { return (1783627770u * (unsigned)n + 154) % 254803967; }
void fillArrWithRandomDouble(double* arr, size_t arrLength, unsigned seed, double from, double to) {
    unsigned k = seed;
    for (size_t i = 0; i < arrLength; i++) {
        k = MrandomUInt(k);
        arr[i] = map(0, 254803967, from, to, (double)k);
    }
}
void fillArrWithRandomLong(long* arr, size_t arrLength, unsigned seed, long from, long to) {
    unsigned k = seed;
    for (size_t i = 0; i < arrLength; i++) {
        k = MrandomUInt(k);
        arr[i] = (long)map(0, 254803967, from, to, (double)k);
    }
}
void fillSquareMatrixWithRandomDouble(double** arr, size_t matrixSize, unsigned seed, double from, double to) {
    unsigned k = seed;
    for (size_t i = 0; i < matrixSize; i++) {
        for (size_t j = 0; i < matrixSize; j++) {
            k = MrandomUInt(k);
            arr[i][j] = map(0, 254803967, from, to, (double)k);
        }
    }
}
void fillMatrixWithRandomDouble(double** arr, size_t col, size_t row, unsigned seed, double from, double to) {
    unsigned k = seed;
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            k = MrandomUInt(k);
            arr[i][j] = map(0, 254803967, from, to, (double)k);
        }
    }
}
void fillMatrixWithRandomULongInt(unsigned long ** arr, size_t col, size_t row, unsigned seed, double from, double to) {
    unsigned k = seed;
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            k = MrandomUInt(k);
            arr[i][j] = (unsigned long)map(0, 254803967, from, to, (double)k);
        }
    }
}


void fillArrWithRandomIncDouble(double* arr, size_t arrLength, unsigned seed, double from, double to) {
    unsigned k = seed;
    double lowBound = from;
    for (size_t i = 0; i < arrLength; i++) {
        k = MrandomUInt(k);
        arr[i] = map(0, 254803967, lowBound, to, (double)k);
        lowBound = arr[i];
    }
}