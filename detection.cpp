#include "detection.h"
#include "config.h"
#include <Arduino.h>
#include <string.h>

static void rgb565ToRgb(uint16_t pixel, uint8_t* r, uint8_t* g, uint8_t* b) {
  pixel = (pixel >> 8) | (pixel << 8);
  *r = (pixel >> 8) & 0xF8;
  *g = (pixel >> 3) & 0xFC;
  *b = (pixel << 3) & 0xF8;
}

static bool isRed(uint8_t r, uint8_t g, uint8_t b) {
  return r > 100 && r > g + 30 && r > b + 30;
}

static bool isGreen(uint8_t r, uint8_t g, uint8_t b) {
  return g > 60 && g > r + 15 && g > b + 10;
}

DetectResult detectCubes(camera_fb_t* frame) {
  DetectResult out = {false, false};
  if (frame == NULL || frame->format != PIXFORMAT_RGB565) return out;

  const int w = (int)frame->width;
  const int h = (int)(frame->len / (frame->width * 2));
  if (w < 20 || h < 20) return out;

  const int y1 = h * 35 / 100;
  const int y2 = h * 72 / 100;
  const int x1 = w * 30 / 100;
  const int x2 = w * 70 / 100;

  uint16_t* px = (uint16_t*)frame->buf;
  int redN = 0;
  int greenN = 0;

  for (int y = y1; y <= y2; y += 2) {
    for (int x = x1; x <= x2; x += 2) {
      uint8_t r, g, b;
      rgb565ToRgb(px[y * w + x], &r, &g, &b);
      if (isRed(r, g, b)) redN++;
      else if (isGreen(r, g, b)) greenN++;
    }
  }

  if (redN > 40 && redN > greenN) out.redCenter = true;
  if (greenN > 30 && greenN > redN) out.greenCenter = true;
  return out;
}

void detectUnoSerialBegin(void) {
  Serial2.begin(115200, SERIAL_8N1, UNO_RX_PIN, UNO_TX_PIN);
}

void detectSendToUno(const DetectResult* d) {
  static char last[12] = "";
  static unsigned long lastMs = 0;
  char line[12];
  snprintf(line, sizeof(line), "R%d;G%d", d->redCenter ? 1 : 0, d->greenCenter ? 1 : 0);

  unsigned long now = millis();
  if (strcmp(line, last) == 0 && (now - lastMs) < 120) return;

  strncpy(last, line, sizeof(last) - 1);
  last[sizeof(last) - 1] = '\0';
  lastMs = now;
  Serial2.println(line);
}
