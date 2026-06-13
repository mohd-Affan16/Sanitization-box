# Sterilization Box

**My third mini-project** — Built during 3rd semester (DDCO subject) at **P.A. College of Engineering, Mangalore** (Group Project).

A smart sterilization chamber designed for safe, remote-controlled disinfection with multiple safety features.
<p align="center">
<img width="450" height="350" alt="WhatsApp Image 2026-06-12 at 11 25 26 PM" src="https://github.com/user-attachments/assets/8a46dea6-c8c3-4573-99e8-84d6eedeb444" />
</p>

## 🎯 Project Overview

This system provides a safe way to sterilize items using UV-C light while preventing accidental exposure through smart safety interlocks and real-time notifications.

## ✨ Features

- Self-hosted static web server on ESP8266 for control
- IR sensor safety interlock (UV lamp works only when door is closed)
- Adjustable sterilization timer with relay control
- Buzzer alert on cycle completion
- Telegram Bot for real-time notifications
- Aluminum foil lining for better UV reflection and safety

## 🛠️ Technologies & Components Used

- **Microcontroller**: ESP8266
- **Sensor**: IR Sensor
- **Actuators**: Relay Module + UV-C Lamp + Buzzer
- **Connectivity**: Wi-Fi + Telegram Bot API
- **Web Interface**: HTML, CSS, JavaScript (static)
- **Development Environment**: Arduino IDE
- **Programming Language**: C/C++

## 📌 How It Works

The ESP8266 hosts a web page for starting/stopping the process and setting timer. The IR sensor ensures safety before turning on the UV-C lamp. Status updates are sent via Telegram Bot.

## 🧠 Biggest Lesson

By the third semester, I had moved from basic offline hardware to cloud-connected IoT. This project took it further by combining a self-hosted web interface, hardware safety mechanisms, and real-time alerts into one complete end-to-end system. Integrating everything reliably was challenging, but seeing the full flow work smoothly gave me great confidence.  

It reinforced how hardware, software, and connectivity can come together to build practical solutions for real-world safety applications.

## 📸 Project Photos

*(Add circuit photos, web interface screenshots, Telegram notifications, and final setup here)*

## 🔗 Links

- **Full Project Report**: [UV-C Sterilization Report.pdf](https://drive.google.com/file/d/1ToFa030doMJLHH_1xuawGg1L7lSJMNRW/view?usp=drivesdk)
- LinkedIn Post: [View post](https://www.linkedin.com/posts/mohammed-affan-79648934a_iot-embeddedsystems-esp8266-activity-7429895027952238592-Zh3_?utm_source=share&utm_medium=member_desktop&rcm=ACoAAFdHmSIBeqaWhdYUc5Wtk6K2MB35X1BTDFs)
- College: P.A. College of Engineering, Mangalore

---

