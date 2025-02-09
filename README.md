# 🚗 **CanDrive Smart Dashboard**  

## 📌 Project Overview  
**CanDrive Smart Dashboard** is an innovative embedded system project developed as part of the **PG-Diploma in Embedded System Design**. This dashboard provides **real-time monitoring and display** of essential vehicle parameters, improving safety and efficiency.  

### 🔍 **Monitored Parameters**  
- **📏 Vehicle Speed & Distance:** Measured using an **IR sensor** (wheel revolution-based calculation).  
- **🌡 Device Temperature:** Tracked via a **TMP102 sensor** and displayed on the dashboard.  
- **🚪 Door Status:** Monitored with a **reed switch** to detect open/closed doors.  

---  

## ✅ **Current Status**  
🚀 The project is **fully developed and operational** with ongoing enhancements for better performance and robustness.  

---  

## ⭐ **Key Features**  
### 📡 **Real-Time Data Display**  
🔹 Instant updates on speed, temperature, and door status.  

### 🔗 **Robust Communication**  
🔹 **CAN bus**: Data exchange between STM32-based nodes.  
🔹 **UART**: Transfers sensor data from STM32 to ESP32.  
🔹 **MQTT Protocol**: Sends processed data from ESP32 to the **ThingsBoard** dashboard over Wi-Fi.  

### ⚙ **FreeRTOS Integration**  
🔹 **STM32 Microcontrollers** use **FreeRTOS** for efficient data collection and task management.  

### 📊 **User-Friendly Interface**  
🔹 **ThingsBoard-based** visualization dashboard for intuitive monitoring.  

---  

## 🔧 **Components and Tools**  
### 🖥 **Microcontrollers**  
✔ **STM32** - Data collection and processing.  
✔ **ESP32** - Cloud communication and MQTT handling.  

### 🛠 **Sensors**  
✔ **IR Sensor** - Speed & Distance Calculation.  
✔ **TMP102** - Temperature Monitoring.  
✔ **Reed Switch** - Door Status Detection.  
✔ **(Future) INA219** - Power Monitoring for stability.  

### 🖥 **Software Tools**  
✔ **STM32CubeIDE**, **Arduino IDE**, **VS Code**  

### 📝 **Programming Languages**  
✔ **Embedded C, Python**  

---  

## 🚀 **Future Enhancements**  
✅ **Power Monitoring**: Integration of **INA219** for real-time power tracking.  
✅ **Enhanced Dashboard**: Improved **ThingsBoard analytics** for better insights.  
✅ **Safety Upgrades**: Additional monitoring features for increased reliability.  

📢 *This project is a step forward in automotive embedded systems! With continuous improvements, we aim to make it more efficient, scalable, and adaptable for the future.* 🚀  

---  

 

