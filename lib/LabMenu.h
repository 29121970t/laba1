#ifndef LABMENUE_
#define LABMENUE_

#include <stdio.h>
#include <stdlib.h>

#include "parseLib4.h"

enum MenuCases {USER_INPUT = 1, RANDOM_INPUT, DEMO_INPUT, EXIT_INPUT};

void printLineWithOffset(char* line, int offset);
size_t getStringLength(char* string);
void printCenteredLine(char* line);
int start(char * title);

#endif