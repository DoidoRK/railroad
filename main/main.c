#include <stdio.h>
#include "esp_spiffs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "conio_linux.h"
#include "esp_log.h"
#include "types.h"
#include "print.h"

static const char *FILE_SYSTEM_TAG = "file_system";
static const char *MAIN_TAG = "railroad_system";

station_t stations[NUM_STATIONS];
train_t trains[NUM_TRAINS];

SemaphoreHandle_t trains_mutex, stations_mutex;

int8_t find_position_in_railway(const position_t *railway, size_t num_points, const position_t *target) {
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
        stations[i].station_index = find_position_in_railway(RAILWAY,RAILWAY_SIZE,&stations[i].station_pos);
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
        trains[i].train_color = 1 + i;
        trains[i].current_pos = STATIONS_POSITIONS[i];  //Trains start in different stations.
        trains[i].train_number = i+1;
        trains[i].current_index = find_position_in_railway(RAILWAY,RAILWAY_SIZE, &trains[i].current_pos);
        trains[i].status = 2;
        if(trains[i].current_index == -1){
            ESP_LOGE(MAIN_TAG, "Train position not found in the railway");
            return ESP_FAIL;
        }
    }
    ESP_LOGI(MAIN_TAG, "Trains successfully initated.");
    return ESP_OK;
}

void station_task(void *params){
    station_t *p_station = (station_t *)params;
    while (true)
    {
        uint8_t i = 0;
        xSemaphoreTake(stations_mutex, portMAX_DELAY);
        p_station->train_parked = -1;
        while(i < NUM_TRAINS){
            if(trains[i].current_index == (*p_station).station_index){
                p_station->train_parked = i;
            }
            i++;
        }
        xSemaphoreGive(stations_mutex);
        vTaskDelay(pdMS_TO_TICKS(STATION_UPDATE_TIME));
    }
}

void train_task(void *params){
    train_t *p_train = (train_t *)params;
    station_t *p_next_station = &stations[(*p_train).train_number];
    while (true)
    {
        xSemaphoreTake(trains_mutex, portMAX_DELAY);
        p_train->current_index++;
        if((*p_train).current_index > RAILWAY_SIZE){
            p_train->current_index = 0;
        }
        p_train->current_pos = RAILWAY[(*p_train).current_index];

        //Checks if there is a train stopped in the next station
        int i = 0;
        p_train->status = NORMAL_SPEED;
        if((*p_next_station).train_parked >= 0){
            //If there is, reduce speed.
            p_train->status = SLOW_SPEED;
        }

        //Checks if it's currently stopped in a station
        while(i < NUM_STATIONS){
            if(stations[i].station_index == (*p_train).current_index){
                //If it is, stops and waits for the passengers
                p_train->status = STOPPED_IN_STATION;
                if(i+1 < NUM_STATIONS){
                    p_next_station = &stations[i+1];
                } else {
                    p_next_station = &stations[0];
                }
            }
            i++;
        }

        xSemaphoreGive(trains_mutex);
        switch ((*p_train).status)
        {
        case STOPPED_IN_STATION: //Train is stopped in station
            vTaskDelay(pdMS_TO_TICKS(TRAIN_STOP_IN_STATION_DELAY_IN_MS));
            break;
        
        case SLOW_SPEED: //There's a train in the next station
            vTaskDelay(pdMS_TO_TICKS(TRAIN_SLOW_SPEED_IN_MS));
            break;

        default:    //The next station is empty
            vTaskDelay(pdMS_TO_TICKS(TRAIN_NORMAL_SPEED_IN_MS));
            break;
        }
    }
}

void print_system_task(void *params){
    size_t contentSize = 0;
    // Open the file for reading
    FILE *file = fopen("/files/map.txt", "r");

    if (file == NULL) {
        ESP_LOGE(FILE_SYSTEM_TAG, "Failed to open the file.\n");
        return;
    }
    // Seek to the beginning of the file
    fseek(file, 0, SEEK_SET);

    // Check the file size
    fseek(file, 0, SEEK_END);
    contentSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    //Buffer to store the file contents.
    char fileContent[contentSize];

    // Read the file content into the buffer
    size_t bytesRead = fread(fileContent, 1, contentSize, file);

    if (bytesRead != contentSize) {
        ESP_LOGE(FILE_SYSTEM_TAG, "Failed to read the file.\n");
        return;
    }
    fileContent[contentSize] = '\0';

    while (1) {
        int z = 0;
        for (size_t i = 0; i < NUM_STATIONS/2; i++)
        {
            for (size_t j = 0; j < NUM_STATIONS/3; j++)
            {
                print_station_time_table(27+(30*i),2+(10*j),trains,stations[z]);
                z++;
            }
        }
        gotoxy(0,0);
        printf("%s", fileContent);
        print_trains(trains);
        gotoxy(0,22);
        vTaskDelay(pdMS_TO_TICKS(PRINT_DELAY));
    }
}


void app_main() {
    // Mount the SPIFFS file system
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/files",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };
    
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        ESP_LOGE(FILE_SYSTEM_TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        exit(1);
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label,&total,&used);
    if (ret != ESP_OK) {
        ESP_LOGE(FILE_SYSTEM_TAG, "Failed to get partition info (%s)", esp_err_to_name(ret));
        exit(1);
    } else {
        ESP_LOGI(FILE_SYSTEM_TAG, "Partition size: total: %d, used: %d", total, used);
    }
    
    trains_mutex = xSemaphoreCreateMutex();
    if (trains_mutex == NULL) {
        ESP_LOGE(MAIN_TAG, "Trains mutex creation failed");
        exit(1);
    }
    xSemaphoreGive(trains_mutex);

    stations_mutex = xSemaphoreCreateMutex();
    if (stations_mutex == NULL) {
        ESP_LOGE(MAIN_TAG, "Stations mutex creation failed");
        exit(1);
    }
    xSemaphoreGive(stations_mutex);

    ESP_ERROR_CHECK(init_trains());
    ESP_ERROR_CHECK(init_stations());
    clrscr();

    xTaskCreate(print_system_task, "PrintSystemTask", 4096, NULL, 3, NULL);

    for (uint8_t i = 0; i < NUM_STATIONS; i++)
    {
        xTaskCreate(station_task, "StationTask", 4096, &stations[i], 1, NULL);
    }

    for (uint8_t i = 0; i < NUM_TRAINS; i++)
    {
        xTaskCreate(train_task, "TrainTask", 4096, &trains[i], 1, NULL);
    }
}