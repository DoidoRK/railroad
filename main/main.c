#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <string.h>
#include "conio_linux.h"
#include "esp_log.h"
#include "types.h"
#include "print.h"
#include "file_system.h"

#define PRINT_DELAY 300
#define TRAIN_STOP_IN_STATION_DELAY 5000
#define TRAIN_NORMAL_SPEED 250
#define TRAIN_SLOW_SPEED 750
#define STATION_UPDATE_TIME 100

static const char *MAIN_TAG = "railroad_system";

char original_map[ROWS * (COLUMNS + 1)];

char animation_buffer[ROWS * (COLUMNS + 1)];

station_t stations[NUM_STATIONS];
train_t trains[NUM_TRAINS];

SemaphoreHandle_t trains_mutex, stations_mutex;

int8_t findPositionInRailway(const position_t *railway, size_t num_points, const position_t *target) {
    for (int i = 0; i < num_points; i++) {
        if (railway[i].x == target->x && railway[i].y == target->y) {
            return i;
        }
    }
    return -1; // Not found
}

esp_err_t init_stations(){
    for (uint8_t i = 0; i < NUM_STATIONS; i++) {
        stations[i].station_pos = STATIONS_POSITIONS[i];
        stations[i].station_index = findPositionInRailway(RAILWAY,RAILWAY_SIZE,&stations[i].station_pos);
        if(-1 == stations[i].station_index){
            ESP_LOGE(MAIN_TAG, "Station position not found in the railway");
            return ESP_FAIL;
        }
        stations[i].station_id = i;
        stations[i].train_parked = -1;
    }
    ESP_LOGI(MAIN_TAG, "Stations successfully initiated.");
    return ESP_OK;
}

esp_err_t init_trains(){
    for (uint8_t i = 0; i < NUM_TRAINS; i++) {
        //In conio_linux.h the first color is black, which is the same as the terminal background
        //So we start from the first color on.
        trains[i].train_color = 1 + i;
        trains[i].current_pos = STATIONS_POSITIONS[i];  //Trains start in different stations.
        trains[i].train_number = 1 + i;
        trains[i].current_index = findPositionInRailway(RAILWAY,RAILWAY_SIZE, &trains[i].current_pos);
        if(-1 == trains[i].current_index){
            ESP_LOGE(MAIN_TAG, "Train position not found in the railway");
            return ESP_FAIL;
        }
    }
    ESP_LOGI(MAIN_TAG, "Trains successfully initated.");
    return ESP_OK;
}

void station_task(void *params){
    uint8_t *p_station_id = (uint8_t *)params;
    uint8_t station_id = *p_station_id;
    while (true)
    {
        uint8_t i = 0;
        bool train_is_parked = false;
        while(i < NUM_TRAINS && !train_is_parked ){
            if(trains[i].current_index == stations[station_id].station_index){
                xSemaphoreTake(stations_mutex, portMAX_DELAY);
                stations[station_id].train_parked = i;
                xSemaphoreGive(stations_mutex);
                train_is_parked = true;
            }
            i++;
        }
        if(!train_is_parked){
            xSemaphoreTake(stations_mutex, portMAX_DELAY);
            stations[station_id].train_parked = -1;
            xSemaphoreGive(stations_mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(STATION_UPDATE_TIME));
    }
}

void train_task(void *params){
    uint8_t *p_train_number = (uint8_t *)params;
    uint8_t train_number = *p_train_number;
    station_t next_station = stations[train_number+1];
    while (true)
    {
        xSemaphoreTake(trains_mutex, portMAX_DELAY);
        if(trains[train_number].current_index < RAILWAY_SIZE){
            trains[train_number].current_index++;
        } else {
            trains[train_number].current_index = 0;
        }
        trains[train_number].current_pos = RAILWAY[trains[train_number].current_index];
        xSemaphoreGive(trains_mutex);

        int i = 0;
        bool stopped_in_station = false;
        while(i < NUM_STATIONS && !stopped_in_station ){
            if(stations[i].station_index == trains[train_number].current_index){
                stopped_in_station = true;
            }
            i++;
        }
        if(stopped_in_station){
            if( NUM_STATIONS > next_station.station_id+1){
                next_station = stations[0];
            } else{
                next_station = stations[next_station.station_id+1];
            }
            vTaskDelay(pdMS_TO_TICKS(TRAIN_STOP_IN_STATION_DELAY));
        } else {
            if(-1 == next_station.train_parked){
                vTaskDelay(pdMS_TO_TICKS(TRAIN_SLOW_SPEED));
            } else {
                vTaskDelay(pdMS_TO_TICKS(TRAIN_NORMAL_SPEED));
            }
        }
    }
}

void print_map_task(void *params){
    while (1) {
        // print_trains(trains);
        gotoxy(0,0);
        printf("%s\n", original_map);
        vTaskDelay(pdMS_TO_TICKS(PRINT_DELAY));
        gotoxy(0,22);
    }
}


void app_main() {
    ESP_ERROR_CHECK(init_file_system());
    ESP_ERROR_CHECK(read_file_content(original_map));
    
    trains_mutex = xSemaphoreCreateMutex();
    if (trains_mutex == NULL) {
        ESP_LOGE(MAIN_TAG, "Trains mutex creation failed");
        return;
    }
    xSemaphoreGive(trains_mutex);

    stations_mutex = xSemaphoreCreateMutex();
    if (stations_mutex == NULL) {
        ESP_LOGE(MAIN_TAG, "Stations mutex creation failed");
        return;
    }
    xSemaphoreGive(stations_mutex);

    ESP_ERROR_CHECK(init_stations());
    ESP_ERROR_CHECK(init_trains());

    clrscr();

    xTaskCreate(print_map_task, "PrintMapTask", 4096, NULL, 3, NULL);

    for (uint8_t i = 0; i < NUM_STATIONS; i++)
    {
        xTaskCreate(station_task, "StationTask", 4096, &i, 1, NULL);
    }

    for (uint8_t i = 0; i < NUM_TRAINS; i++)
    {
        xTaskCreate(train_task, "TrainTask", 4096, &i, 1, NULL);
    }
}