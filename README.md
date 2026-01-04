# fitTrack - IoT Health Monitoring Smartwatch

<div align="center">

**ESP32-Based Wearable Health Tracker with Advanced Fall Detection**

[![Arduino](https://img.shields.io/badge/Arduino-Compatible-00979D.svg)](https://www.arduino.cc/)
[![ESP32](https://img.shields.io/badge/ESP32-Powered-blue.svg)](https://www.espressif.com/)
[![Firebase](https://img.shields.io/badge/Firebase-Cloud-orange.svg)](https://firebase.google.com/)
[![Status](https://img.shields.io/badge/Status-Prototype-yellow.svg)](#)

*An early-stage prototype exploring the intersection of IoT, health monitoring, and intelligent wearables*

[Features](#-features) •
[Hardware](#-hardware-components) •
[Architecture](#-system-architecture) •
[Future Vision](#-future-roadmap)

</div>

---

## 📢 Project Status

> **Note**: This is an **early-stage prototype** (circa 2024) created as a proof-of-concept for an IoT health monitoring system. While the core functionality is demonstrated, the project represents an initial exploration phase and is not production-ready.

**Current Stage**: Prototyping & Concept Validation  
**Last Updated**: June 2024  
**Development Status**: On hold - awaiting next iteration

---

## 🎯 Project Vision

fitTrack aims to create an **affordable, open-source smartwatch alternative** that focuses on essential health monitoring features often found in premium devices:

- Real-time heart rate and blood oxygen monitoring
- Intelligent fall detection for elderly care
- GPS-based activity tracking
- Cloud-connected health dashboard
- Ambient sound recording for context awareness

### Why This Project Matters

Commercial smartwatches often cost $200-$500+. This project explores whether DIY hardware can deliver core functionality at a fraction of the cost, while remaining open-source and hackable.

---

## ✨ Features

### Implemented in Prototype

#### 💓 Health Monitoring
- **Heart Rate Tracking** - MAX30102 pulse oximeter sensor
- **SpO2 Measurement** - Blood oxygen saturation monitoring
- **Body Temperature** - Non-contact IR thermometer (MLX90614)
- **Ambient Temperature** - Environmental monitoring

#### 🚨 Advanced Fall Detection
- **3-Stage Algorithm**:
  1. Sudden deceleration detection (< 0.4g)
  2. Impact recognition (> 3g)
  3. Orientation change analysis (80-100° rotation)
- Temporal validation windows to prevent false positives
- 6-axis IMU fusion (MPU-6050)

#### 📍 Location & Activity
- **GPS Tracking** - Real-time latitude/longitude
- **Altitude Monitoring** - For hiking/elevation tracking
- **Speed Calculation** - Running/cycling metrics
- **Motion Tracking** - 3-axis accelerometer + 3-axis gyroscope

#### 🎙️ Audio Capture
- **Ambient Sound Recording** - MAX4466 microphone module
- 1024-sample audio buffering
- Potential for voice commands (not yet implemented)

#### ☁️ Cloud Integration
- **Firebase Realtime Database** - Live data streaming
- **WiFi Connectivity** - ESP32 built-in
- Real-time health dashboard (backend only)

---

## 🛠️ Hardware Components

| Component | Model | Purpose | Interface |
|-----------|-------|---------|----------|
| **Microcontroller** | ESP32 | Main controller with WiFi | - |
| **Heart Rate Sensor** | MAX30102 | Pulse oximetry (HR + SpO2) | Analog (pins 34, 35) |
| **IMU** | MPU-6050 | Accelerometer + Gyroscope | I2C (0x68) |
| **GPS Module** | NEO-6M | Location tracking | UART (RX16, TX17) |
| **Temperature** | MLX90614 | IR thermometer | I2C |
| **Microphone** | MAX4466 | Audio recording | Analog (pin 33) |
| **ADC** | ADS1115 | 16-bit analog conversion | I2C |

**Estimated BOM Cost**: ~$50-70 (vs $200+ commercial smartwatches)

---

## 🏗️ System Architecture

### Data Flow

```
[Sensors] → [ESP32] → [WiFi] → [Firebase] → [Web Dashboard]
   │
   ├─ MAX30102 (HR, SpO2)
   ├─ MPU-6050 (Motion, Fall Detection)
   ├─ NEO-6M (GPS)
   ├─ MLX90614 (Temperature)
   └─ MAX4466 (Audio)
```

### Fall Detection Algorithm

```cpp
// Trigger 1: Free fall detection
if (acceleration < 0.4g) {
    startFallSequence();
}

// Trigger 2: Impact detection
if (acceleration > 3g && trigger1Active) {
    detectImpact();
}

// Trigger 3: Orientation validation
if (orientationChange > 80° && trigger2Active) {
    confirmFallEvent();
}
```

---

## 💻 Quick Start

### Prerequisites
- Arduino IDE 1.8+ or PlatformIO
- ESP32 board support installed
- Firebase account (free tier)

### Installation

```bash
# Clone the repository
git clone https://github.com/AE707/fitTrack-hard.git
cd fitTrack-hard

# Install required libraries via Arduino Library Manager:
# - IOXhop_FirebaseESP32
# - Adafruit MPU6050
# - Adafruit MLX90614
# - Adafruit ADS1X15
# - TinyGPS++

# Configure WiFi and Firebase credentials in main.ino:
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";
#define FIREBASE_HOST "YOUR_FIREBASE_URL"
#define FIREBASE_AUTH "YOUR_AUTH_TOKEN"

# Upload to ESP32
# Tools > Board > ESP32 Dev Module
# Upload
```

### ⚠️ Important Notes
- **Security**: Credentials are hardcoded in this prototype. **Never deploy to production** without proper secret management.
- **Calibration**: MPU-6050 requires calibration for accurate fall detection.
- **Power**: This prototype requires external power; battery management not yet implemented.

---

## 🚀 Future Roadmap

### Phase 2: Hardware Refinement
- [ ] **PCB Design** - Custom board instead of breadboard
- [ ] **Battery Management** - LiPo battery + charging circuit
- [ ] **Display Integration** - OLED screen for on-device feedback
- [ ] **Case Design** - 3D-printable enclosure
- [ ] **Wearable Form Factor** - Wrist strap integration

### Phase 3: Software Enhancement
- [ ] **OTA Updates** - Over-the-air firmware deployment
- [ ] **Power Optimization** - Deep sleep modes, battery life > 24hrs
- [ ] **Advanced Algorithms**:
  - Kalman filtering for sensor fusion
  - Heart rate variability (HRV) analysis
  - Sleep tracking via motion patterns
  - Step counting and calorie estimation
- [ ] **Local Storage** - SD card logging for offline operation
- [ ] **Encryption** - Secure data transmission

### Phase 4: User Experience
- [ ] **Mobile App** - React Native companion app
- [ ] **Web Dashboard** - Real-time health visualization
- [ ] **Notifications** - Bluetooth alerts to smartphone
- [ ] **Voice Commands** - Wake word detection
- [ ] **Multi-User Support** - Family health monitoring

### Phase 5: Advanced Features
- [ ] **ECG Monitoring** - Single-lead electrocardiogram
- [ ] **Stress Detection** - HRV-based stress analysis
- [ ] **AI Health Insights** - ML-based anomaly detection
- [ ] **Emergency Alerts** - Auto-dial on fall detection
- [ ] **Integration APIs** - Apple Health, Google Fit

---

## 📚 Code Structure

```
fitTrack-hard/
├── main.ino              # Main application logic
├── GPS.cpp / GPS.h       # GPS module driver
├── MAX30102.cpp / .h     # Heart rate sensor driver
├── MAX4466.cpp / .h      # Microphone driver
└── .vscode/              # VS Code configuration
```

### Key Modules

**GPS Module** (`GPS.cpp`):
```cpp
class GPSModule {
    bool getData(float& lat, float& lng, float& alt, 
                 unsigned long& time, float& speed);
};
```

**Heart Rate** (`MAX30102.cpp`):
```cpp
class MAX30102 {
    int calculateHeartRate();
    int calculateSpo2();
};
```

---

## 🛡️ Known Limitations

This is an **early prototype** with several limitations:

### Hardware
- ⚠️ No battery power (requires USB/external supply)
- ⚠️ Breadboard assembly (fragile, not wearable)
- ⚠️ No display (relies on Serial Monitor)
- ⚠️ Large form factor (not practical for wrist wear)

### Software
- ⚠️ Hardcoded credentials (security risk)
- ⚠️ Minimal error handling
- ⚠️ No data validation or checksums
- ⚠️ Audio data uploaded as strings (inefficient)
- ⚠️ No user interface

### Algorithm
- ⚠️ Fall detection not clinically validated
- ⚠️ MPU-6050 calibration hardcoded (may not work for all users)
- ⚠️ No machine learning (rule-based only)

---

## 💼 Why This Project is Valuable

### For Learning
- **Multi-Sensor Integration** - Practical experience with 6+ different sensors
- **IoT Architecture** - End-to-end cloud-connected device
- **Signal Processing** - Fall detection algorithm design
- **Embedded Systems** - Real-time constraints, power considerations

### For Portfolio
- Demonstrates **hardware + software** full-stack skills
- Shows **problem-solving** in wearable tech space
- Highlights **algorithm development** (fall detection)
- Proves **cloud integration** capabilities

### Transferable Skills
- **Real-time data processing** → Game engines, multiplayer servers
- **Sensor fusion** → Robotics, AR/VR, autonomous systems
- **IoT architecture** → Smart home, industrial IoT
- **Health tech** → Medical devices, telemedicine

---

## 🤝 Contributing

This project is currently in **prototype phase** and not actively maintained. However, if you're interested in:

- Forking and building your own version
- Suggesting improvements to the fall detection algorithm
- Sharing your own wearable IoT projects

Feel free to open an issue or discussion!

---

## 📝 License

This project is provided as-is for educational and reference purposes. 

**Note**: Before using this project as a foundation for any product:
- Replace hardcoded credentials with environment variables
- Add proper error handling and data validation
- Conduct safety testing (especially for fall detection)
- Comply with medical device regulations if used for health monitoring

---

## 👤 Author

**Alaa ElGhoul** - Computer Engineering (MSc), ENIG
Specializations: IoT Systems, Embedded Devices, Robotics

**Other Projects**:
- [SubM](https://github.com/AE707/SubM) - Distributed multi-ESP32 underwater vehicle

---

## 📊 Project Timeline

- **June 2024**: Initial prototype development
- **Current**: Documentation and concept refinement
- **Next Phase**: Awaiting hardware redesign for Phase 2

---

## 👀 Learn More

Interested in the technical details?

- **Fall Detection Algorithm**: See `main.ino` lines 81-136 for the 3-trigger implementation
- **Sensor Fusion**: Check how IMU data combines with GPS for activity tracking
- **Firebase Integration**: `sendDataToFirebase()` function shows real-time data streaming

---

<div align="center">

**An experimental exploration of DIY wearable health technology**

*Built with curiosity, refined through iteration*

</div>
