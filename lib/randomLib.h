#include <stdlib.h>

unsigned MrandomUInt(unsigned n);
void fillArrWithRandomDouble(double* arr, size_t arrLength, unsigned seed, double from, double to);
double map(double fromL, double fromH, double toL, double toH, double value);
void fillArrWithRandomIncDouble(double* arr, size_t arrLength, unsigned seed, double from, double to);
void fillSquareMatrixWithRandomDouble(double** arr, size_t matrixSize, unsigned seed, double from, double to);
void fillArrWithRandomLong(long* arr, size_t arrLength, unsigned seed, long from, long to);
void fillMatrixWithRandomDouble(double** arr, size_t col, size_t row, unsigned seed, double from, double to) ;
void fillMatrixWithRandomULongInt(unsigned long ** arr, size_t col, size_t row, unsigned seed, double from, double to);
