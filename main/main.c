#include <stdio.h>
#include "esp_spiffs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "conio_linux.h"
#include "esp_log.h"
#include "types.h"
#include "print.h"

#define PRINT_DELAY 500

static const char *TAG = "file_system";

station_t stations[NUM_STATIONS];
train_t trains[NUM_TRAINS];

int findPositionInRailway(const position_t *railway, size_t num_points, const position_t *target) {
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
        stations[i].station_id = 65 + i;
    }
    ESP_LOGI(TAG, "Stations successfully initiated.");
}

void init_trains(){
    ESP_LOGI(TAG, "Trains successfully initated.");
    for (uint8_t i = 0; i < NUM_TRAINS; i++) {
        //In conio_linux.h the first color is black, which is the same as the terminal background
        //So we start from the first color on.
        trains[i].train_color = 1 + i;
        trains[i].current_pos = STATIONS_POSITIONS[i];  //Trains start in different stations.
        trains[i].train_number = 1 + i ;
        trains[i].current_index = findPositionInRailway(RAILWAY,RAILWAY_SIZE, &trains[i].current_pos);
    }
}

void print_map_task(void *args){
    size_t contentSize = 0;
    // Open the file for reading
    FILE *file = fopen("/files/map.txt", "r");

    if (file == NULL) {
        ESP_LOGE(TAG,"Failed to open the file.\n");
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
        ESP_LOGE(TAG,"Failed to read the file.\n");
        return;
    }
    fileContent[contentSize] = '\0';

    while (1) {
        gotoxy(0,0);
        printf("%s\n", fileContent);
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
        ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label,&total,&used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get partition info (%s)", esp_err_to_name(ret));
        return;
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }
    
    init_stations();
    init_trains();
    clrscr();

    xTaskCreate(print_map_task, "PrintMapTask", 4096, NULL, 3, NULL);
}