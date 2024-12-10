    
    #define BLYNK_PRINT Serial
  
    #define BLYNK_TEMPLATE_ID "TMPL6kfIUH31X"
    #define BLYNK_TEMPLATE_NAME "Pill Dispensing Machine"
    #define BLYNK_AUTH_TOKEN "bbqEVKPxqw81AvEGYwqLlPPlvCKgDwu9"
     
    int  ST =137;                            //here these are the steps  so basically there are  2048 steps in one rotation of stepper motor so lts say if you you
                                             //want to  break this rotation into 8  compartments then  it will become 2048/8 = 256 so the motor will  stopped whenever i/8 part will  be
                                             //completed  so you can set the number of steps after which the motor should stop
    
    
    long int pillMissedDuration =10000;        // this is the time duration for pill missed right now it is set to 1 mitute i.e 60000 ms  you an set as you want but we wil reommend 
                                               // to keep it 60000 or more  in order to avoid any server related issues

    
    #include <SPI.h>
    #include <WiFiS3.h>
    #include <BlynkSimpleWifi.h>
    
    // Your WiFi credentials.
    // Set password to "" for open networks.
    char ssid[] = "";
    char pass[] = "";
    
    BlynkTimer timer;


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <SoftwareSerial.h>
 #include "DHT.h"
 #include <Adafruit_MLX90614.h>
 #include "RTC.h"

//Include the NTP library
#include <NTPClient.h>
#include <WiFiUdp.h>

    Adafruit_MLX90614 mlx = Adafruit_MLX90614();

SoftwareSerial hw312(2, 4); // RX, TX
 int LedPin= 5;
 int BuzzerPin= 3;
  unsigned long prv= 0;
    #define DHTPIN 12  
    #define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
    DHT dht(DHTPIN, DHTTYPE);

// Command definitions
#define CMD_SEL_DEV 0x09
#define DEV_TF 0x02
#define CMD_SET_VOLUME 0x06
#define CMD_PLAY_W_INDEX 0x03
// Buffer for sending commands
static int8_t Send_buf[8] = {0};
// LCD configuration
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust address if necessary

// Stepper motor configuration
#define STEPS 2048 // Steps per revolution for 28BYJ-48
Stepper stepper(STEPS, 8, 10, 9, 11); // Pins connected to ULN2003 driver board

// IR sensor
const int irSensorPin = 6;

 //long int countdownDuration = 2 * 60 * 1000;


int Blynk_Hours=9;
int  Blynk_Minutes=0;
// Timer variables
 
long  countdownDuration1 = 85000000;  // (3600000)
long  countdownDuration2 = 86200000;   //12//
long  totalDuration1;//=((86400000-countdownDuration2)+countdownDuration1);
long  totalDuration2 ;//=(countdownDuration2-countdownDuration1);
long  remainingTime1;
long  remainingTime2;


long countdownStartTime = 0;
int flag=0;


WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP
NTPClient timeClient(Udp);



// State variables
bool motorRunning = false;
unsigned long irDetectionStart = 0;
int hrs;
bool timer1Active= true;
long  realTime;

void setup() {
  // Initialize LCD
  Serial.begin(9600);

Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  RTC.begin();
  Serial.println("\nStarting connection to server...");
  timeClient.begin();
  timeClient.update();

      dht.begin();
      mlx.begin();
      delay(2000);
      pinMode(LedPin, OUTPUT);
      pinMode(BuzzerPin, OUTPUT);
    pinMode( irSensorPin, INPUT);
    
    
     timer.setInterval(2000L, myTimerEvent);




   hw312.begin(9600); // HardwareSerial on GPIO16 and GPIO17

  delay(500); // Allow HW-312 to initialize

  // Select the TF Card as the device
  sendCommand(CMD_SEL_DEV, DEV_TF);
  delay(200);

  // Set volume to max (30)
  sendCommand(CMD_SET_VOLUME, 30);
  delay(200);

  // Play the first file (001.mp3)
  

 lcd.begin();
  
  // Initialize stepper motor
  stepper.setSpeed(10); // Set motor speed (RPM)
  
  // IR sensor
  pinMode(irSensorPin, INPUT);

  // Serial monitor
  

  // Initialize timer
  resetTimer();
  
// Serial.println(countdownDuration1);
// Serial.println(countdownDuration2);
 
}

