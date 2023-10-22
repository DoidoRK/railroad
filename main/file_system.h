#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_
#include <stdio.h>
#include "esp_spiffs.h"
#include "esp_log.h"
#include "types.h"

#define FILE_NAME "/files/map.txt"

static const char *FILE_SYSTEM_TAG = "file_system";


esp_err_t init_file_system(){
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
        return ESP_FAIL;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label,&total,&used);
    if (ret != ESP_OK) {
        ESP_LOGE(FILE_SYSTEM_TAG, "Failed to get partition info (%s)", esp_err_to_name(ret));
        return ESP_FAIL;
    } else {
        ESP_LOGI(FILE_SYSTEM_TAG, "Partition size: total: %d, used: %d", total, used);
        return ESP_OK;
    }
}

esp_err_t read_file_content(char file_content[BUFF_SIZE]){
    size_t content_size;

    // Open the file for reading
    FILE *file = fopen(FILE_NAME, "r");

    if (file == NULL) {
        ESP_LOGE(FILE_SYSTEM_TAG, "Failed to open the file.\n");
        return ESP_FAIL;
    }

    // Seek to the beginning of the file
    fseek(file, 0, SEEK_SET);

    // Check the file size
    fseek(file, 0, SEEK_END);
    content_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read the file content into the buffer
    size_t bytes_read = fread(file_content, 1, BUFF_SIZE, file);

    if (bytes_read != content_size) {
        ESP_LOGE(FILE_SYSTEM_TAG, "Failed to read the file.\n");
        return ESP_FAIL;
    }
    file_content[BUFF_SIZE] = '\0';

    return ESP_OK;
}

#endif //_FILE_SYSTEM_H_