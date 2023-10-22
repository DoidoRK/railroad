#ifndef _PRINT_H_
#define _PRINT_H_

#include "stdio.h"
#include "types.h"
#include "conio_linux.h"

// Function to get the character at a specific coordinates
char get_character(char* map, int row, int col) {
    if (row >= 0 && row < ROWS && col >= 0 && col < COLUMNS) {
        int index = row * (COLUMNS + 1) + col;
        return map[index];
    }
    return '\0';
}

// Function to modify a character in the buffer at specific coordinates
void modify_character(char* buffer, int row, int col, char character) {
    if (row >= 0 && row < ROWS && col >= 0 && col < COLUMNS) {
        int index = row * (COLUMNS + 1) + col;
        buffer[index] = character;
    }
}

#endif //_PRINT_H_