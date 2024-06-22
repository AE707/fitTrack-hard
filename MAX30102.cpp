#include "max30102.h"
#include "Arduino.h"

MAX30102::MAX30102(int irPin, int rPin) {
  this->irPin = irPin;
  this->rPin = rPin;
}

void MAX30102::begin() {
  pinMode(irPin, INPUT);
  pinMode(rPin, INPUT);
}

int MAX30102::calculateHeartRate() {
  int irValue = analogRead(irPin);
  int rValue = analogRead(rPin);

  float ratio = (float)irValue / (float)rValue;
  int heartRate = static_cast<int>(60 / ratio);
  
  return heartRate;
}

int MAX30102::calculateSpo2() {
  int irValue = analogRead(irPin);
  int rValue = analogRead(rPin);

  float ratio = (float)irValue / (float)rValue;
  int spo2 = static_cast<int>((100 - 5 * ratio) - 2);
  
  return spo2;
}
