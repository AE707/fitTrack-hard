#include <IOXhop_FirebaseESP32.h>// Include the Firebase library for ESP8266
#include <Adafruit_MLX90614.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <WiFi.h>
#include <Wire.h>
#include <string.h>
#include <Adafruit_ADS1X15.h>
#include "max30102.h"
#include "GPS.h"


const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
float ax=0, ay=0, az=0, gx=0, gy=0, gz=0;
bool fall = false; //stores if a fall has occurred
bool trigger1=false; //stores if first trigger (lower threshold) has occurred
bool trigger2=false; //stores if second trigger (upper threshold) has occurred
bool trigger3=false; //stores if third trigger (orientation change) has occurred
byte trigger1count=0; //stores the counts past since trigger 1 was set true
byte trigger2count=0; //stores the counts past since trigger 2 was set true
byte trigger3count=0; //stores the counts past since trigger 3 was set true
int angleChange=0;

// Replace with your network credentials
const char* ssid = "AE7";
const char* password = "1234567890";

// Replace with your Firebase project's URL
#define FIREBASE_HOST "https://fittrack-c4ca4-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "QGVXXSVMFaPAwKon7hsEFlWrJdXXMEm0lLiG1jZl"

#define IR_PIN 34
#define R_PIN 35
int rxPin= 16;
int txPin = 17;
MAX30102 max30102(IR_PIN, R_PIN);
GPSModule gpsModule(16, 17);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_ADS1115 ads;

// Define audio buffer size
const int bufferSize = 1024;
int16_t audioBuffer[bufferSize];

void setup() {
    Serial.begin(9600);

    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Initialize Firebase
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

     Wire.begin();
     Wire.beginTransmission(MPU_addr);
     Wire.write(0x6B);  // PWR_MGMT_1 register
     Wire.write(0);     // set to zero (wakes up the MPU-6050)
     Wire.endTransmission(true);

    max30102.begin();
    if (gpsModule.setupGPS()) {
        Serial.println("GPS module initialized.");
    } else {
        Serial.println("GPS module failed to initialize.");
    }

    mlx.begin();
    ads.begin();
}

