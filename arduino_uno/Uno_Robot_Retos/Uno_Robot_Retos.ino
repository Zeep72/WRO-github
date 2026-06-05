/*
 * Robot: 3 vueltas completas a la izquierda, vuelve al inicio y para.
 * Obstaculo verde -> gira izquierda | Obstaculo rojo -> gira derecha
 * Si esta muy cerca -> retrocede y luego gira.
 *
 * ESP32 GPIO14 -> UNO pin10 | UNO pin11 -> ESP32 GPIO15 | GND comun
 */

#include <Servo.h>
#include <SoftwareSerial.h>
#include <math.h>

// Motores
#define LPWM 5
#define RPWM 6
#define LB   2
#define LF   4
#define RB   7
#define RF   8

// Sonar
#define TRIG A1
#define ECHO A0

// Servo y camara
#define SERVO_PIN    9
#define CAM_RX_PIN  10
#define ESP_TX_PIN  11

SoftwareSerial camSerial(CAM_RX_PIN, ESP_TX_PIN);
Servo servo;

// Estados
#define ST_SCAN   0
#define ST_LAP    1
#define ST_TURN   2
#define ST_AVOID  3
#define ST_HOME   4
#define ST_DONE   5

int state = ST_SCAN;
int laps = 0;
int scanDeg = 0;
int avoidDir = 0;  // -1=izq, +1=der

float posX = 0, posY = 0, heading = 0;
float sonarBg = 80.0f;

bool camRed = false;
bool camGreen = false;
unsigned long lastCamMs = 0;
unsigned long phaseMs = 0;
unsigned long avoidCooldown = 0;

char camBuf[24];
uint8_t camLen = 0;

// --- Motores ---
void stopMotors() {
  digitalWrite(LB, HIGH); digitalWrite(LF, HIGH);
  digitalWrite(RB, HIGH); digitalWrite(RF, HIGH);
  analogWrite(LPWM, 0);
  analogWrite(RPWM, 0);
}

void fwd(int s) {
  digitalWrite(LB, HIGH); digitalWrite(LF, LOW);
  digitalWrite(RB, HIGH); digitalWrite(RF, LOW);
  analogWrite(LPWM, s);
  analogWrite(RPWM, s);
}

void back(int s) {
  digitalWrite(LB, LOW); digitalWrite(LF, HIGH);
  digitalWrite(RB, LOW); digitalWrite(RF, HIGH);
  analogWrite(LPWM, s);
  analogWrite(RPWM, s);
}

void left(int s) {
  digitalWrite(LB, LOW); digitalWrite(LF, HIGH);
  digitalWrite(RB, HIGH); digitalWrite(RF, LOW);
  analogWrite(LPWM, s);
  analogWrite(RPWM, s);
}

void right(int s) {
  digitalWrite(LB, HIGH); digitalWrite(LF, LOW);
  digitalWrite(RB, LOW); digitalWrite(RF, HIGH);
  analogWrite(LPWM, s);
  analogWrite(RPWM, s);
}

// --- Sonar ---
float sonarCm() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  unsigned long us = pulseIn(ECHO, HIGH, 25000);
  if (us == 0) return 999.0f;
  return us / 58.0f;
}

void updateBg(float d) {
  if (d < 5.0f || d > 300.0f) return;
  sonarBg = sonarBg * 0.85f + d * 0.15f;
}

// --- Odometria ---
void odom(float dist, float dAng) {
  heading += dAng;
  while (heading >= 360.0f) heading -= 360.0f;
  while (heading < 0.0f) heading += 360.0f;
  float r = heading * (float)M_PI / 180.0f;
  posX += dist * sinf(r);
  posY += dist * cosf(r);
}

// --- Camara ESP32: linea "R1;G0" ---
void readCam() {
  while (camSerial.available()) {
    char c = (char)camSerial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      if (camLen > 0) {
        camBuf[camLen] = 0;
        int r = 0, g = 0;
        if (sscanf(camBuf, "R%d;G%d", &r, &g) == 2) {
          camRed = (r == 1);
          camGreen = (g == 1);
          lastCamMs = millis();
        }
        camLen = 0;
      }
      continue;
    }
    if (camLen < sizeof(camBuf) - 1) camBuf[camLen++] = c;
  }
}

bool camOk() {
  return (millis() - lastCamMs) < 500;
}

// --- Cambio de estado ---
void goState(int s) {
  state = s;
  phaseMs = millis();
  stopMotors();
}

