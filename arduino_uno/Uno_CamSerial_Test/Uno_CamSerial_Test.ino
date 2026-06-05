/*
 * Prueba recepcion ESP32-CAM -> Arduino UNO
 *
 * Formato de linea (115200 baud):
 *   R[IZQ_CERCA:52,CENTRO:80];G[DER_CERCA:99]
 *
 * Conexion recomendada (ver comentarios abajo):
 *   - Modo A: ESP32 GPIO14 (TX2) -> UNO pin 10 + SoftwareSerial (puedes usar monitor USB)
 *   - Modo B: ESP32 GPIO14 -> UNO pin 0 (RX) con divisor, sin USB al UNO
 */

#include <SoftwareSerial.h>

#define USE_SOFT_SERIAL  1
#define CAM_RX_PIN       10
#define CAM_BAUD         115200

#if USE_SOFT_SERIAL
SoftwareSerial camSerial(CAM_RX_PIN, 11);
#define CAM_PORT camSerial
#else
#define CAM_PORT Serial
#endif

char lineBuf[180];
uint8_t lineLen = 0;

void printParsed(const char* raw) {
  Serial.print(F("<< "));
  Serial.println(raw);

  int r0 = -1, r1 = -1, g0 = -1, g1 = -1;
  for (int i = 0; raw[i]; i++) {
    if (raw[i] == 'R' && raw[i + 1] == '[') r0 = i + 2;
    if (raw[i] == ']' && raw[i + 1] == ';' && raw[i + 2] == 'G' && raw[i + 3] == '[') {
      r1 = i;
      g0 = i + 4;
    }
    if (g0 > 0 && raw[i] == ']' && raw[i + 1] == 0) g1 = i;
  }

  Serial.println(F("--- Cubos rojos ---"));
  if (r0 < 0 || r1 <= r0) {
    Serial.println(F("  (ninguno)"));
  } else {
    lineBuf[0] = 0;
    for (int i = r0; i < r1 && i < (int)sizeof(lineBuf) - 1; i++) {
      lineBuf[i - r0] = raw[i];
      lineBuf[i - r0 + 1] = 0;
    }
    char* tok = strtok(lineBuf, ",");
    while (tok) {
      char* colon = strchr(tok, ':');
      if (colon) {
        *colon = 0;
        Serial.print(F("  zona="));
        Serial.print(tok);
        Serial.print(F("  X="));
        Serial.println(colon + 1);
      }
      tok = strtok(NULL, ",");
    }
  }

  Serial.println(F("--- Cubos verdes ---"));
  if (g0 < 0 || g1 <= g0) {
    Serial.println(F("  (ninguno)"));
  } else {
    lineBuf[0] = 0;
    for (int i = g0; i < g1 && i < (int)sizeof(lineBuf) - 1; i++) {
      lineBuf[i - g0] = raw[i];
      lineBuf[i - g0 + 1] = 0;
    }
    char* tok = strtok(lineBuf, ",");
    while (tok) {
      char* colon = strchr(tok, ':');
      if (colon) {
        *colon = 0;
        Serial.print(F("  zona="));
        Serial.print(tok);
        Serial.print(F("  X="));
        Serial.println(colon + 1);
      }
      tok = strtok(NULL, ",");
    }
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
#if USE_SOFT_SERIAL
  camSerial.begin(CAM_BAUD);
  Serial.println(F("=== UNO test ESP32-CAM (SoftwareSerial pin 10) ==="));
  Serial.println(F("ESP32 GPIO14 (TX2) -> UNO pin 10, GND comun"));
#else
  Serial.println(F("=== UNO test ESP32-CAM (Serial pin 0) ==="));
  Serial.println(F("Desconecta USB o usa otro adaptador para programar"));
#endif
  Serial.println(F("Esperando lineas R[...];G[...] ..."));
  lineLen = 0;
}

void loop() {
  while (CAM_PORT.available()) {
    char c = (char)CAM_PORT.read();
    if (c == '\r') continue;
    if (c == '\n') {
      if (lineLen > 0) {
        lineBuf[lineLen] = 0;
        if (lineBuf[0] == 'R' && strchr(lineBuf, '[')) {
          printParsed(lineBuf);
        }
        lineLen = 0;
      }
      continue;
    }
    if (lineLen < sizeof(lineBuf) - 1) {
      lineBuf[lineLen++] = c;
    }
  }
}
