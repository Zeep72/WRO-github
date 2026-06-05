/*
 * Robot base + lectura ESP32-CAM (mismo pinout que tu sketch)
 *
 * Cambios respecto a tu codigo:
 *   - Monitor debug USB: 115200 (o deja 9600 si prefieres)
 *   - Camara en SoftwareSerial pin 10 @ 115200 (GPIO14 del ESP32)
 *   - Parser R[...];G[...] para decidir giro del servo
 *
 * Logica simple de prueba:
 *   - Cubo rojo en IZQ_*  -> SERVO_LEFT
 *   - Cubo rojo en DER_*  -> SERVO_RIGHT
 *   - Cubo verde CENTRO   -> SERVO_CENTER
 *   - Sin cubos           -> SERVO_CENTER + avance (como tu loop con sonar)
 */

#include <Servo.h>
#include <SoftwareSerial.h>

#define LPWM 5
#define RPWM 6
#define LB 2
#define LF 4
#define RB 7
#define RF 8
#define TRIG A1
#define ECHO A0
#define SERVO_PIN 9

#define SERVO_CENTER  148
#define SERVO_LEFT    165
#define SERVO_RIGHT   135
#define SPEED         200
#define SPEED_SLOW    (SPEED / 1.2)
#define DIST_SLOW     90
#define SERVO_REFRESH_MS  20

#define CAM_RX_PIN    10
#define CAM_BAUD      115200

#define DEBUG true

#if DEBUG
#define DBG_BEGIN(baud)  Serial.begin(baud)
#define DBGLN(msg)       Serial.println(msg)
#else
#define DBG_BEGIN(baud)
#define DBGLN(msg)
#endif

SoftwareSerial camSerial(CAM_RX_PIN, 11);

Servo servo;
String lastState = "";
int lastServo = -1;
int currentServoAngle = SERVO_CENTER;
unsigned long lastServoRefresh = 0;

char camLine[180];
uint8_t camLineLen = 0;
bool hasRedLeft = false;
bool hasRedRight = false;
bool hasGreenCenter = false;
unsigned long lastCamMs = 0;

float distance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH) / 58.0f;
}

void speed(int v) {
  analogWrite(LPWM, v);
  analogWrite(RPWM, v);
}

void forward() {
  digitalWrite(LB, HIGH); digitalWrite(LF, LOW);
  digitalWrite(RB, HIGH); digitalWrite(RF, LOW);
}

void stopMotors() {
  digitalWrite(LB, HIGH); digitalWrite(LF, HIGH);
  digitalWrite(RB, HIGH); digitalWrite(RF, HIGH);
}

void setServo(int angle) {
  currentServoAngle = angle;
  servo.write(angle);
  lastServoRefresh = millis();
}

void keepServoAlive() {
  if (millis() - lastServoRefresh >= SERVO_REFRESH_MS) {
    servo.write(currentServoAngle);
    lastServoRefresh = millis();
  }
}

void parseCamLine(const char* line) {
  hasRedLeft = false;
  hasRedRight = false;
  hasGreenCenter = false;
  lastCamMs = millis();

  const char* p = line;
  while (*p) {
    if (*p == 'I' && strncmp(p, "IZQ_", 4) == 0) {
      const char* back = p;
      while (back > line && *(back - 1) != 'R' && *(back - 1) != ',') back--;
      if (back > line && *(back - 1) == 'R') hasRedLeft = true;
    }
    if (*p == 'D' && strncmp(p, "DER_", 4) == 0) {
      const char* back = p;
      while (back > line && *(back - 1) != 'R' && *(back - 1) != ',') back--;
      if (back > line && *(back - 1) == 'R') hasRedRight = true;
    }
    if (*p == 'C' && strncmp(p, "CENTRO", 6) == 0) {
      const char* back = p;
      while (back > line && *(back - 1) != 'G' && *(back - 1) != ',') back--;
      if (back > line && *(back - 1) == 'G') hasGreenCenter = true;
    }
    p++;
  }
}

void readCamSerial() {
  while (camSerial.available()) {
    char c = (char)camSerial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      if (camLineLen > 0) {
        camLine[camLineLen] = 0;
        if (camLine[0] == 'R') parseCamLine(camLine);
        camLineLen = 0;
      }
      continue;
    }
    if (camLineLen < sizeof(camLine) - 1) camLine[camLineLen++] = c;
  }
}

void setup() {
  DBG_BEGIN(115200);
  DBGLN(F("\n=== Robot + ESP32-CAM ==="));

  pinMode(LB, OUTPUT); pinMode(LF, OUTPUT);
  pinMode(RB, OUTPUT); pinMode(RF, OUTPUT);
  pinMode(LPWM, OUTPUT); pinMode(RPWM, OUTPUT);
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);

  camSerial.begin(CAM_BAUD);
  servo.attach(SERVO_PIN);
  setServo(SERVO_CENTER);
  delay(500);
  DBGLN(F("Listo. Camara en pin 10."));
}

void loop() {
  keepServoAlive();
  readCamSerial();

  float d = distance();
  int spd = SPEED;
  int servoAngle = SERVO_CENTER;
  const char* state = "AVANZANDO";

  if (d > 0 && d < DIST_SLOW) {
    spd = SPEED_SLOW;
    state = "OBSTACULO";
  }

  if (millis() - lastCamMs < 500) {
    if (hasRedLeft) {
      servoAngle = SERVO_LEFT;
      state = "CAM_ROJO_IZQ";
    } else if (hasRedRight) {
      servoAngle = SERVO_RIGHT;
      state = "CAM_ROJO_DER";
    } else if (hasGreenCenter) {
      servoAngle = SERVO_CENTER;
      state = "CAM_VERDE_CENTRO";
    }
  }

  setServo(servoAngle);
  forward();
  speed(spd);

  if (state != lastState) {
    lastState = state;
    Serial.print(F("[Estado] "));
    Serial.println(state);
  }

  delay(80);
}
