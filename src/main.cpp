#include <Arduino.h>

#include "ESP32MotorControl.h"
#include "Sensorultra.h"

#define ESQ 1
#define DIR 0

#define LEFT 34
#define CENTER 35
#define RIGHT 32

#define SENSOR_COR_S0 19
#define SENSOR_COR_S1 21
#define SENSOR_COR_S2 4
#define SENSOR_COR_S3 16
#define SENSOR_COR_OUT 5

#define RED 1 
#define GREEN 2
#define BLUE 

#define PIN_RED    22 
#define PIN_GREEN  23 
#define PIN_BLUE   13 

#define echoPin 14 
#define trigPin 12

ESP32MotorControl MotorControl = ESP32MotorControl();

long duration{}; 
int distance{};
int data{};
int red, green, blue;

void setup() 
{
  Serial.begin(115200); 
  MotorControl.attachMotors(17, 18, 26, 27);

  //Light sensor
  pinMode(LEFT, INPUT);
  pinMode(CENTER, INPUT);
  pinMode(RIGHT, INPUT); 

  pinMode(SENSOR_COR_OUT, INPUT);
  pinMode(SENSOR_COR_S2, OUTPUT);
  pinMode(SENSOR_COR_S3, OUTPUT);
  pinMode(SENSOR_COR_S0, OUTPUT);
  pinMode(SENSOR_COR_S1, OUTPUT); 

  digitalWrite(SENSOR_COR_S0,HIGH);
  digitalWrite(SENSOR_COR_S1,HIGH);
  
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);

  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
}
void loop() 
{  
  digitalWrite(SENSOR_COR_S2,LOW);        
  digitalWrite(SENSOR_COR_S3,LOW);  
  red = pulseIn(SENSOR_COR_OUT,LOW);
  delay(20);

  digitalWrite(SENSOR_COR_S2,HIGH);        
  digitalWrite(SENSOR_COR_S3,HIGH);
  green = pulseIn(SENSOR_COR_OUT,LOW);
  delay(20);

  digitalWrite(SENSOR_COR_S2,LOW);        
  digitalWrite(SENSOR_COR_S3,HIGH);
  blue = pulseIn(SENSOR_COR_OUT,LOW);
  delay(20);

  // Sensor de distancia ultrasónico 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Velocidade da onda de som dividida por 2, (ida e volta)

  // Sensor de luz
  if (red < blue && red < green && red >= 3 && red <= 15)
  {
    analogWrite(PIN_RED,255);
    analogWrite(PIN_GREEN,0);
    analogWrite(PIN_BLUE,0);
  }
  else if (green < blue && green < red && green >= 3 && green <= 15)
  {
    analogWrite(PIN_RED,0);
    analogWrite(PIN_GREEN,255);
    analogWrite(PIN_BLUE,0);
  }
  else if (blue < red && blue < green && blue >= 3 && blue <= 15)
  {
    analogWrite(PIN_RED,0);
    analogWrite(PIN_GREEN,0);
    analogWrite(PIN_BLUE,255);
  }
  else if (blue <= 5 && green <= 5 && red <= 5)
  {
    analogWrite(PIN_RED,0);
    analogWrite(PIN_GREEN,0);
    analogWrite(PIN_BLUE,0);
  }
  else if (blue >= 20 && green >= 20 && red >= 20)
  { 
    analogWrite(PIN_RED,0);
    analogWrite(PIN_GREEN,0);
    analogWrite(PIN_BLUE,0);
  }

  // Segue linha
  if (digitalRead(LEFT) == 0 && digitalRead(RIGHT) == 0 && digitalRead(CENTER) == 1)
  {
    MotorControl.motorForward(DIR, 50);
    MotorControl.motorForward(ESQ, 50);
  }
  else if (digitalRead(LEFT) == 1 && digitalRead(RIGHT) == 0)
  {
    MotorControl.motorForward(DIR, 65);
    MotorControl.motorForward(ESQ, 10);
  }
  else if (digitalRead(RIGHT) == 1 && digitalRead(LEFT) == 0)
  {
    MotorControl.motorForward(DIR, 10);
    MotorControl.motorForward(ESQ, 65);
  }
  // Se o robo se encontrar no branco e estiver dentro do tunel então guia-se com o sensor de distancia
  else if (digitalRead(LEFT) == 0 && digitalRead(RIGHT) == 0 && digitalRead(CENTER) == 0 
                                && (distance <= 10 || (distance > 10 && distance < 20)))
  {
    Serial.println("No tunel"); 
    if (distance < 10)
    {
      MotorControl.motorForward(ESQ, 10);
      MotorControl.motorForward(DIR, 40); 
    }
    else if (distance > 10 && distance < 20)
    {
      MotorControl.motorForward(ESQ, 40);
      MotorControl.motorForward(DIR, 10); 
    }
    else
    {
      MotorControl.motorForward(ESQ, 40); 
      MotorControl.motorForward(DIR, 40); 

    }
  }
}
