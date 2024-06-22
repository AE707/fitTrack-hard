#ifndef MAX30102_H
#define MAX30102_H

class MAX30102 {
public:
  MAX30102(int irPin, int rPin);
  void begin();
  int calculateHeartRate();
  int calculateSpo2();
private:
  int irPin;
  int rPin;
};

#endif
