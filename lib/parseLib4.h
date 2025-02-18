#ifndef PARSE_LIB_
#define PARSE_LIB_

#include <errno.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "randomLib.h"

typedef enum StatusCode { SUCCES, ERMALLOC, EROUTOFRANGE, ERTOOLONG, ERINVALIDARG, ERINVALIDUERINPUT, ERVALIDATION } StatusCode;

void handleMallocError();

int isDigit(char c);
int isValidIntChar(char c);
int isSpace(char c);
int isValidDoubleChar(char c);
int isValidDoubleStringChar(char c);
int isValidIntString(char c);
int isValidString(char* str, int (*validator)(char));
int isValidString_l(char* str, size_t length, int (*validator)(char));

size_t getLine(char** string, size_t* size, FILE* stream);
size_t readLine(char** string, size_t* size);
size_t readLineWithDialog(char** array, const char* massege, size_t* length);
size_t readYNWithDialog(const char* str, int* res);
size_t readUDWithDialog(const char* str, int* res);
size_t readLineWithDialog_v(char** array, const char* message, size_t* length, int(*validate)(char* str, size_t len));

long parseLong(char* startPtr, char** endPtr);
size_t parseMultLong(long** array, char separator, char* inputString, size_t* count);
size_t readLong(long* ptr);
size_t readMultLong(long** array, char separator, size_t* count);
size_t readMultLongWithDialog(long** array, char separator, const char* massege, size_t* count);
size_t readLongWithDialog(long* ptr, char* messege);

size_t readLong_v(long* ptr, int (*validator)(long number));
size_t readLongWithDialog_v(long* ptr, char* messege, int (*validator)(long number));
size_t parseMultLong_v(long** array, char separator, char* inputString, size_t* count, int (*validator)(long number, size_t index));
size_t readMultLong_v(long** array, char separator, size_t* count, int (*validator)(long number, size_t index));
size_t readMultLongWithDialog_v(long** array, char separator, const char* massege, size_t* count,
                                int (*validator)(long number, size_t index));

double parseDouble(char* startPtr, char** endPtr);
size_t parseMultDouble(double** array, char separator, char* inputString, size_t* count);
size_t readMultDouble(double** array, char separator, size_t* count);
size_t readMultDoubleWithDialog(double** array, char separator, const char* massege, size_t* count);
size_t readDoubleWithDialog(double* ptr, char* messege);
size_t readDoubleWithDialog_v(double* ptr, char* messege, int (*validator)(double number));
size_t readMultDoubleWithDialog_v(double** array, char separator, const char* message, size_t* count,
                                  int (*validator)(double number, size_t index));



int isG0(long num);
size_t readMatrix(double*** mat, size_t* rowCount, size_t* colCount);
size_t randomMatrix(double*** mat, long* rCount, long* cCount, double from, double to);

#endif