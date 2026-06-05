#pragma once

#include "esp_camera.h"

// Resultado simple: bloque rojo o verde en el centro de la franja
struct DetectResult {
  bool redCenter;
  bool greenCenter;
};

DetectResult detectCubes(camera_fb_t* frame);
void detectUnoSerialBegin(void);
void detectSendToUno(const DetectResult* d);
