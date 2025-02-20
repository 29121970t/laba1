#include "LabMenu.h"

#define TERMINAL_LINE_WIDTH 174

void printLineWithOffset(char* line, int offset) {
    char tmpLine[TERMINAL_LINE_WIDTH] = {0};
    printf("%*c%s\n", offset, ' ', line);
}
size_t getStringLength(char* string) {
    size_t i = 0;
    for (; string[i] != '\0'; i++);
    return i;
}

void printCenteredLine(char* line) {
    int offset = (TERMINAL_LINE_WIDTH - getStringLength(line)) / 2;
    printLineWithOffset(line, offset);
}
int ___isInputC(long c) { return c >= 1 && c <= 4; }

int start(char * title){
    printCenteredLine("Lab work 1");
    printLineWithOffset("Would you like to:", 0);
    printLineWithOffset("1. Enter data with keyboard", 4);
    printLineWithOffset("2. Generate random input", 4);
    printLineWithOffset("3. Run in demo mode (hardcoded input)", 4);
    printLineWithOffset("4. Exit programm", 4);

    long answ = 0;
    readLongWithDialog_v(&answ, ">", ___isInputC);
    return answ;
}