void loop() {
 

RealTime();
Blynk.run();
      timer.run(); // Initiates BlynkTimer


  

   if (timer1Active)
  {
     
if (realTime <= countdownDuration1) {
    remainingTime1 = countdownDuration1 - realTime;
} else if (realTime > countdownDuration2) {
    remainingTime1 = (86400000 - realTime) + countdownDuration1;
}

       medTime1();
  if (remainingTime1<=0){timer1Active=false;  }
  
  }
  else
   {remainingTime2 = countdownDuration2 - realTime;
   medTime2();
   
   
   if (remainingTime2<=0){      timer1Active=true;}}









}

void medTime1()
{


  if (remainingTime1 > 0 && !motorRunning) {
    displayTime(remainingTime1);
  } else if (!motorRunning) {
    // Start motor when countdown reaches zero
    digitalWrite(LedPin,HIGH);
    motorRunning = true;
    Serial.println("Please take the medicine!");
        sendCommand(CMD_PLAY_W_INDEX, 1);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dispensing...");
  }

  // Motor control logic
  if (motorRunning) {
    stepper.step(ST); // Take one step at a time
    
    lcd.setCursor(0, 0);
    lcd.print("Dispensing...");

     
      digitalWrite(LedPin,LOW);
      // Stop motor if no detection
Blynk.logEvent("pill_dispensed", String(" Timer-1 Pill Dispensed, Please take your Medicine! ") );
         ///////////////////////////////////////      place first  notification  code here


      motorRunning = false;
      irDetectionStart = millis();
      flag=1;
      resetTimer();
    
  }

if (flag==1){
if (digitalRead(irSensorPin) == HIGH) {

      if (millis() - irDetectionStart > pillMissedDuration) { // If there is no hnd for more than a minute then  the pill missed notification will be send 
        Serial.println("Timer-2 Pill Missed!");


  ///////////////////////////////////      place notification reminder code here
 Blynk.logEvent("pill_not_taken", String("Timer-2 Pill Missed! ") );




       // sendCommand(CMD_PLAY_W_INDEX, 1);
        flag=0;
      }
    }
    if (digitalRead(irSensorPin) == LOW){flag=0;}
}


}

void medTime2()
{


  if (remainingTime2 > 0 && !motorRunning) {
    displayTime(remainingTime2);
  } else if (!motorRunning) {
    // Start motor when countdown reaches zero
    digitalWrite(LedPin,HIGH);analogWrite(BuzzerPin,127);
    Serial.println("Please take the medicine!");
        sendCommand(CMD_PLAY_W_INDEX, 1);
    motorRunning = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dispensing...");
  }

  // Motor control logic
  if (motorRunning) {
    stepper.step(ST); // Take one step at a time
    
    lcd.setCursor(0, 0);
    lcd.print("Dispensing...");
    
      digitalWrite(LedPin,LOW);
      // Stop motor if no detection
     


    Blynk.logEvent("pill_dispensed", String(" Timer-2 Pill Dispensed, Please take your Medicine! ") );

      motorRunning = false;
      irDetectionStart = millis();
      flag=1;
      resetTimer();
    
  }

if (flag==1){
if (digitalRead(irSensorPin) == HIGH) {

      if (millis() - irDetectionStart > pillMissedDuration) { // If there is no hnd for more than a minute then  the pill missed notification will be send 
        Serial.println("Timer-1 Pill Missed!");


  ///////////////////////////////////      place notification reminder code here
 Blynk.logEvent("pill_not_taken", String("Timer-1 Pill Missed! ") );




      
        flag=0;
      }
    }
    if (digitalRead(irSensorPin) == LOW){flag=0;}
}


}

