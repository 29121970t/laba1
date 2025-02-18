#include "parseLib4.h"

void handleMallocError() {
    puts("MALLOC ERROR. No enougth memory on device.");
    exit(1);
}

int isDigit(char c) { return c >= '0' && c <= '9' ? 1 : 0; }
int isValidIntChar(char c) { return isDigit(c) || c == '-'; }
int isSpace(char c) { return c == ' ' || c == '\t' || c == '\n'; }
int isValidDoubleChar(char c) { return isValidIntChar(c) || c == '.'; }
int isValidDoubleStringChar(char c) { return isValidDoubleChar(c) || isSpace(c); }
int isValidIntString(char c) { return isValidIntChar(c) || isSpace(c); }

char* getStatusDescription(size_t code) {
    static char statusDescriptions[8][44] = {"No error occured",
                                             "Could not allocete or enought memory",
                                             "Number is too large or too small",
                                             "The input is too long",
                                             "Function got invalid argument",
                                             "Invalid user input",
                                             "Validation filed. Reread input requirements",
                                             "No such error code"};
    if (code > 7) return statusDescriptions[8];
    return (char*)(&(statusDescriptions[code]));
}
size_t errnoToStatusCode(int errnoCode) {
    switch (errnoCode) {
        case EINVAL:
            return ERINVALIDUERINPUT;
            break;
        case ERANGE:
            return EROUTOFRANGE;
            break;
        default:
            return 0;
            break;
    }
}
int isValidString(char* str, int (*validator)(char)) {
    for (size_t i = 0; i < ULONG_MAX; i++) {
        if (str[i] == '\0') return 1;
        if (!validator(str[i])) return 0;
    }
    return -1;
}

int isValidString_l(char* str, size_t length, int (*validator)(char)) {
    for (size_t i = 0; i < length; i++) {
        if (!validator(str[i])) return 0;
    }
    return 1;
}

int isValidString_c(char* str, int (*validator)(char), char c) {
    for (size_t i = 0; i < ULONG_MAX; i++) {
        if (str[i] == '\0') return 1;
        if (!validator(str[i]) && str[i] != c) return 0;
    }
    return -1;
}

void* realloc_f(void* __ptr, size_t __size) {
    char* tmpPtr = NULL;
    if ((tmpPtr = realloc(__ptr, __size)) == NULL) free(__ptr);
    return tmpPtr;
}

size_t getLine(char** string, size_t* size, FILE* stream) {
    size_t currentSize = 8;
    size_t counter;
    char* result = NULL;
    char* tmpPtr = NULL;

    if ((result = calloc(currentSize, sizeof(char))) == NULL) return ERMALLOC;

    for (counter = 0; counter < ULONG_MAX; counter++) {
        if (currentSize - counter == 0) {
            currentSize *= 2;
            if ((result = realloc_f(result, currentSize * sizeof(char))) == NULL) return ERMALLOC;
        }

        result[counter] = getc(stream);
        if (result[counter] == '\n') break;
    }
    if (counter == ULONG_MAX) {
        free(result);
        return ERTOOLONG;
    }

    currentSize = counter + 1;
    if ((result = realloc_f(result, currentSize * sizeof(char))) == NULL) return ERMALLOC;

    result[counter] = '\0';
    *string = result;

    if (size != NULL) *size = currentSize;
    return 0;
}

long parseLong(char* startPtr, char** endPtr) {
    errno = 0;
    long result = 0;
    unsigned numberStartIndex = 0;
    int negFlag = 0;
    unsigned cursor = 0;

    for (; numberStartIndex < UINT_MAX; numberStartIndex++) {
        if (!isSpace(startPtr[numberStartIndex])) break;
    }

    if (startPtr[numberStartIndex] == '-') {
        negFlag = 1;
        numberStartIndex++;
    }

    for (cursor = numberStartIndex; cursor < UINT_MAX; cursor++) {
        if (!isDigit(startPtr[cursor])) {
            if (cursor == numberStartIndex) errno = EINVAL;
            if (endPtr != NULL) *endPtr = startPtr + cursor;
            return negFlag ? -result : result;
        }

        long testMult = result * 10;
        int digit = startPtr[cursor] - '0';

        if (testMult / 10 != result || LONG_MAX - testMult < digit) {
            errno = ERANGE;
            if (endPtr != NULL) *endPtr = startPtr + cursor + 1;
            return negFlag ? LONG_MIN : LONG_MAX;
        }
        result = testMult + digit;
    }

    if (cursor == UINT_MAX) errno = ERANGE;

    return 0;
}

