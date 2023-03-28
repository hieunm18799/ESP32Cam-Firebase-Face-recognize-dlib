#ifndef SDCARD_H_
#define SDCARD_H_

#include "FS.h"                // SD Card ESP32
#include "SD.h"            // SD Card ESP32

int fileCountInFolder(String folder_name);
bool createFile(String file_name_full_path, uint8_t *content, size_t length);

#endif