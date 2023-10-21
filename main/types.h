#ifndef _TYPES_H_
#define _TYPES_H_

#include <inttypes.h>

typedef struct position_struct {
    uint8_t x;
    uint8_t y;
} position_t;

typedef struct train_struct {
    position_t current_pos;
    position_t next_pos;
    uint8_t train_color;
    uint8_t train_number;
} train_t;

#endif //_TYPES_H_