size_t readLong(long* ptr) {
    char* str = NULL;
    size_t errorTmp = 0;
    if (errorTmp = getLine(&str, NULL, stdin)) return errorTmp;
    if (!isValidString(str, isValidIntString)) {
        free(str);
        return ERINVALIDUERINPUT;
    }

    *ptr = parseLong(str, NULL);
    free(str);

    if (errno) return errnoToStatusCode(errno);

    return 0;
}

size_t readLong_v(long* ptr, int (*validator)(long number)) {
    char* str = NULL;
    size_t errorTmp = 0;
    if (errorTmp = getLine(&str, NULL, stdin)) return errorTmp;
    if (!isValidString(str, isValidIntString)) {
        free(str);
        return ERINVALIDUERINPUT;
    }

    *ptr = parseLong(str, NULL);
    free(str);

    if (errno) return errnoToStatusCode(errno);
    if (validator && !validator(*ptr)) return ERVALIDATION;

    return 0;
}
size_t _parseMultLongExitHanler(size_t counter, long* result, size_t* count, long** array, size_t toRet) {
    if (counter == 0) {
        free(result);
        result = NULL;
    }

    if (result && (result = realloc_f(result, counter * sizeof(long))) == NULL) return ERMALLOC;
    *count = counter;
    *array = result;
    return toRet;
}

size_t parseMultLong(long** array, char separator, char* inputString, size_t* count) {
    char* tmpPtr1 = inputString;
    char* tmpPtr2 = NULL;

    size_t bufferSize = 8;
    size_t counter = 0;
    size_t errorTmp = 0;
    long* result = NULL;

    *count = 0;

    if ((result = calloc(bufferSize, sizeof(long))) == NULL) return ERMALLOC;

    for (counter = 0; counter < ULONG_MAX; counter++) {
        if (bufferSize == counter) {
            bufferSize *= 2;
            if ((result = realloc_f(result, bufferSize * sizeof(long))) == NULL) return ERMALLOC;
        }

        errno = 0;
        result[counter] = parseLong(tmpPtr1, &tmpPtr2);

        unsigned i = 0;
        for (; i < UINT_MAX; i++) {
            if (!isSpace(tmpPtr2[i])) break;
        }

        if (errno) return _parseMultLongExitHanler(counter, result, count, array, errnoToStatusCode(errno));
        if (tmpPtr2[i] != separator && tmpPtr2[i] != '\0')
            return _parseMultLongExitHanler(counter, result, count, array, ERINVALIDUERINPUT);

        if (tmpPtr2[i] == '\0') return _parseMultLongExitHanler(++counter, result, count, array, 0);

        tmpPtr1 = tmpPtr2 + 1 + i;
    }
    free(result);
    return ERTOOLONG;
}

size_t parseMultLong_v(long** array, char separator, char* inputString, size_t* count, int (*validator)(long number, size_t index)) {
    char* tmpPtr1 = inputString;
    char* tmpPtr2 = NULL;

    size_t bufferSize = 8;
    size_t counter = 0;
    size_t errorTmp = 0;
    long* result = NULL;

    *count = 0;

    if ((result = calloc(bufferSize, sizeof(long))) == NULL) return ERMALLOC;

    for (counter = 0; counter < ULONG_MAX; counter++) {
        if (bufferSize == counter) {
            bufferSize *= 2;
            if ((result = realloc_f(result, bufferSize * sizeof(long))) == NULL) return ERMALLOC;
        }

        errno = 0;
        result[counter] = parseLong(tmpPtr1, &tmpPtr2);

        unsigned i = 0;
        for (; i < UINT_MAX; i++) {
            if (!isSpace(tmpPtr2[i])) break;
        }
        if (errno) return _parseMultLongExitHanler(counter, result, count, array, errnoToStatusCode(errno));
        if (tmpPtr2[i] != separator && tmpPtr2[i] != '\0')
            return _parseMultLongExitHanler(counter, result, count, array, ERINVALIDUERINPUT);
        if (validator && !validator(result[counter], counter)) return _parseMultLongExitHanler(counter, result, count, array, ERVALIDATION);

        if (tmpPtr2[i] == '\0') return _parseMultLongExitHanler(++counter, result, count, array, 0);

        tmpPtr1 = tmpPtr2 + 1 + i;
    }

    return ERTOOLONG;
}

