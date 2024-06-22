#ifndef GPS_H
#define GPS_H

#include <TinyGPS++.h>
#include <SoftwareSerial.h>  // For ESP32 SoftwareSerial

class GPSModule {
public:
    GPSModule(int rxPin, int txPin);  // Constructor
    bool setupGPS();
    bool getData(float& latitude, float& longitude, float& altitude, unsigned long& time, float& speed);
    
private:
    SoftwareSerial gpsSerial;
    TinyGPSPlus gps;
};

#endif
