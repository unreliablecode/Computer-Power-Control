# ⚡ Computer-Power-Control — ESP32 Web Controller

A simple yet practical web-based power controller for my **home server PC**, built with an **ESP32** microcontroller.
Since my motherboard doesn't support "power on after AC loss" and although **Wake-on-LAN** exists, I wanted a more hardware-level approach — so I wired a **relay module** to jump the power switch pins, all controlled via a local web interface.

---

## 📦 Features

* 🌐 **Web-based Control Panel** hosted on the ESP32.
* 🔒 **Simple Login Authentication** to prevent unauthorized access.
* ⚙️ **Relay Control** to jump the power switch pins of the PC.
* 📶 **Ping/Check Server Status** feature to test server availability.
* 🎨 **Modern UI with glassmorphism-inspired styling** for a clean, lightweight control interface.

---

## 📸 Screenshots

### 🔑 Login Page

![Login Page](https://github.com/user-attachments/assets/75ea59c3-6a8c-452d-808c-3cd9af401db4)

---

### 🖥️ Main Control Page

![Main Page](https://github.com/user-attachments/assets/df2591f0-42b5-4218-a2ab-3b6f66374e3a)

---

## 🔌 How It Works

* **ESP32 connects to your local Wi-Fi** using static IP configuration.
* Host a simple webserver with login authentication.
* **Trigger the relay** by sending a HIGH signal to GPIO32 (for 500ms) when "Toggle Relay" is clicked.
* **Check server status** by attempting a TCP connection to the server’s IP and port (in this case, `192.168.1.7:80`).
* **Smooth UI interactions** with native JavaScript `fetch()` calls and status display.

---

## 🛠️ Tech Stack

* **ESP32 (WiFi Microcontroller)**
* **C++ / Arduino Framework**
* **WebServer Library**
* **HTML, CSS (Glassmorphism styling)**
* **JavaScript (for async server check)**

---

## 📡 Why Not Wake-on-LAN?

Because while **Wake-on-LAN (WoL)** works, it depends on BIOS settings, network card support, and is sometimes unreliable in certain conditions.
This method gives me **physical-level control** by momentarily shorting the power switch pins, ensuring the machine powers on even if WoL isn't an option.

---

## ⚙️ Future Improvements

* Add **MQTT integration** for remote control via home automation systems.
* Implement a **mobile-friendly responsive layout**.
* Option to **schedule automatic power cycles**.
* Add **server response time chart** history.

---

## 📝 License

Free to use, modify, and learn from. Attribution appreciated ✌️.

---