size_t readMultLong(long** array, char separator, size_t* count) {
    char* str = NULL;
    size_t tmpError = 0;
    if (tmpError = getLine(&str, NULL, stdin)) return tmpError;
    if (tmpError = parseMultLong(array, separator, str, count)) {
        free(str);
        return tmpError;
    }
    free(str);
    return 0;
}

size_t readMultLong_v(long** array, char separator, size_t* count, int (*validator)(long number, size_t index)) {
    char* str = NULL;
    size_t tmpError = 0;
    if (tmpError = getLine(&str, NULL, stdin)) return tmpError;
    if (tmpError = parseMultLong_v(array, separator, str, count, validator)) {
        free(str);
        return tmpError;
    }
    free(str);
    return 0;
}

size_t readMultLongWithDialog(long** array, char separator, const char* message, size_t* count) {
    size_t tmpError = 0;
    do {
        printf("%s", message);
        if (tmpError = readMultLong(array, separator, count)) {
            if (tmpError == ERMALLOC) return ERMALLOC;
            printf("Error occured while parsing %lu-th number: %s\n", *count + 1, getStatusDescription(tmpError));
            free(*array);
        }

    } while (tmpError);
    return 0;
}

size_t readMultLongWithDialog_v(long** array, char separator, const char* message, size_t* count,
                                int (*validator)(long number, size_t index)) {
    size_t tmpError = 0;
    do {
        printf("%s", message);
        if (tmpError = readMultLong_v(array, separator, count, validator)) {
            if (tmpError == ERMALLOC) return ERMALLOC;
            printf("Error occured while parsing %lu-th number: %s\n", *count + 1, getStatusDescription(tmpError));
            free(*array);
        }

    } while (tmpError);
    return 0;
}

size_t readLongWithDialog(long* ptr, char* message) {
    size_t tmpError = 0;
    do {
        printf("%s", message);
        if (tmpError = readLong(ptr)) {
            if (tmpError == ERMALLOC) return ERMALLOC;
            printf("Error occured while parsing number: %s\n", getStatusDescription(tmpError));
        }

    } while (tmpError);
    return 0;
}

size_t readLongWithDialog_v(long* ptr, char* message, int (*validator)(long number)) {
    size_t tmpError = 0;
    do {
        printf("%s", message);

        if (tmpError = readLong_v(ptr, validator)) {
            if (tmpError == ERMALLOC) return ERMALLOC;
            printf("Error occured while parsing number: %s\n", getStatusDescription(tmpError));
        }

    } while (tmpError);
    return 0;
}

double parseDouble(char* startPtr, char** endPtr) {
    errno = 0;
    double result = 0;
    double shift = .1;
    unsigned cursor = 0;
    int negFlag = 0;
    unsigned digitCounter = 0;
    long dotPos = -1;

    for (; cursor < UINT_MAX; cursor++) {
        if (!isSpace(startPtr[cursor])) break;
    }

    if (startPtr[cursor] == '-') {
        negFlag = 1;
        cursor++;
    }

    for (digitCounter = cursor; digitCounter < UINT_MAX; digitCounter++) {
        if (!isDigit(startPtr[digitCounter])) {
            if (digitCounter == cursor) errno = EINVAL;
            if (dotPos == -1 && startPtr[digitCounter] == '.') {
                dotPos = digitCounter;
                continue;
            }
            if (endPtr != NULL) *endPtr = startPtr + digitCounter;
            break;
        }

        double testMult = result * 10;
        double digit = startPtr[digitCounter] - '0';

        result = testMult + digit;
    }
    if (digitCounter == INT_MAX) {
        errno = ERANGE;
        return 0;
    }

    if (dotPos != -1) {
        for (long i = dotPos + 1; i < digitCounter; i++) {
            result /= 10;
        }
    }

    return negFlag ? -result : result;
}
// refactor step
size_t readDouble(double* ptr) {
    char* str = NULL;
    size_t errorTmp = 0;
    if (errorTmp = getLine(&str, NULL, stdin)) return errorTmp;
    if (!isValidString(str, isValidDoubleStringChar)) {
        free(str);
        return ERINVALIDUERINPUT;
    }

    *ptr = parseDouble(str, NULL);
    free(str);

    if (errno) return errnoToStatusCode(errno);

    return 0;
}

