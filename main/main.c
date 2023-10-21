#include <stdio.h>
#include "esp_spiffs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "conio_linux.h"
#include "esp_log.h"
// #include "types.h"
// #include "print.h"

static const char *TAG = "file_system";

// // const uint8_t RAILWAY_SIZE = sizeof(RAILWAY_X_VALUES) / sizeof(RAILWAY_X_VALUES[0]);

// // position_t railway_path[RAILWAY_SIZE];


// // void init_railway_path(){
// //     // Populate the railway_path variable with the data points
// //     for (size_t i = 0; i < RAILWAY_SIZE; i++) {
// //         railway_path[i].x = RAILWAY_X_VALUES[i];
// //         railway_path[i].y = RAILWAY_Y_VALUES[i];
// //     }
// // }


void print_map_task(void *args){
    // Initialize a buffer to store the file content
    
    size_t contentSize = 0;
    // Open the file for reading
    FILE *file = fopen("/files/map.txt", "r");

    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }
    // Seek to the beginning of the file
    fseek(file, 0, SEEK_SET);

    // Check the file size
    fseek(file, 0, SEEK_END);
    contentSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    //Buffer storing the file's content
    char fileContent[contentSize];

    // Read the file content into the buffer
    size_t bytesRead = fread(fileContent, 1, contentSize, file);

    if (bytesRead != contentSize) {
        printf("Failed to read the file.\n");
    }
    fileContent[contentSize] = '\0';

    while (1) {
        // Null-terminate the buffer to make it a valid C string
        // Print the file content
        gotoxy(0,0);
        printf("%s\n", fileContent);
        vTaskDelay(166 / portTICK_PERIOD_MS);
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

    vTaskDelay(2000 / portTICK_PERIOD_MS);
    clrscr();

    xTaskCreate(print_map_task, "PrintMapTask", 2048, NULL, 3, NULL);
}