void displayTime(long ms) {
  long int hours = ms / (60 * 60 * 1000);
  long int  minutes = (ms % (60 * 60 * 1000)) / (60 * 1000);
  long int seconds = (ms % (60 * 1000)) / 1000;


  
  if (seconds<10 && seconds>=0){ 
    String SC = "0"+String(seconds);
      lcd.setCursor(0,0);
  lcd.print("Timer : ");
  
  lcd.print(hours);
  lcd.print(":");
  lcd.print(minutes);
  lcd.print(":");
  lcd.print(SC);
  }
else{
  lcd.setCursor(0,0);
  lcd.print("Timer : ");
  
  lcd.print(hours);
  lcd.print(":");
  lcd.print(minutes);
  lcd.print(":");
  lcd.print(seconds);}
  
}

void resetTimer() {
  
  countdownStartTime = realTime;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Timer Reset!");
  delay(2000);
  lcd.clear();
}



void sendCommand(int8_t command, int16_t dat) {
  delay(20);

  Send_buf[0] = 0x7E;               // Start byte
  Send_buf[1] = 0xFF;               // Version
  Send_buf[2] = 0x06;               // Length
  Send_buf[3] = command;            // Command
  Send_buf[4] = 0x00;               // Feedback (0x00 = no feedback)
  Send_buf[5] = (int8_t)(dat >> 8); // Data high byte
  Send_buf[6] = (int8_t)(dat);      // Data low byte
  Send_buf[7] = 0xEF;               // End byte

  for (uint8_t i = 0; i < 8; i++) {
    hw312.write(Send_buf[i]); // Send the command byte by byte
  }

  // Debugging output
  Serial.print("Sent Command: ");
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print("0x");
    Serial.print(Send_buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}


void myTimerEvent()
    {
   
    float h = dht.readHumidity();
    Blynk.virtualWrite(V5, h);
    float t = dht.readTemperature();
    Serial.println(t);
    Blynk.virtualWrite(V4, t);
    
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));

    
    Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
    Blynk.virtualWrite(V6, mlx.readObjectTempC());
    }
    
    
    
   


BLYNK_WRITE(V0) {
// Called when the datastream V1 value changes

// param[0] is the user time value selected in seconds.
Serial.print("Timer 1in sec: ");
 countdownDuration1=param[0].asInt();
 countdownDuration1=countdownDuration1*1000;
 totalDuration1 =86400000-countdownDuration2;
 totalDuration1 =totalDuration1+countdownDuration1;


  
Serial.println(countdownDuration1);
Serial.println(totalDuration1);


}
BLYNK_WRITE(V9) {
// Called when the datastream V1 value changes

// param[0] is the user time value selected in seconds.
Serial.print("Timer2 in sec: ");
 countdownDuration2=param[0].asInt();
 countdownDuration2=countdownDuration2*1000;



Serial.println(countdownDuration2);
      Serial.println(realTime);


  


}



void RealTime() {
  // Time zone adjustment
  auto timeZoneOffsetHours = 4;
  auto unixTime = timeClient.getEpochTime() + (timeZoneOffsetHours * 3600);

  // Set RTC time
  RTCTime timeToSet = RTCTime(unixTime);
  RTC.setTime(timeToSet);

  // Retrieve the date and time from the RTC
  RTCTime currentTime;
  RTC.getTime(currentTime);
 int hrs=currentTime.getHour();
  int mns=currentTime.getMinutes();
  int secs=currentTime.getSeconds();
realTime = (hrs * 3600000) + (mns * 60000) + (secs * 1000);


  // Format date and time
  String date = String(currentTime.getDayOfMonth()) + "/" +
                String(Month2int(currentTime.getMonth())) + "/" +
                String(currentTime.getYear());
  String time = String(hrs) + ":" +
                String(mns) + ":" +
                String(secs);

  // Print to Serial Monitor
  // Serial.println(date + " - " + time);

  // Display on LCD
 
  lcd.setCursor(0, 1);
  lcd.print("Time: " + time); // Display the time on the second line
  if (millis()-prv>1000){ lcd.clear();prv=millis();}
  
}