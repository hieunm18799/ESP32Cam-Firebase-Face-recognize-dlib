#include <string.h>
#include "Camera.h"
// #include "SDCard.h"
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "SDCard.h"

// Provide the token generation process info.
#include <addons/TokenHelper.h>
// Provide the SD card interfaces setting and mounting
#include <addons/SDHelper.h>

#include <soc/soc.h>          // disable brownout problems
#include <soc/rtc_cntl_reg.h> // disable brownout problems

/* 1. Define the WiFi credentials */
#define WIFI_SSID "*"
#define WIFI_PASSWORD "*"

/* 2. Define the API Key */
#define API_KEY "*"

/* 3. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "*"
#define USER_PASSWORD "*"

/* 4. Define the Firebase storage bucket ID e.g bucket-name.appspot.com */
#define STORAGE_BUCKET_ID "*.appspot.com"

// The Firebase Storage upload callback function
void fcsUploadCallback(FCS_UploadStatusInfo info);

bool takePictureAndSaveToSDCard(int *imageCounter);

void initWiFi();