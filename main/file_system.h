#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_
#include <stdio.h>
#include "esp_spiffs.h"
#include "esp_log.h"
#include "types.h"

#define FILE_NAME "/files/map.txt"

static const char *FILE_SYSTEM_TAG = "file_system";


void init_file_system(){
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
}

void read_file_content(char file_content[ROWS * (COLUMNS + 1)]){
    // size_t content_size = 0;

    // Open the file for reading
    FILE *file = fopen(FILE_NAME, "r");

    if (file == NULL) {
        ESP_LOGE(FILE_SYSTEM_TAG, "Failed to open the file.\n");
        // return NULL;
    }
    // // Seek to the beginning of the file
    // fseek(file, 0, SEEK_SET);

    // // Check the file size
    // fseek(file, 0, SEEK_END);
    // content_size = ftell(file);
    // fseek(file, 0, SEEK_SET);

    // //Buffer to store the file contents.
    // char file_content[content_size];

    // Read the file content into the buffer
    // size_t bytesRead = fread(file_content, 1, content_size, file);
    size_t bytesRead = fread(file_content, 1, ROWS * (COLUMNS + 1), file);

    // if (bytesRead != content_size) {
    //     ESP_LOGE(FILE_SYSTEM_TAG, "Failed to read the file.\n");
    //     // return NULL;
    // }
    if (bytesRead != ROWS * (COLUMNS + 1)) {
        ESP_LOGE(FILE_SYSTEM_TAG, "Failed to read the file.\n");
        // return NULL;
    }
    file_content[ROWS * (COLUMNS + 1)] = '\0';
    // return file_content;
}

#endif //_FILE_SYSTEM_H_