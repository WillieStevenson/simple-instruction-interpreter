/*
 *
 *
 * VM helper functions
 *
 * Author : Willie Stevenson
 *
 *
 */

#ifndef UTIL_H
#define UTIL_H


void displayCharArray(char* IR, int size);
void displayMemory(char memory[][6], int iSize, int jSize);
int getOpCode(char* IR);
int charToInt(char c);
void intToCharArray(int i, char* destArr);
void charArrayCopy(char* sourceArr, char* destArr);
void loadProgramIntoMemory(Vm* vm, char* filename);

#endif /* UTIL_H */