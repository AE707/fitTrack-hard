#include "GPS.h"

GPSModule::GPSModule(int rxPin, int txPin) : gpsSerial(rxPin, txPin) {
    // Constructor
}

bool GPSModule::setupGPS() {
    gpsSerial.begin(9600);
    return true;  // Add error handling if needed
}

bool GPSModule::getData(float& latitude, float& longitude, float& altitude, unsigned long& time, float& speed) {
    while (gpsSerial.available() > 0) {
        if (gps.encode(gpsSerial.read())) {
            if (gps.location.isValid()) {
                latitude = gps.location.lat();
                longitude = gps.location.lng();
                altitude = gps.altitude.meters();
            }
            if (gps.date.isValid() && gps.time.isValid()) {
                time = gps.time.value();
            }
            if (gps.speed.isValid()) {
                speed = gps.speed.kmph();
            }
            return true;
        }
    }
    return false;
}
