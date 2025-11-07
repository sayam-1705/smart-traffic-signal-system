# 🚦 Smart Adaptive Traffic Signal System (Arduino-Based)

### 📍 Project Overview
The **Smart Adaptive Traffic Signal System** is an Arduino-based prototype that intelligently controls traffic lights based on **real-time traffic density and motion detection**.  
It minimizes waiting time, improves traffic flow efficiency, and safely handles emergency and pedestrian situations — making it a practical demonstration of **smart city automation**.

---

## 🧠 Key Features

| Feature | Description |
|----------|-------------|
| **Adaptive Timing** | Automatically adjusts green light duration using an ultrasonic distance sensor. |
| **Motion Detection** | Detects vehicle movement on the secondary lane using a PIR sensor. |
| **Emergency Mode** | Instantly prioritizes one lane for emergency vehicles. |
| **Pedestrian Mode** | Halts all lanes temporarily for safe crossing with a buzzer alert. |
| **Visual Display** | LCD shows real-time status: lane, distance, activity, and timers. |
| **Audible Alerts** | Buzzer provides transition beeps and pedestrian countdown signals. |

---

## 🧩 Components Used

| Component | Quantity | Function |
|------------|-----------|-----------|
| Arduino UNO | 1 | Main controller |
| HC-SR04 Ultrasonic Sensor | 1 | Measures queue distance (Lane 1) |
| PIR Motion Sensor | 1 | Detects movement (Lane 2) |
| 16x2 I²C LCD Display | 1 | Displays system status |
| LEDs (Red, Yellow, Green) | 6 | Simulate traffic lights |
| Push Buttons | 2 | Pedestrian and emergency triggers |
| Buzzer | 1 | Audible alerts |
| Breadboard, Jumpers, Resistors | — | Circuit wiring and support |

---

## ⚙️ Working Principle

1. **Traffic Density Measurement**
   - Ultrasonic sensor measures the distance of vehicles queued in Lane 1.
   - Shorter distance → higher density → longer green duration.

2. **Motion Detection**
   - PIR sensor detects approaching vehicles on Lane 2.
   - Detected motion increases green time for Lane 2.

3. **Adaptive Control**
   - Each lane’s green light duration adjusts dynamically:
     - High density → 7s  
     - Medium → 5s  
     - Low → 3s  

4. **Emergency Mode**
   - Pressing the emergency button immediately activates green for Lane 1.

5. **Pedestrian Mode**
   - Pedestrian button activates an all-red state for 5 seconds with buzzer countdown.

6. **Safety Transition**
   - Includes 2-second yellow and 1-second all-red intervals to prevent collisions.

---

## 🧾 Circuit Connection Table

| Component | Arduino Pin | Description |
|------------|--------------|-------------|
| HC-SR04 TRIG | D2 | Ultrasonic trigger |
| HC-SR04 ECHO | D3 | Ultrasonic echo |
| PIR OUT | D4 | Motion input |
| Lane 1 LEDs | D5–D7 | Green, Yellow, Red |
| Lane 2 LEDs | D8–D10 | Green, Yellow, Red |
| Pedestrian Button | A0 | Active LOW input |
| Emergency Button | A1 | Active LOW input |
| Buzzer | A2 | Tone output |
| LCD SDA | A4 | I²C data |
| LCD SCL | A5 | I²C clock |
| +5V / GND | — | Power to all modules |

---

## 🧱 Block Diagram

    ┌─────────────────────────────────┐
    │           Arduino UNO           │
    ├─────────────────────────────────┤
    │  HC-SR04 → D2 (TRIG), D3 (ECHO) │
    │  PIR → D4                       │
    │  LCD I²C → A4 (SDA), A5 (SCL)   │
    │  Ped Btn → A0, Emerg Btn → A1   │
    │  Buzzer → A2                    │
    │  Lane 1 LEDs → D5–D7            │
    │  Lane 2 LEDs → D8–D10           │
    └─────────────────────────────────┘

---

## 💻 Arduino Code

> Full code is available in [`Smart_Traffic_Signal.ino`](./Smart_Traffic_Signal.ino)

The code:
- Reads sensor inputs (HC-SR04, PIR)  
- Calculates adaptive green times  
- Displays results on LCD  
- Handles pedestrian/emergency overrides  
- Controls LEDs and buzzer alerts  

---

## 🔍 Verification & Testing

| Test | Expected Behavior |
|------|--------------------|
| **Power ON** | LCD shows “Smart Traffic System Ready.” |
| **Ultrasonic Sensor** | Move an object closer → Lane 1 green time increases. |
| **PIR Sensor** | Wave hand near PIR → Lane 2 activity increases. |
| **Pedestrian Button** | All lights turn red for 5 seconds; buzzer beeps. |
| **Emergency Button** | Lane 1 turns green immediately. |
| **LCD Display** | Shows lane status, sensor readings, and timers. |

Use the **Serial Monitor** (115200 baud) to verify sensor data and timing logic.

---

## 🧮 Timing Configuration

| Traffic Condition | Lane 1 (Ultrasonic) | Lane 2 (PIR) |
|--------------------|---------------------|---------------|
| High Density | 7 s Green | 3–5 s Green |
| Medium Density | 5 s Green | 3–5 s Green |
| Low Density | 3 s Green | 3 s Green |

Yellow phase → 2 s  
All red → 0.8 s  
Pedestrian crossing → 5 s  

---

## 🧠 Project Impact

- **Technical Impact:** Demonstrates adaptive embedded control using real-time sensor feedback.  
- **Social Impact:** Reduces idle time, saves fuel, and improves pedestrian safety.  
- **Scalability:** Can be expanded to 4-lane or IoT-connected traffic systems.  

---

## 🧰 Future Enhancements

- Add **Wi-Fi (ESP8266)** or **GSM** module for remote traffic monitoring.  
- Integrate **multiple ultrasonic sensors** for 4-way intersections.  
- Add **data logging** for traffic analytics.  
- Use **machine vision (camera module)** for vehicle classification.  

---

## 👨‍💻 Project Details

- **Author:** Sayam Mondal  
- **Hardware Platform:** Arduino UNO R3  
- **Programming Language:** C++ (Arduino IDE)  
- **Version:** 1.0  
- **Project Type:** Personal Academic Project  
- **Repository Purpose:** For documentation and demonstration of embedded system design.  

---

## ⚖️ Personal Use Notice

This project is developed solely for **personal and academic demonstration purposes**.  
You are free to view, study, and replicate this design for **learning or non-commercial projects**.  
Please credit the author if you reference or reuse any part of the code or documentation.

---

### 🚀 *Smart traffic control today for smarter cities tomorrow.*
