#include <Servo.h>
#define Lpwm_pin 5
#define Rpwm_pin 6
int pinLB = 2, pinLF = 4, pinRB = 7, pinRF = 8;
Servo myservo;
const int SERVO_PIN = 10;
const int CENTRO = 96;
const int IZQ = CENTRO - 45;
const int DER = CENTRO + 45;
const int TRIG_PIN = A1;
const int ECHO_PIN = A0;
int DIST_INICIO_LENTO = 30;
int DIST_MIN_ACERCAMIENTO = 5;
int DIST_PARADA_SEGURIDAD = 3;
int VELOCIDAD_MAX = 200;
int VELOCIDAD_MIN = 90;
const int S0 = 11, S1 = 9, S2 = 12, S3 = 13;
const int colorOut = A2;
const int LED_PIN = 3;
float DOMINANCIA_VERDE = 0.06;
float DOMINANCIA_ROJO  = 0.15;
float NEGRO_LUZ_MIN = 3000.0;
int RETROCESO_MANIOBRA = 400;
int GIRO_SOSTENIDO      = 500;
void setup() {
  pinMode(pinLB, OUTPUT); pinMode(pinLF, OUTPUT);
  pinMode(pinRB, OUTPUT); pinMode(pinRF, OUTPUT);
  pinMode(Lpwm_pin, OUTPUT); pinMode(Rpwm_pin, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT);
  pinMode(S0, OUTPUT); pinMode(S1, OUTPUT); pinMode(S2, OUTPUT); pinMode(S3, OUTPUT);
  pinMode(colorOut, INPUT);
  digitalWrite(S0, HIGH); digitalWrite(S1, HIGH);
  pinMode(LED_PIN, OUTPUT); digitalWrite(LED_PIN, HIGH);
  myservo.attach(SERVO_PIN);
  myservo.write(CENTRO);
  Serial.begin(9600);
}
float checkDistance() {
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float d = pulseIn(ECHO_PIN, HIGH) / 58.00;
  delay(10);
  return d;
}
long leerPeriodo(bool s2, bool s3) {
  digitalWrite(S2, s2); digitalWrite(S3, s3);
  delay(10);
  long p = pulseIn(colorOut, LOW, 30000);
  if (p == 0) p = 30000;
  return p;
}
int detectarColor() {
  long pRojo  = leerPeriodo(LOW, LOW);
  long pVerde = leerPeriodo(HIGH, HIGH);
  long pAzul  = leerPeriodo(LOW, HIGH);
  float iRojo  = 1000000.0 / pRojo;
  float iVerde = 1000000.0 / pVerde;
  float iAzul  = 1000000.0 / pAzul;
  float total  = iRojo + iVerde + iAzul;
  float fRojo  = iRojo  / total;
  float fVerde = iVerde / total;
  bool hayDominante = (fVerde > 0.333 + DOMINANCIA_VERDE) || (fRojo > 0.333 + DOMINANCIA_ROJO);
  if (total < NEGRO_LUZ_MIN && !hayDominante) return 3;
  if (fVerde > 0.333 + DOMINANCIA_VERDE) return 2;
  if (fRojo  > 0.333 + DOMINANCIA_ROJO)  return 1;
  return 0;
}
void Set_Speed(unsigned char pwm) { analogWrite(Lpwm_pin, pwm); analogWrite(Rpwm_pin, pwm); }
void advance() { digitalWrite(pinLB,HIGH); digitalWrite(pinLF,LOW); digitalWrite(pinRB,HIGH); digitalWrite(pinRF,LOW); }
void back()    { digitalWrite(pinLB,LOW);  digitalWrite(pinLF,HIGH); digitalWrite(pinRB,LOW);  digitalWrite(pinRF,HIGH); }
void detener() { Set_Speed(0); }
void reaccionarConEnderezado(int lado, int ladoContrario) {
  detener(); delay(50);
  myservo.write(CENTRO);
  back(); Set_Speed(180);
  delay(RETROCESO_MANIOBRA);
  Set_Speed(0);
  myservo.write(lado);
  advance(); Set_Speed(180);
  delay(GIRO_SOSTENIDO);
  myservo.write(ladoContrario);
  delay(GIRO_SOSTENIDO);
  myservo.write(CENTRO);
  Set_Speed(0);
}
void reaccionarNegro() {
  detener(); delay(50);
  myservo.write(CENTRO);
  back(); Set_Speed(180);
  delay(RETROCESO_MANIOBRA);
  Set_Speed(0);
  myservo.write(IZQ);
  advance(); Set_Speed(180);
  delay(GIRO_SOSTENIDO);
  myservo.write(CENTRO);
}
void loop() {
  float distancia = checkDistance();
  int color = detectarColor();
  if (color == 2) {
    reaccionarConEnderezado(DER, IZQ);
    return;
  } else if (color == 1) {
    reaccionarConEnderezado(IZQ, DER);
    return;
  } else if (color == 3) {
    reaccionarNegro();
    return;
  }
  if (distancia <= DIST_PARADA_SEGURIDAD) {
    detener();
  } else if (distancia <= DIST_INICIO_LENTO) {
    int velocidad = map(distancia, DIST_MIN_ACERCAMIENTO, DIST_INICIO_LENTO, VELOCIDAD_MIN, VELOCIDAD_MAX);
    velocidad = constrain(velocidad, VELOCIDAD_MIN, VELOCIDAD_MAX);
    advance();
    Set_Speed(velocidad);
  } else {
    advance();
    Set_Speed(VELOCIDAD_MAX);
  }
}
