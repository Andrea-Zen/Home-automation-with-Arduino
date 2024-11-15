
Here's an English version of the README for your Arduino home automation project:

Home Automation Project with Arduino UNO and ESP8266
Project Description
This project is a home automation system built with Arduino UNO and ESP8266. It uses environmental sensors (temperature, humidity, air quality, light, and motion) to monitor the conditions of a room or area and automatically control various relays based on preset conditions or schedules. A web interface allows real-time viewing of sensor data, relay scheduling, email alerts, and emergency shutdowns.

Features
Environmental Monitoring:

Temperature and humidity readings (DHT22).
Air quality monitoring (MQ135).
Light level detection (LDR).
Motion sensor to detect presence in a room.
Scheduled Device Control:

Scheduled relay activation/deactivation at specific times, configurable via the web interface.
Historical Data Visualization:

Displays line charts of the last 10 readings for temperature, humidity, and air quality using Chart.js.
Web Interface:

Remote access to view sensor readings.
Manual control of relays and scheduling options.
Emergency shutdown to deactivate all devices.
Email Notifications:

Automatic email alerts when critical values are detected (e.g., air quality exceeds safe levels or motion is detected).
Event Log:

Logs significant events, like relay state changes and air quality alerts.
Required Hardware
Arduino UNO
ESP8266 WiFi Module
DHT22 Temperature and Humidity Sensor
MQ135 Gas Sensor for air quality monitoring
PIR Motion Sensor
Light Dependent Resistor (LDR) for light level measurement
Relays (at least 2) for device control
Real Time Clock (RTC) Module DS1307
Connecting wires, resistors, and breadboard
Setup
Wiring Diagram
DHT22: Connect the data pin to Arduino pin D2.
MQ135: Connect the analog output pin to Arduino pin A1.
LDR: Connect the LDR with a resistor configuration to pin A0.
PIR: Connect the output pin to Arduino pin D5.
Relays:
Relay 1 to pin D3.
Relay 2 to pin D4.
ESP8266 Module: Connect RX and TX to manage WiFi connections.
RTC DS1307: Connect SDA and SCL to Arduino pins A4 and A5 for I2C communication.
Software Setup
Install Required Libraries:

DHT Sensor Library
MQ135 Library
RTC Library
ESP8266WiFi
Chart.js (included in the project as a CDN link for data visualization)
Configure WiFi Credentials:

In the .ino file, enter your network name and password in the ssid and password variables.
Email Setup:

Set the email sender address, recipient address, and account password for alert notifications.
Upload the Code:

Use the Arduino IDE to upload the code to the Arduino UNO.
Ensure that the ESP8266 module is properly connected and that Arduino can establish a WiFi connection.
Using the System
Accessing the Web Interface:

Once connected to the network, the system will display the assigned IP address of the ESP8266 in the serial monitor. Use this IP address in a browser to access the control interface.
Viewing Data:

The homepage displays real-time data for temperature, humidity, air quality, light level, and motion detection.
Data is updated every 2 seconds for continuous monitoring.
Charts:

Line charts display the last 10 readings from sensors, showing trends over time.
Device Control:

Turn relays on or off manually or set specific times for automatic activation/deactivation.
Emergency Shutdown:

Use the "Emergency Shutdown" button to turn off all relays immediately if needed.
Receiving Notifications:

Automatically receive email alerts when air quality reaches a critical level or motion is detected.
Key Code Functions
Sensor Data Collection and Storage:

Each sensor reading is stored in arrays for historical tracking and graphical display.
Relay Management:

Functions to manually or automatically activate relays on a schedule and handle emergency shutdown.
Dynamic Web Interface:

Generates an HTML page to display and control device states and sensor data.
Email Notifications:

Uses an SMTP library for automatic email alerts when critical conditions are detected.
Event Logging:

Logs significant events, such as motion detection and relay state changes, to facilitate troubleshooting and historical analysis.
Possible Extensions
Additional Sensors: Add sensors like smoke or CO2 detectors for monitoring other safety parameters.
AI-Based Automations: Implement advanced analytics and automatic decision-making using an external module.
Integration with Home Automation Systems: Connect to systems like Home Assistant or OpenHAB for control through a dedicated app.
Troubleshooting
WiFi Not Connected: Check that WiFi credentials are correct and the ESP8266 module is configured properly.
Emails Not Sending: Verify email settings, SMTP authentication, and any provider restrictions.
Sensor Readings Are Incorrect: Ensure sensors are properly connected and no short circuits are present.