size_t readDouble_v(double* ptr, int (*validator)(double number)) {
    char* str = NULL;
    size_t errorTmp = 0;
    if (errorTmp = getLine(&str, NULL, stdin)) return errorTmp;
    if (!isValidString(str, isValidDoubleStringChar)) {
        free(str);
        return ERINVALIDUERINPUT;
    }

    *ptr = parseDouble(str, NULL);
    free(str);
    if (errno) return errnoToStatusCode(errno);
    if (validator && !validator(*ptr)) return ERVALIDATION;

    return 0;
}

size_t _parseMultDoubleExitHanler(size_t counter, double* result, size_t* count, double** array, size_t toRet) {
    if (counter == 0) {
        free(result);
        result = NULL;
    }

    if (result && (result = realloc_f(result, counter * sizeof(double))) == NULL) return ERMALLOC;
    *count = counter;
    *array = result;
    return toRet;
}

size_t parseMultDouble(double** array, char separator, char* inputString, size_t* count) {
    if (separator == '.') return ERINVALIDARG;

    char* tmpPtr1 = inputString;
    char* tmpPtr2 = NULL;

    size_t bufferSize = 8;
    size_t counter;
    size_t errorTmp = 0;

    double* result = NULL;

    if ((result = calloc(bufferSize, sizeof(double))) == NULL) return ERMALLOC;
    for (counter = 0; counter < ULONG_MAX; counter++) {
        if (bufferSize == counter) {
            if ((result = realloc_f(result, bufferSize * 2 * sizeof(double))) == NULL) return ERMALLOC;
            bufferSize *= 2;
        }
        errno = 0;
        result[counter] = parseDouble(tmpPtr1, &tmpPtr2);

        unsigned i = 0;
        for (; i < UINT_MAX; i++) {
            if (!isSpace(tmpPtr2[i])) break;
        }

        if (errno) return _parseMultDoubleExitHanler(counter, result, count, array, errnoToStatusCode(errno));
        if (tmpPtr2[i] != separator && tmpPtr2[i] != '\0')
            return _parseMultDoubleExitHanler(counter, result, count, array, ERINVALIDUERINPUT);

        if (tmpPtr2[i] == '\0') return _parseMultDoubleExitHanler(++counter, result, count, array, 0);

        tmpPtr1 = tmpPtr2 + 1 + i;
    }

    free(result);
    return ERTOOLONG;
}

size_t parseMultDouble_v(double** array, char separator, char* inputString, size_t* count, int (*validator)(double number, size_t index)) {
    if (separator == '.') return ERINVALIDARG;

    char* tmpPtr1 = inputString;
    char* tmpPtr2 = NULL;

    size_t bufferSize = 8;
    size_t counter;
    size_t errorTmp = 0;

    double* result = NULL;

    if ((result = calloc(bufferSize, sizeof(double))) == NULL) return ERMALLOC;
    for (counter = 0; counter < ULONG_MAX; counter++) {
        if (bufferSize == counter) {
            if ((result = realloc_f(result, bufferSize * 2 * sizeof(double))) == NULL) return ERMALLOC;
            bufferSize *= 2;
        }
        errno = 0;
        result[counter] = parseDouble(tmpPtr1, &tmpPtr2);

        unsigned i = 0;
        for (; i < UINT_MAX; i++) {
            if (!isSpace(tmpPtr2[i])) break;
        }

        if (errno) return _parseMultDoubleExitHanler(counter, result, count, array, errnoToStatusCode(errno));
        if (tmpPtr2[i] != separator && tmpPtr2[i] != '\0')
            return _parseMultDoubleExitHanler(counter, result, count, array, ERINVALIDUERINPUT);
        if (validator && !validator(result[counter], counter))
            return _parseMultDoubleExitHanler(counter, result, count, array, ERVALIDATION);
        if (tmpPtr2[i] == '\0') return _parseMultDoubleExitHanler(++counter, result, count, array, 0);

        tmpPtr1 = tmpPtr2 + 1 + i;
    }

    free(result);
    return ERTOOLONG;
}

