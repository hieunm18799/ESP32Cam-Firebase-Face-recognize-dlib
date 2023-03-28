#include "Camera.h"

esp_err_t initCamera()
{
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound())
  {
    config.grab_mode = CAMERA_GRAB_LATEST;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10; // lower the better but may need to lower xclk_freq_hz
    config.fb_count = 4;
  }
  else
  {
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_DRAM;
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Camera init
  esp_err_t err = esp_camera_init(&config);

  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return err;
  }

  sensor_t *s = esp_camera_sensor_get();
  // s->set_brightness(s, 1);     // -2 to 2
  // s->set_contrast(s, 1);       // -2 to 2
  // s->set_saturation(s, -1);     // -2 to 2
  // s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  // s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
  // s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
  // s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
  // s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
  // s->set_aec2(s, 0);           // 0 = disable , 1 = enable
  // s->set_ae_level(s, 0);       // -2 to 2
  // s->set_aec_value(s, 300);    // 0 to 1200
  // s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
  // s->set_agc_gain(s, 0);       // 0 to 30
  // s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
  // s->set_bpc(s, 0);            // 0 = disable , 1 = enable
  // s->set_wpc(s, 1);            // 0 = disable , 1 = enable
  // s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
  // s->set_lenc(s, 1);           // 0 = disable , 1 = enable
  // s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
  // s->set_vflip(s, 0);          // 0 = disable , 1 = enable
  // s->set_dcw(s, 1);            // 0 = disable , 1 = enable
  // s->set_colorbar(s, 0);       // 0 = disable , 1 = enable
  ////////////////////////
  s->set_gain_ctrl(s, 1);     // auto gain on
  s->set_exposure_ctrl(s, 1); // auto exposure on
  s->set_awb_gain(s, 1);      // Auto White Balance enable (0 or 1)
  s->set_brightness(s, 1);    // up the brightness just a bit
  s->set_contrast(s, 1);      // -2 to 2
  s->set_saturation(s, -1);   // lower the saturation
  
  // the cam needs time to setup
  delay(15000);
  return ESP_OK;
}

void setupFlashPWM()
{
  ledcSetup(LED_CHANNEL, LED_PIN, LED_RESOLUTION);
  ledcAttachPin(BRIGHT_LED_PIN, LED_CHANNEL);
  brightLed(0);
}

// change illumination LED brightness
void brightLed(byte ledBrightness)
{
  ledcWrite(LED_CHANNEL, ledBrightness); // change LED brightness (0 - 255)
  Serial.println("Brightness changed to " + String(ledBrightness));
}