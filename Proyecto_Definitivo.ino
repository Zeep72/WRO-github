/*
 * ESP32-CAM — deteccion simple rojo/verde al UNO
 * GPIO14 -> UNO pin10 | UNO pin11 -> GPIO15 | GND comun
 */

#include <Arduino.h>
#include "config.h"
#include "detection.h"
#include "esp_camera.h"

static bool startCamera(void) {
  camera_config_t cfg;
  memset(&cfg, 0, sizeof(cfg));

  cfg.ledc_channel = LEDC_CHANNEL_0;
  cfg.ledc_timer = LEDC_TIMER_0;
  cfg.pin_d0 = CAM_Y2;  cfg.pin_d1 = CAM_Y3;  cfg.pin_d2 = CAM_Y4;  cfg.pin_d3 = CAM_Y5;
  cfg.pin_d4 = CAM_Y6;  cfg.pin_d5 = CAM_Y7;  cfg.pin_d6 = CAM_Y8;  cfg.pin_d7 = CAM_Y9;
  cfg.pin_xclk = CAM_XCLK; cfg.pin_pclk = CAM_PCLK;
  cfg.pin_vsync = CAM_VSYNC; cfg.pin_href = CAM_HREF;
  cfg.pin_pwdn = CAM_PWDN; cfg.pin_reset = CAM_RESET;

#if defined(ESP_ARDUINO_VERSION) && (ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0))
  cfg.pin_sccb_sda = CAM_SIOD;
  cfg.pin_sccb_scl = CAM_SIOC;
#else
  cfg.pin_sscb_sda = CAM_SIOD;
  cfg.pin_sscb_scl = CAM_SIOC;
#endif

  cfg.xclk_freq_hz = 20000000;
  cfg.pixel_format = PIXFORMAT_RGB565;
  cfg.frame_size = FRAMESIZE_QQVGA;
  cfg.fb_count = psramFound() ? 2 : 1;
  cfg.grab_mode = CAMERA_GRAB_LATEST;
  cfg.fb_location = psramFound() ? CAMERA_FB_IN_PSRAM : CAMERA_FB_IN_DRAM;

  if (esp_camera_init(&cfg) != ESP_OK) return false;

  sensor_t* s = esp_camera_sensor_get();
  if (s) {
    s->set_saturation(s, 1);
    s->set_brightness(s, 0);
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  detectUnoSerialBegin();
  delay(300);

  if (!startCamera()) {
    Serial.println("ERROR camara");
    while (true) delay(1000);
  }
  Serial.println("ESP32-CAM listo. Salida: R0;G0 / R1;G0 / etc.");
}

void loop() {
  camera_fb_t* fb = esp_camera_fb_get();
  if (fb == NULL) {
    delay(30);
    return;
  }

  DetectResult r = detectCubes(fb);
  detectSendToUno(&r);
  esp_camera_fb_return(fb);

  delay(60);
  yield();
}
