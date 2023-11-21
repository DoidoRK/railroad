#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include "types.h"

uint8_t calculateTimeToStation(
    uint8_t current_index, uint8_t target_index,
    uint8_t station_stop_time, uint8_t train_normal_speed_in_seconds,
    uint8_t train_slow_speed_in_seconds, int train_status
    ) {
    uint8_t total_time = 0;

    for (int i = 0; i < RAILWAY_SIZE; ++i) {
        int next_index = (current_index + 1) % RAILWAY_SIZE; // Circular traversal

        // Check if the current index is the target station
        if (current_index == target_index) {
            break;
        }

        uint8_t distance = next_index - current_index;

        if(train_status == SLOW_SPEED){
            total_time += distance * train_normal_speed_in_seconds + train_slow_speed_in_seconds;
        } else {
            total_time += distance * train_normal_speed_in_seconds;
        }

        // Check if the current index is a station, add station stop time
        for (int j = 0; j < NUM_STATIONS; ++j) {
            if (current_index == target_index) {
                total_time += station_stop_time;
                break;
            }
        }

        // Move to the next index
        current_index = next_index;
    }
    return total_time;
}

#endif //_UTILS_H_