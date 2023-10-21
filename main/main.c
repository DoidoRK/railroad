#include <stdio.h>
#include "esp_spiffs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "conio_linux.h"
#include "esp_log.h"
#include "types.h"
#include "print.h"

#define PRINT_DELAY 100
#define TRAIN_STOP_IN_STATION_DELAY 2000
#define TRAIN_MOVE_DELAY 200

static const char *FILE_SYSTEM_TAG = "file_system";
static const char *MAIN_TAG = "railroad_system";

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

void init_stations(){
    for (uint8_t i = 0; i < NUM_STATIONS; i++) {
        stations[i].station_pos = STATIONS_POSITIONS[i];
        stations[i].station_index = findPositionInRailway(RAILWAY,RAILWAY_SIZE,&stations[i].station_pos);
        if(-1 == stations[i].station_index){
            ESP_LOGE(MAIN_TAG, "Station position not found in the railway");
            return;
        }
        stations[i].station_id = 65 + i;
        stations[i].is_train_parked = false;
    }
    ESP_LOGI(MAIN_TAG, "Stations successfully initiated.");
}

void init_trains(){
    for (uint8_t i = 0; i < NUM_TRAINS; i++) {
        //In conio_linux.h the first color is black, which is the same as the terminal background
        //So we start from the first color on.
        trains[i].train_color = 1 + i;
        trains[i].current_pos = STATIONS_POSITIONS[i];  //Trains start in different stations.
        trains[i].train_number = 1 + i;
        trains[i].current_index = findPositionInRailway(RAILWAY,RAILWAY_SIZE, &trains[i].current_pos);
        if(-1 == trains[i].current_index){
            ESP_LOGE(MAIN_TAG, "Train position not found in the railway");
            return;
        }
    }
    ESP_LOGI(MAIN_TAG, "Trains successfully initated.");
}

void train_task(void *params){
    uint8_t *p_train_number = (uint8_t *)params;
    uint8_t train_number = *p_train_number;
    bool stopped_in_station = false;
    while (true)
    {
        if(trains[train_number].current_index < RAILWAY_SIZE){
            xSemaphoreTake(stations_mutex, portMAX_DELAY);
            stations[trains[train_number].current_index].is_train_parked = false;
            xSemaphoreGive(stations_mutex);
            xSemaphoreTake(trains_mutex, portMAX_DELAY);
            trains[train_number].current_index++;
            trains[train_number].current_pos = RAILWAY[trains[train_number].current_index];
            xSemaphoreGive(trains_mutex);
        } else {
            trains[train_number].current_index = 0;
        }
        stopped_in_station = false;
        for (size_t i = 0; i < NUM_STATIONS; i++)
        {
            if(stations[i].station_index == trains[train_number].current_index){
                stopped_in_station = true;
                xSemaphoreTake(stations_mutex, portMAX_DELAY);
                stations[i].is_train_parked = true;
                xSemaphoreGive(stations_mutex);
            }
        }
        if(stopped_in_station){
            vTaskDelay(pdMS_TO_TICKS(TRAIN_STOP_IN_STATION_DELAY));
        } else {
            vTaskDelay(pdMS_TO_TICKS(TRAIN_MOVE_DELAY));
        }
        vTaskDelay(pdMS_TO_TICKS(TRAIN_MOVE_DELAY));
    }
}

void print_map_task(void *params){
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
        print_trains(trains);
        gotoxy(0,0);
        printf("%s\n", fileContent);
        vTaskDelay(pdMS_TO_TICKS(PRINT_DELAY));
        gotoxy(0,22);
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
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label,&total,&used);
    if (ret != ESP_OK) {
        ESP_LOGE(FILE_SYSTEM_TAG, "Failed to get partition info (%s)", esp_err_to_name(ret));
        return;
    } else {
        ESP_LOGI(FILE_SYSTEM_TAG, "Partition size: total: %d, used: %d", total, used);
    }
    
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

    init_stations();
    init_trains();
    clrscr();


    xTaskCreate(print_map_task, "PrintMapTask", 4096, NULL, 3, NULL);

    for (uint8_t i = 0; i < NUM_TRAINS; i++)
    {
        xTaskCreate(train_task, "TrainTask", 4096, &i, 2, NULL);
    }
}