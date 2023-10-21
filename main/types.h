#ifndef _TYPES_H_
#define _TYPES_H_

#include <inttypes.h>

#define TRAINS_NUM 4

const uint8_t RAILWAY_X_VALUES[] = {5, 6, 8, 9, 10, 10, 10, 9, 7, 6, 5, 3,
                                   2, 1, 2, 3, 5, 6, 7, 8, 10, 11, 12, 13, 14,
                                   15, 16, 16, 16, 16, 17, 18, 19, 20, 19, 18,
                                   16, 14, 13, 11, 10, 9, 8, 7, 6, 5, 3, 2, 2,
                                   2, 1, 1, 2, 3
                                   };
                                   
const uint8_t RAILWAY_Y_VALUES[] = {8, 8, 9, 10, 11, 12, 13, 14, 15, 15, 15, 16,
                                   17, 18, 19, 20, 21, 21, 21, 21, 20, 19, 18, 17,
                                   16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3,
                                   2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 3, 3, 4, 5,
                                   6, 7
                                   };

typedef struct position_struct {
    uint8_t x;
    uint8_t y;
} position_t;

typedef struct station_struct {
    position_t station_pos;
    uint8_t station_id;
} station_t;

typedef struct train_struct {
    position_t current_pos;
    position_t next_pos;
    uint8_t train_color;
    uint8_t train_number;
} train_t;

#endif //_TYPES_H_