size_t readMultDouble(double** array, char separator, size_t* count) {
    char* str = NULL;
    size_t tmpError = 0;
    if (tmpError = getLine(&str, NULL, stdin)) return tmpError;
    if (tmpError = parseMultDouble(array, separator, str, count)) {
        free(str);
        return tmpError;
    }
    free(str);
    return 0;
}

size_t readMultDouble_v(double** array, char separator, size_t* count, int (*validator)(double number, size_t index)) {
    char* str = NULL;
    size_t tmpError = 0;
    if (tmpError = getLine(&str, NULL, stdin)) return tmpError;
    if (tmpError = parseMultDouble_v(array, separator, str, count, validator)) {
        free(str);
        return tmpError;
    }
    free(str);
    return 0;
}

size_t readMultDoubleWithDialog(double** array, char separator, const char* message, size_t* count) {
    size_t tmpError = 0;
    do {
        printf("%s", message);
        if (tmpError = readMultDouble(array, separator, count)) {
            if (tmpError == ERMALLOC) return ERMALLOC;
            printf("Error occured while parsing %lu-th number: %s\n", *count + 1, getStatusDescription(tmpError));
            free(*array);
        }

    } while (tmpError);
    return 0;
}

size_t readMultDoubleWithDialog_v(double** array, char separator, const char* message, size_t* count,
                                  int (*validator)(double number, size_t index)) {
    size_t tmpError = 0;
    do {
        printf("%s", message);
        if (tmpError = readMultDouble_v(array, separator, count, validator)) {
            if (tmpError == ERMALLOC) return ERMALLOC;
            printf("Error occured while parsing %lu-th number: %s\n", *count + 1, getStatusDescription(tmpError));
            free(*array);
        }

    } while (tmpError);
    return 0;
}

size_t readDoubleWithDialog(double* ptr, char* message) {
    size_t tmpError = 0;
    do {
        printf("%s", message);
        if (tmpError = readDouble(ptr)) {
            if (tmpError == ERMALLOC) return ERMALLOC;
            printf("Error occured while parsing number: %s\n", getStatusDescription(tmpError));
        }

    } while (tmpError);
    return 0;
}

size_t readDoubleWithDialog_v(double* ptr, char* message, int (*validator)(double number)) {
    size_t tmpError = 0;
    do {
        printf("%s", message);
        if (tmpError = readDouble_v(ptr, validator)) {
            if (tmpError == ERMALLOC) return ERMALLOC;
            printf("Error occured while parsing number: %s\n", getStatusDescription(tmpError));
        }

    } while (tmpError);
    return 0;
}

size_t readLine(char** string, size_t* size) {
    size_t errorTmp = 0;
    if (errorTmp = getLine(string, size, stdin)) return errorTmp;
    return 0;
}
size_t readLine_v(char** string, size_t* size, int(*validate)(char* str, size_t len)) {
    size_t len = 0;
    size_t errorTmp = 0;
    if (errorTmp = getLine(string, &len, stdin)) return errorTmp;
    if (!validate(*string, len)) return ERVALIDATION;
    if(size) *size = len;

    return 0;
}
size_t readLineWithDialog(char** array, const char* message, size_t* length) {
    size_t tmpError = 0;
    do {
        printf("%s", message);
        if (tmpError = readLine(array, length)) {
            if (tmpError == ERMALLOC) return ERMALLOC;
            printf("Error occured while parsing input: %s\n", getStatusDescription(tmpError));
        }

    } while (tmpError);
    return 0;
}
size_t readLineWithDialog_v(char** array, const char* message, size_t* length, int(*validate)(char* str, size_t len)) {
    size_t tmpError = 0;
    do {
        printf("%s", message);
        if (tmpError = readLine_v(array, length, validate)) {
            if (tmpError == ERMALLOC) return ERMALLOC;
            printf("Error occured while parsing input: %s\n", getStatusDescription(tmpError));
        }

    } while (tmpError);
    return 0;
}


