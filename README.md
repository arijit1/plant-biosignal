# 🌿 Lily Bio-Monitor: Plant Electrophysiology Dashboard

A high-precision, wireless bio-data acquisition system designed to monitor and record the electrical action potentials of a Lily plant using an ESP32 and an AD620 Instrumentation Amplifier.

---

## 🚀 Project Overview

This project transforms a houseplant into a data-generating organism. By measuring micro-voltage shifts between the leaf and the stem, we observe the plant's response to environmental stimuli (touch, light, water) in real-time via a web-based dashboard hosted directly on the ESP32.

---

## 🛠 Hardware Setup

- **Microcontroller:** ESP32 (utilizing 12-bit ADC resolution)
- **Amplifier:** AD620 Instrumentation Amplifier (low-noise, high-precision)
- **Electrodes:**  
  - **S+** → Leaf Contact  
  - **S-** → Stem / Ground Reference
- **Grounding:** A common ground is established via a moist soil probe to minimize floating signals and signal drift

### Calibration

1. **104 Potentiometer (Offset):**  
   Adjusted to center the baseline at approximately **1500mV – 1650mV**  
   (middle of the 3.3V range)

2. **103 Potentiometer (Gain):**  
   Tuned clockwise to increase sensitivity until a leaf pinch causes a visible spike (**>500mV**)

---

## 💻 Code Logic & Features

### 1. Brute Force Sampling (Signal Cleaning)

To eliminate the **50/60Hz electrical hum** from room wiring and chargers, the firmware performs **oversampling**.

```cpp
for(int i = 0; i < 1000; i++) {
  sum += analogRead(plantPin);
  delayMicroseconds(50);
}
int val = sum / 1000;
```

It takes **1,000 rapid readings** and averages them to create a single, stable data point, effectively filtering out high-frequency noise.

---

### 2. Physical Unit Conversion

The raw **12-bit digital value (0–4095)** is converted into actual **millivolts (mV)** based on the ESP32’s **3300mV logic level**.

```cpp
float mV = (val / 4095.0) * 3300.0;
```

---

### 3. Persistent Peak Tracking (JavaScript)

The browser-based UI utilizes the `localStorage` API to track **Session Highest** and **Session Lowest** peaks.

```javascript
let hi = localStorage.getItem('lilyHi') || -9999;
if(cur > hi) {
  localStorage.setItem('lilyHi', cur);
  hi = cur;
}
```
```terminal command for port
ls /dev/cu*
```

This ensures that the dashboard remembers the **absolute maximum and minimum values** detected since the last manual reset, even if the page is refreshed.

---

## ⚠️ Limitations

- **WiFi Interference:**  
  The ESP32’s WiFi radio generates high-frequency electrical noise. Keep the ESP32 antenna away from the AD620 module.

- **Browser Sampling Rate:**  
  The current Web UI refreshes every **1 second**. Extremely fast biological events (**<500ms**) may be missed in the browser view but captured in raw sampling.

- **Electrode Contact:**  
  Signal quality depends heavily on soil moisture levels and leaf surface contact quality.

---
## Outcomes
- **After leaf is pinched , it spikes 3200mV  **
<img width="1440" height="900" alt="Screenshot 2026-04-04 at 10 10 13 PM" src="https://github.com/user-attachments/assets/593085b0-cab9-4caf-bca8-2fc7ee8b5a29" />
<img width="1188" height="806" alt="image" src="https://github.com/user-attachments/assets/5b5777cb-255f-4a1d-93a9-76de52bfeb50" />
<img width="1188" height="806" alt="image" src="https://github.com/user-attachments/assets/91a3b56f-6756-44cf-aa7a-e0f2c279cab5" />

## Circuit Diagram

*Developed for the Lily Electrophysiology Bio-Data Project.*