void mpu_readData() {
 ax = (AcX-2050)/16384.00;
 ay = (AcY-77)/16384.00;
 az = (AcZ-1947)/16384.00;
 gx = (GyX+270)/131.07;
 gy = (GyY-351)/131.07;
 gz = (GyZ+136)/131.07;
 // calculating Amplitute vactor for 3 axis
 float raw_amplitude = pow(pow(ax,2)+pow(ay,2)+pow(az,2),0.5);
 int amplitude = raw_amplitude * 10;  // Mulitiplied by 10 bcz values are between 0 to 1
 Serial.println(amplitude);
 if (amplitude<=2 && trigger2==false){ //if AM breaks lower threshold (0.4g)
   trigger1=true;
   Serial.println("TRIGGER 1 ACTIVATED");
   }
 if (trigger1==true){
   trigger1count++;
   if (amplitude>=12){ //if AM breaks upper threshold (3g)
     trigger2=true;
     Serial.println("TRIGGER 2 ACTIVATED");
     trigger1=false; trigger1count=0;
     }
 }
 if (trigger2==true){
   trigger2count++;
   angleChange = pow(pow(gx,2)+pow(gy,2)+pow(gz,2),0.5); 
   Serial.println(angleChange);
   if (angleChange>=30 && angleChange<=400){ //if orientation changes by between 80-100 degrees
     trigger3=true; trigger2=false; trigger2count=0;
     Serial.println(angleChange);
     Serial.println("TRIGGER 3 ACTIVATED");
       }
   }
 if (trigger3==true){
    trigger3count++;
    if (trigger3count>=10){ 
       angleChange = pow(pow(gx,2)+pow(gy,2)+pow(gz,2),0.5);
       Serial.println(angleChange); 
       if ((angleChange>=0) && (angleChange<=10)){ //if orientation changes remains between 0-10 degrees
           fall=true; trigger3=false; trigger3count=0;
           Serial.println(angleChange);
             }
       else{ //user regained normal orientation
          trigger3=false; trigger3count=0;
          Serial.println("TRIGGER 3 DEACTIVATED");
       }
     }
  }
 if (fall==true){ //in event of a fall detection
   Serial.println("FALL DETECTED");
    
   fall=false;
   }
 if (trigger2count>=6){ //allow 0.5s for orientation change
   trigger2=false; trigger2count=0;
   Serial.println("TRIGGER 2 DECACTIVATED");
   }
 if (trigger1count>=6){ //allow 0.5s for AM to break upper threshold
   trigger1=false; trigger1count=0;
   Serial.println("TRIGGER 1 DECACTIVATED");
   }
  delay(100);
}
void loop() {
    int heartRate = max30102.calculateHeartRate();
    int spo2 = max30102.calculateSpo2();
    float latitude, longitude, altitude, speed;
    unsigned long time;
    float objTemp = mlx.readObjectTempC();
    float ambientTempe = mlx.readAmbientTempC();
    



    if (gpsModule.getData(latitude, longitude, altitude, time, speed)) {
        Serial.print("Heart Rate: ");
        Serial.print(heartRate);
        Serial.print(" bpm, SpO2: ");
        Serial.print(spo2);
        Serial.println(" %");

        Serial.print("Latitude: ");
        Serial.println(latitude, 6);
        Serial.print("Longitude: ");
        Serial.println(longitude, 6);
        Serial.print("Altitude: ");
        Serial.println(altitude);
        Serial.print("Time: ");
        Serial.println(time);
        Serial.print("Speed: ");
        Serial.println(speed);

        Serial.print("Ambient temperature = "); 
        Serial.print(mlx.readAmbientTempC());
        Serial.print("°C");      
        Serial.print("   ");
        Serial.print("Object temperature = "); 
        Serial.print(mlx.readObjectTempC()); 
        Serial.println("°C");
  
        Serial.print("Ambient temperature = ");
        Serial.print(mlx.readAmbientTempF());
        Serial.print("°F");      
        Serial.print("   ");
        Serial.print("Object temperature = "); 
        Serial.print(mlx.readObjectTempF()); 
        Serial.println("°F");
        Serial.print(GyX);
        Serial.println("GyX : ");
        Serial.print(GyY);
        Serial.println("GyY : ");
        Serial.print(GyZ);
        Serial.println("GyZ : ");
        Serial.print(AcX);
        Serial.println("AcX : ");
        Serial.print(AcY);
        Serial.println("ay : ");
        Serial.print(AcZ);
        Serial.println("AcZ : ");        

          for (int i = 0; i < bufferSize; i++) {
    // Read analog audio data from the microphone
    int16_t sample = analogRead(33); // Use appropriate channel

    // Store the sample in the buffer
    audioBuffer[i] = sample;
  }

 // Delay before recording and uploading again
        String gxStr = String(gx);
        String gyStr = String(gy);
        String gzStr = String(gz);
        String axStr = String(ax);
        String ayStr = String(ay);
        String azStr = String(az);


        String audioDataString = convertBufferToString(audioBuffer, bufferSize);
 sendDataToFirebase(heartRate, spo2, latitude, longitude, altitude, time, speed, objTemp, ambientTempe, audioDataString, gxStr, gyStr, gzStr, axStr, ayStr, azStr);
    }

    delay(3000);  // Adjust delay as needed
}
       String convertBufferToString(int16_t *buffer, int bufferSize) {
    String result = "";
   for (int i = 0; i < bufferSize; i++) {
     result += String(buffer[i]) + ",";
   }
    return result;
}

void mpu_read(){
 Wire.beginTransmission(MPU_addr);
 Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
 Wire.endTransmission(false);
 Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
 AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
 AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
 AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
 Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
 GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
 GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
 GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
 }

void sendDataToFirebase(int heartRate, int spo2, float latitude, float longitude, float altitude, unsigned long time, float speed, float objTemp, float ambientTempe, String audioDataString, String gxStr, String gyStr, String gzStr, String axStr, String ayStr, String azStr) {
    Firebase.setFloat("HeartRate", heartRate);
    Firebase.setFloat("TauxD'oxygéne", spo2);
    Firebase.setFloat("Latitude", latitude);
    Firebase.setFloat("Longitude", longitude);
    Firebase.setFloat("Altitude", altitude);
    Firebase.setFloat("Time", time);
    Firebase.setFloat("Speed", speed);
    Firebase.setFloat("ObjectTemperature", objTemp);
    Firebase.setFloat("AmbientTemperature", ambientTempe);
      // Upload audio data to Firebase
    
    Firebase.setString("/audio", audioDataString);
    Firebase.setString("GyX", gxStr);
    Firebase.setString("GyY", gyStr);
    Firebase.setString("GyZ", gzStr);
    Firebase.setString("AcX", axStr);
    Firebase.setString("AcY", ayStr);
    Firebase.setString("AcZ", azStr);
 

    
    
}