// --- Radar inicial: gira 360 y mide distancias ---
void doScan() {
  servo.write(148);
  unsigned long t = millis() - phaseMs;

  if (t < 70) {
    left(150);
    return;
  }
  stopMotors();

  float d = sonarCm();
  Serial.print(F("^R:"));
  Serial.print(scanDeg);
  Serial.print(F(","));
  Serial.println((int)d);

  heading = (float)scanDeg;
  scanDeg += 15;

  if (scanDeg >= 360) {
    posX = posY = heading = 0;
    laps = 0;
    scanDeg = 0;
    sonarBg = sonarCm();
    if (sonarBg > 300.0f) sonarBg = 80.0f;
    goState(ST_LAP);
    return;
  }
  phaseMs = millis();
}

// --- Avance entre vueltas ---
void doLap() {
  servo.write(148);
  float d = sonarCm();
  updateBg(d);

  // Cada 250 ms manda distancia al monitor (radar en vivo)
  static unsigned long lastSonarTx = 0;
  if (millis() - lastSonarTx > 250) {
    lastSonarTx = millis();
    Serial.print(F("^S:"));
    Serial.print((int)d);
    Serial.print(F(","));
    Serial.println((int)sonarBg);
  }

  // Evitar obstaculos (con cooldown de 2 s para no spamear)
  if (millis() > avoidCooldown) {
    bool drop = (sonarBg - d) > 12.0f && d < 55.0f;
    bool bloque = camOk() && (camRed || camGreen);
    bool cerca = d < 20.0f;

    if ((drop || bloque) && (cerca || bloque)) {
      avoidDir = camGreen ? -1 : 1;  // verde=izq, rojo=der (default der si solo sonar)
      if (camOk() && camRed) avoidDir = 1;
      if (camOk() && camGreen) avoidDir = -1;
      goState(ST_AVOID);
      return;
    }
  }

  // Tramo de avance fijo, luego vuelta completa
  if (millis() - phaseMs > 2200) {
    goState(ST_TURN);
    return;
  }

  fwd(180);
  odom(0.8f, 0);
}

// --- Vuelta completa a la izquierda ---
void doTurn() {
  servo.write(165);
  if (millis() - phaseMs > 2700) {
    stopMotors();
    odom(0, 360.0f);
    laps++;
    Serial.print(F("^P:lap "));
    Serial.println(laps);

    if (laps >= 3) {
      goState(ST_HOME);
    } else {
      goState(ST_LAP);
    }
    return;
  }
  left(155);
}

// --- Esquivar: retrocede si cerca, gira, sigue ---
void doAvoid() {
  unsigned long t = millis() - phaseMs;
  float d = sonarCm();

  if (t < 500) {
    if (d < 18.0f) back(140);
    return;
  }

  if (t < 1150) {
    if (avoidDir < 0) left(155);
    else right(155);
    return;
  }

  if (t < 1750) {
    fwd(160);
    odom(0.6f, 0);
    return;
  }

  avoidCooldown = millis() + 2000;
  camRed = false;
  camGreen = false;
  goState(ST_LAP);
}

// --- Regreso al punto de inicio ---
void doHome() {
  servo.write(148);
  float dist = sqrtf(posX * posX + posY * posY);

  if (dist < 6.0f) {
    // Alinear al heading 0
    float err = -heading;
    if (err > 180) err -= 360;
    if (err < -180) err += 360;
    if (fabsf(err) > 10.0f) {
      if (err > 0) left(140);
      else right(140);
      odom(0, err > 0 ? 1.0f : -1.0f);
      return;
    }
    goState(ST_DONE);
    return;
  }

  float target = atan2f(-posX, -posY) * 180.0f / (float)M_PI;
  if (target < 0) target += 360.0f;
  float err = target - heading;
  if (err > 180) err -= 360;
  if (err < -180) err += 360;

  if (fabsf(err) > 12.0f) {
    if (err > 0) left(150);
    else right(150);
    odom(0, err > 0 ? 1.5f : -1.5f);
  } else {
    fwd(170);
    odom(dist > 20.0f ? 20.0f : dist, 0);
    delay(150);
    stopMotors();
  }
}

void setup() {
  Serial.begin(115200);
  camSerial.begin(115200);

  pinMode(LB, OUTPUT); pinMode(LF, OUTPUT);
  pinMode(RB, OUTPUT); pinMode(RF, OUTPUT);
  pinMode(LPWM, OUTPUT); pinMode(RPWM, OUTPUT);
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);

  servo.attach(SERVO_PIN);
  servo.write(148);
  delay(400);

  Serial.println(F("Robot: 3 vueltas izq + regreso"));
  goState(ST_SCAN);
}

void loop() {
  readCam();

  switch (state) {
    case ST_SCAN:  doScan();  break;
    case ST_LAP:   doLap();   break;
    case ST_TURN:  doTurn();  break;
    case ST_AVOID: doAvoid(); break;
    case ST_HOME:  doHome();  break;
    case ST_DONE:
      stopMotors();
      servo.write(148);
      delay(200);
      break;
  }

  delay(15);
}
