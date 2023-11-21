#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include "types.h"

uint8_t calculate_time_to_station(
    uint8_t train_index, uint8_t station_index,
    uint8_t station_stop_time, uint8_t train_normal_speed_in_seconds,
    uint8_t train_slow_speed_in_seconds, int train_status
    ) {
    uint8_t total_time = 0;
    
    //Computes time between train and station based only on train tracks between them
    if(station_index > train_index){
        total_time = station_index - train_index;
    } else {
        total_time = RAILWAY_SIZE - (train_index - station_index);
    }
    
    //Adds slowed speed time contributions
    if(train_status == SLOW_SPEED){
        total_time += train_slow_speed_in_seconds;
    }
    
    return total_time;
}

#endif //_UTILS_H_