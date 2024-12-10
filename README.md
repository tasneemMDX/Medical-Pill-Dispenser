MedHub: Smart IoT-Based Pill Dispenser
Welcome to MedHub, a smart IoT-based pill dispenser designed to simplify medication management for senior citizens. This device provides an automated and reliable solution to ensure patients take their medications on time, improving adherence, safety, and overall quality of life.

Features
Automated Pill Dispensing: Dispenses the correct pill at scheduled times with the help of a stepper motor.
Dual Timer System: Set two timers—morning and evening—for customizable medication schedules.
Real-Time Monitoring: Tracks whether the patient has taken the pill using an IR sensor.
Blynk App Integration:
Caregivers receive real-time notifications for pill dispensation and missed doses.
Downloadable data sheets for monitoring medication history via the Blynk cloud.
Alerts and Notifications:
Visual Alert: LED light indicates when it’s time for a dose.
Audio Alert: Plays a reminder to prompt patients to take their medication.
Data Logging: Logs adherence records that can be accessed by caregivers or healthcare providers to adjust treatments and improve health outcomes.
User-Friendly Design: Simple, intuitive interface for elderly users, fostering independence.
How It Works
Setup:
Plug in the device and connect it to the internet using WiFi credentials.
Use the Blynk app to set up your medication schedule and timers.
Operation:
At the scheduled time, the dispenser activates, dispensing the correct pill.
Visual (LED) and audio alerts notify the patient.
The IR sensor detects whether the pill is retrieved.
Notifications:
If the pill is taken, the Blynk app logs it in real time.
If the pill is missed within the predefined duration, the caregiver receives an alert.
Data Access:
Caregivers can download adherence reports and medication history from the Blynk cloud for better monitoring.
Hardware Components
Arduino UNO R4: Central controller for the device.
Stepper Motor: Precisely rotates the pill container to dispense medication.
IR Sensor: Monitors pill retrieval.
DHT22 Sensor: Tracks temperature and humidity for optimal medication storage.
MP3 Module with Bluetooth Speaker: Plays audio alerts.
LCD Display: Displays countdown, real-time clock, and dispensing status.
LED Light: Visual notification for patients.
Software Requirements
Blynk App: Downloadable on iOS and Android devices.
Arduino IDE: For uploading the code to the Arduino board.
Setup Instructions
Connect the device to power.
Upload the MedHub code to the Arduino UNO R4 using the Arduino IDE.
Open the Blynk app, log in, and enter the Blynk token provided in the code.
Configure WiFi settings within the code (char ssid[] and char pass[]).
Set up timers and alerts in the app for morning and evening medications.
Ensure all hardware components are connected as per the wiring diagram.
