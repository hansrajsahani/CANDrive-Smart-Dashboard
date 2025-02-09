# CanDrive Smart Dashboard  

## Project Overview  
*CanDrive Smart Dashboard* is an innovative embedded system project designed as part of the PG-Diploma in Embedded System Design. This dashboard provides real-time monitoring and display of essential vehicle parameters, enhancing the safety and efficiency of vehicle operations.  

The system integrates various sensors and displays to track and present:  
- *Vehicle Speed & Distance: Calculated using an **IR sensor* to determine wheel revolutions.  
- *Device Temperature: Measured using the **TMP102 temperature sensor* and displayed on the dashboard.  
- *Door Status: Monitored using a **reed switch* to detect open or closed doors.  

## Current Status  
The project is *fully developed and operational*, but we continue to enhance its features and robustness.  

## Key Features  
1. *Real-Time Data Display*: The system provides immediate updates on all monitored parameters.  
2. *Robust Communication*:  
   - *CAN bus* for data transfer between STM32-based data collection nodes.  
   - *UART communication* to transfer sensor data from STM32 to ESP32.  
   - *MQTT protocol* over Wi-Fi for sending processed data from ESP32 to the ThingsBoard dashboard.  
3. *FreeRTOS Implementation: Used on **STM32 microcontrollers* for better control over data collection and transmission.  
4. *User-Friendly Interface*: ThingsBoard-based dashboard for visualization.  

## Components and Tools  
- *Microcontrollers*: STM32 (for data collection), ESP32 (for cloud communication)  
- *Sensors*:  
  - *Speed & Distance Measurement*: IR sensor (wheel revolution-based calculation)  
  - *Temperature Monitoring*: TMP102 sensor  
  - *Door Status Detection*: Reed switch  
  - *Future Enhancement: **INA219* for power monitoring to ensure a stable power supply  
- *Online Dashboard*: ThingsBoard  
- *Software Tools*: STM32CubeIDE, Arduino IDE, VS Code  
- *Programming Language*: Embedded C, Python  

## Future Enhancements  
- Integration of *INA219* for real-time power monitoring.  
- Enhancements in the ThingsBoard dashboard for better analytics.  
- Additional safety and monitoring features for improved reliability.  

This project is an ongoing effort to push the boundaries of embedded systems in automotive applications. With continuous improvements, we aim to make it more efficient, scalable, and adaptable to future advancements. Stay tuned for updates! 🚀
