# 🌱 Smart Irrigation System V2

An IoT-based Smart Irrigation System built using the ESP32 that monitors soil moisture, temperature, and humidity in real time. The system displays sensor data on an OLED screen, sends live updates to the Blynk IoT app, and provides buzzer alerts based on soil conditions.

---

## Features

- Soil moisture monitoring
- Temperature & humidity monitoring using DHT11
- 0.96" OLED live display
- WiFi connectivity using ESP32
- Blynk IoT dashboard
- Automatic WiFi reconnection
- Soil moisture calibration
- Buzzer alerts for dry and overwatered soil
- Modular Arduino code

---

## Hardware Used

- ESP32 Dev Board
- FC-28 Soil Moisture Sensor
- DHT11 Temperature & Humidity Sensor
- 0.96" OLED Display (SSD1306)
- Active Buzzer
- IRLZ44N MOSFET
- Breadboard
- Jumper Wires
- USB Power Supply

---

## Software Used

- Arduino IDE
- Blynk IoT
- ESP32 Board Package
- Adafruit SSD1306 Library
- Adafruit GFX Library
- DHT Library

---

## Working

The ESP32 continuously reads:

- Soil Moisture
- Temperature
- Humidity

The values are displayed on the OLED screen and uploaded to the Blynk IoT dashboard over WiFi.

Depending on the soil moisture:

- Below 30% → Start Pump (Dry Soil)
- 30–80% → Normal
- Above 80% → Stop Pump (Wet Soil)

The buzzer alerts the user according to the soil condition.

---

## Project Images

- Hardware Setup
- OLED Display
- Blynk Dashboard
- Circuit Diagram

---

## Future Improvements

- Automatic pump control using relay
- Water level monitoring
- Rain sensor integration
- Weather API support
- Mobile notifications
- Solar-powered operation

---

## Author

**Paavan Sharma**

Electronics & Communication Engineering (ECE)

LinkedIn:
https://www.linkedin.com/in/paavan-sharma-627638264/

---

⭐ If you like this project, consider giving it a star.
