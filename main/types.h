#ifndef _TYPES_H_
#define _TYPES_H_

#include <inttypes.h>

#define ROWS 21
#define COLUMNS 23
#define BUFF_SIZE ROWS * (COLUMNS + 1)

#define NUM_TRAINS 4
#define NUM_STATIONS 6
#define RAILWAY_SIZE 52

typedef struct position_struct {
    uint8_t x;
    uint8_t y;
} position_t;

typedef struct train_struct {
    position_t current_pos;
    int8_t current_index;
    uint8_t train_color;
    uint8_t train_number;
} train_t;

typedef struct station_struct {
    position_t station_pos;
    int8_t station_index;
    uint8_t station_id;
    int8_t train_parked;
} station_t;


const position_t RAILWAY[RAILWAY_SIZE] = {
    {5, 8}, {6, 8}, {8, 9}, {9, 10}, {10, 11}, {10, 12}, {10, 13}, {9, 14}, {7, 15}, {6, 15}, {5, 15},
    {3, 16}, {2, 17}, {1, 18}, {2, 19}, {3, 20}, {5, 21}, {6, 21}, {7, 21}, {8, 21}, {10, 20}, {11, 19},
    {12, 18}, {13, 17}, {14, 16}, {15, 15}, {16, 14}, {16, 13}, {16, 12}, {16, 11}, {17, 10}, {18, 9},
    {19, 8}, {20, 7}, {19, 6}, {18, 5}, {16, 4}, {14, 3}, {13, 2}, {11, 1}, {10, 1}, {9, 1}, {8, 1}, {7, 1},
    {6, 1}, {5, 1}, {3, 2}, {2, 3}, {1, 4}, {1, 5}, {2, 6}, {3, 7}
};

const position_t STATIONS_POSITIONS[NUM_STATIONS] = {
    {6, 8},
    {5, 15},
    {7, 21},
    {16, 13},
    {20, 7},
    {7, 1}
};

#endif //_TYPES_H_