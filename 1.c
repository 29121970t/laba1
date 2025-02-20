#include <limits.h>
#include <stdio.h>

#include "lib/LabMenu.h"
#include "lib/parseLib4.h"

#ifdef __linux__
#include <unistd.h>
int setFileSize(FILE *file, size_t newSize) { return ftruncate(fileno(file), newSize); }

#elif _WIN32
#include <fileapi.h>
int setFileSize(FILE *file, size_t newSize) {
    long tmp = ftell(file);
    fseek(file, newSize, SEEK_SET);
    return SetEndOfFile(fileno(file));
}
#endif

void printLongFromFile(FILE *file, long count, const char *massage) {
    long pos = ftell(file);
    rewind(file);
    long *testReadPtr = calloc(count, sizeof(long));
    fread(testReadPtr, sizeof(long), count, file);
    printf("%s", massage);
    for (int i = 0; i < count; i++) printf("%ld, ", testReadPtr[i]);
    putc('\n', stdout);
    free(testReadPtr);
    fseek(file, pos, SEEK_SET);
}

int mapIndex(FILE *file, int index, long threshold, int len) {
    int res = 0;
    long ptr = ftell(file);
    rewind(file);
    for (int j = index + 1; j > 0; j--) {
        long temp = LONG_MIN;
        for (; temp <= threshold && res < len; res++) fread(&temp, sizeof(long), 1, file);
        if (res >= len) return -1;
    }
    fseek(file, ptr, SEEK_SET);
    return res - 1;
}
int calcLen(FILE *file, int len, long threshold) {
    int res = 0;
    long temp = 0;
    long ptr = ftell(file);
    rewind(file);
    for (int i = 0; i < len; i++) {
        fread(&temp, sizeof(long), 1, file);
        if (temp > threshold) res++;
    }
    fseek(file, ptr, SEEK_SET);

    return res;
}

void sortFile(FILE *file, int inputArrayLength, long threshold) {
    int swapped;
    int n = calcLen(file, inputArrayLength, threshold);
    long a, b = 0;
    for (int i = 0; i < n - 1; i++) {
        swapped = 0;
        for (int j = 0; j < n - i - 1; j++) {
            int j_ = mapIndex(file, j, threshold, inputArrayLength);
            int j_1 = mapIndex(file, j + 1, threshold, inputArrayLength);

            fseek(file, j_ * sizeof(long), SEEK_SET);
            fread(&a, sizeof(long), 1, file);

            fseek(file, j_1 * sizeof(long), SEEK_SET);
            fread(&b, sizeof(long), 1, file);

            if (a < b) {
                fseek(file, j_ * sizeof(long), SEEK_SET);
                fwrite(&b, sizeof(long), 1, file);
                fseek(file, j_1 * sizeof(long), SEEK_SET);
                fwrite(&a, sizeof(long), 1, file);
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}


int main(int argc, char const *argv[]) {
    FILE *file;
    long *inputArray;
    long threshold = LONG_MIN;
    size_t inputArrayLength = 0;

    long demoArray[] = {80, 63, 55, 32, 89, 72, 1, 52, 21, 99, 26, 25, 27, 91, 74, 81, 13, 94, 17, 92};
    long demoArrayLength = sizeof(demoArray) / sizeof(long);
    long demoThreshold = 30;

    if (argc < 2) {
        puts("Error: No file name was passed.");
        return 1;
    }

    if ((file = fopen(argv[1], "w+b")) == NULL) {
        puts("Error: Cannot open provided file.");
        return 1;
    }

    

    switch (start("Lab work 1")) {
        case USER_INPUT:
            readMultLongWithDialog(&inputArray, ',', "Please enter list of numbers: ", &inputArrayLength);
            readLongWithDialog(&threshold, "Please enter sort threshold: ");
            fwrite(inputArray, sizeof(long), inputArrayLength, file);
            free(inputArray);
            break;
        case RANDOM_INPUT:
            readLongWithDialog_v(&inputArrayLength, "Please enter array length: ", isG0);

            long max, min, seed = 0;

            readLongWithDialog(&max, "Please enter max value (Natural number): ");
            readLongWithDialog(&min, "Please enter min value (Natural number): ");
            readLongWithDialog_v(&seed, "Please enter seed: ", isG0);

            if ((inputArray = calloc(inputArrayLength, sizeof(long))) == NULL) handleMallocError();

            for (long i = 0; i < inputArrayLength; i++) {
                seed = MrandomUInt(seed);
                inputArray[i] = map(0, 254803967, max, min, (double)seed);
            }

            threshold = map(0, 254803967, max, min, (double)seed);

            printf("Threshold: %ld\n", threshold);

            fwrite(inputArray, sizeof(long), inputArrayLength, file);
            free(inputArray);
            break;

        case DEMO_INPUT:
            fwrite(demoArray, sizeof(long), demoArrayLength, file);
            inputArrayLength = demoArrayLength;
            threshold = demoThreshold;
            printf("Threshold: %ld\n", threshold);
            break;

            
        case EXIT_INPUT:
            exit(0);
            break;
        default:
            puts("Somthing horable went wrong....");
            exit(1);
            break;
    }

    // check write result
    printLongFromFile(file, inputArrayLength, "Written input: ");

    // calc and write sum
    rewind(file);
    long acc = 0;
    long temp = 0;
    for (int i = 0; i < inputArrayLength; i++) {
        fread(&temp, sizeof(long), 1, file);
        if (temp % 2 == 0) acc += temp;
    }
    fwrite(&acc, sizeof(long), 1, file);

    // check write result
    long testRead = 0;
    fseek(file, -sizeof(long), SEEK_END);
    fread(&testRead, sizeof(long), 1, file);
    printf("Written sum: %ld\n", testRead);

    // delete all even numbers
    rewind(file);
    long currentNumber = 0;
    long *buffer = NULL;
    if ((buffer = calloc(inputArrayLength, sizeof(long))) == NULL) handleMallocError();
    for (int i = 0; i < inputArrayLength; i++) {
        fseek(file, i * sizeof(long), SEEK_SET);
        fread(&currentNumber, sizeof(long), 1, file);

        if (currentNumber % 2 == 0) {
            int tmpLen = inputArrayLength - i;
            for (int j = 0; (j < tmpLen) && (currentNumber % 2 == 0); j++) {
                inputArrayLength--;
                fread(&currentNumber, sizeof(long), 1, file);
            }
            fseek(file, -sizeof(long), SEEK_CUR);
            fread(buffer, sizeof(long), inputArrayLength - i + 1, file);

            fseek(file, i * sizeof(long), SEEK_SET);
            fwrite(buffer, sizeof(long), inputArrayLength - i + 1, file);
        }
    }
    setFileSize(file, (inputArrayLength + 1) * sizeof(long));
    free(buffer);

    // check result
    printLongFromFile(file, inputArrayLength, "Written delition result: ");

    // sort
    sortFile(file, inputArrayLength, threshold);
    // check result
    printLongFromFile(file, inputArrayLength, "Written sort result: ");

    fclose(file);
    return 0;
}