size_t readYNWithDialog(const char* message, int* res) {
    size_t errorTmp = 0;
    char* answ1 = NULL;
    int flag = 0;
    do {
        printf("%s", message);
        if (errorTmp = readLine(&answ1, NULL)) return errorTmp;

        if (flag = (answ1[0] != 'Y' && answ1[0] != 'N' && answ1[0] != 'y' && answ1[0] != 'n' || answ1[1] != '\0'))
            puts("ERROR: Cannont interpret this answer. Rered input description");
        *res = answ1[0] == 'Y' || answ1[0] == 'y';
        free(answ1);

    } while (flag);

    return 0;
}

size_t readUDWithDialog(const char* message, int* res) {
    size_t errorTmp = 0;
    char* answ1 = NULL;
    int flag = 0;
    do {
        printf("%s", message);
        if (errorTmp = readLine(&answ1, NULL)) return errorTmp;

        if (flag = (answ1[0] != 'U' && answ1[0] != 'D' && answ1[0] != 'u' && answ1[0] != 'd' || answ1[1] != '\0'))
            puts("ERROR: Cannont interpret this answer. Rered input description");
        *res = answ1[0] == 'u' || answ1[0] == 'U';
        free(answ1);

    } while (flag);

    return 0;
}



int isG0(long num) { return num > 0; }

size_t readMatrix(double*** mat, size_t* rowCount, size_t* colCount) {
    double** matrix = NULL;
    size_t rc = 0;
    size_t cc = 0;
    char tmpStr[100] = {0};
    size_t tmpErr = 0;

    if (tmpErr = readLongWithDialog_v(&rc, "Please enter row count. (r > 0)\n", isG0)) return tmpErr;
    if ((matrix = calloc(rc, sizeof(double*))) == NULL) return ERMALLOC;
    if (tmpErr = readMultDoubleWithDialog(matrix, ',', "Please enter first row of a matrix (Real numbers separated by commas)\n", &cc))
        return tmpErr;

    for (long i = 1; i < rc; i++) {
        int flag = 0;
        size_t tmpColCount = 0;
        do {
            sprintf(tmpStr, "Please enter %ld-th row of a matrix: %ld numbers separated by commas.\n", i + 1, cc);
            if (tmpErr = readMultDoubleWithDialog(&matrix[i], ',', tmpStr, &tmpColCount)) return tmpErr;

            if (flag = tmpColCount != cc) {
                puts("error: Row length mismatch");
                free(matrix[i]);
            }

        } while (flag);
    }
    *rowCount = rc;
    *colCount = cc;
    *mat = matrix;
    return 0;
}

size_t randomMatrix(double*** mat, long* rCount, long* cCount, double from, double to) {
    double** matrix = NULL;
    size_t tmpErr = 0;
    long seed = 0;

    if (tmpErr = readLongWithDialog_v(rCount, "Please enter row count. (r > 0)\n", isG0)) return tmpErr;
    if (tmpErr = readLongWithDialog_v(cCount, "Please enter clumn count. (r > 0)\n", isG0)) return tmpErr;

    if ((matrix = calloc(*rCount, sizeof(double*))) == NULL) return ERMALLOC;

    if (tmpErr = readLongWithDialog_v(&seed, "Please enter seed for random number generation (Natural number)\n", isG0)) return tmpErr;

    unsigned rand = MrandomUInt(seed);
    for (long i = 0; i < *rCount; i++) {
        if ((matrix[i] = calloc(*cCount, sizeof(long))) == NULL) return ERMALLOC;

        for (int j = 0; j < *cCount; j++) {
            rand = MrandomUInt(rand);
            matrix[i][j] = map(0, 254803967, from, to, rand);
        }
    }
    *mat = matrix;
    return 0;
}
