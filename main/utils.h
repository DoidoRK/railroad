#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <esp_log.h>
#include <esp_err.h>
#include "types.h"
#include "conio_linux.h"

static const char *UTILS_TAG = "utils";


// Function to modify a character in the buffer at specific coordinates
esp_err_t modify_character(char buffer[], int num_rows, int num_cols, int row_index, int col_index, char new_char) {
    if (row_index < 0 || row_index >= num_rows || col_index < 0 || col_index >= num_cols) {
        // Check for valid indices
        ESP_LOGE(UTILS_TAG, "Invalid indices.");
        return ESP_FAIL;
    }

    int index = row_index * num_cols + col_index;
    buffer[index] = new_char;
    return ESP_OK;
}

// Function to copy the a buffer to a new buffer
esp_err_t copy_buffer(char* dest_buffer, char* origin_buffer , size_t buffer_size) {
    if (buffer_size < sizeof(origin_buffer)) {
        ESP_LOGE(UTILS_TAG, "Buffer size is too small to copy the map.");
        return ESP_FAIL;
    }
    memcpy(dest_buffer, origin_buffer, buffer_size);
    return ESP_OK;
}


void clear_buffer(char buffer[], int bufferSize) {
    for (int i = 0; i < bufferSize; i++) {
        buffer[i] = ' ';
    }
}

void update_trains(char buffer[BUFF_SIZE], train_t active_trains[NUM_TRAINS]){
    //Prints each train in the screen.
    for (uint8_t i = 0; i < NUM_TRAINS; i++)
    {
        modify_character(buffer, ROWS, COLUMNS, active_trains[i].current_pos.x, active_trains[i].current_pos.y, '1');
        // modify_character(buffer,active_trains[i].current_pos.x-1,active_trains[i].current_pos.y-1,'a');
    }
}

#endif //_UTILS_H_