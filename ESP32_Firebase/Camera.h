#ifndef CAMERA_H_
#define CAMERA_H_

#include <esp_camera.h>
#include <img_converters.h>
#include "Arduino.h"

// ESP32 Cam AI thinker (OV2640) setting pin
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

// Bright LED (Flash)
#define BRIGHT_LED_PIN 4  // onboard Illumination/flash LED pin (4)
#define LED_PIN 5000    // PWM's frequency
#define LED_CHANNEL 15   // camera uses timer1
#define LED_RESOLUTION 8 // resolution (8 = from 0 to 255)

esp_err_t initCamera();

void setupFlashPWM();

void brightLed(byte ledBrightness);

#endif