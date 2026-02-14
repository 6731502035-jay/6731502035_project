# Gravity Meter Dashboard

A premium web dashboard to visualize soil moisture and control a water pump via ESP32.

![Screenshots](https://placehold.co/600x400?text=Gravity+Meter+Dashboard) 
*(Note: Start the app to see the actual dashboard)*

## Features
- **Gravity Meter**: Visual representation of moisture. Icon falls when dry, floats when wet.
- **Control Panel**: "Water" button to trigger pump on ESP32.
- **Real-time**: Polls ESP32 for status updates.

## Setup

### 1. Hardware (ESP32)
1. Open `firmware/gravity_meter.ino` in Arduino IDE.
2. Install **ArduinoJson** library via Library Manager.
3. Update `ssid` and `password` with your WiFi credentials.
4. Upload to ESP32.
5. Note the **IP Address** printed in the Serial Monitor.

### 2. Frontend
1. Navigate to `frontend` directory:
   ```bash
   cd frontend
   ```
2. Install dependencies:
   ```bash
   npm install
   ```
3. Start the development server:
   ```bash
   npm run dev
   ```
4. Open the link (e.g., `http://localhost:5173`).
5. Enter your ESP32's IP address in the dashboard.

## Usage
- The **Gravity Meter** shows current moisture.
- Click **Water** to turn on the pump for 5 seconds.
