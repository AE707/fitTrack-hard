#ifndef MAX4466_H
#define MAX4466_H

#include <Arduino.h>

class MAX4466Module {
public:
    MAX4466Module(int analogPin);
    bool setupMAX4466();
    bool getAudioData(float& audioSignal, float& soundLevel, float& frequencyContent);
    
private:
    int pin;
};

#endif
