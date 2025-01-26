#include <Arduino.h>
#include <Wire.h>

#include <rv8523.h>

RV8523 rtc;

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println("Initializing RV-8523");

  Wire.begin();
  while(!rtc.begin(Wire)){
    Serial.println("No connection to clock");
    delay(1000);
  }


  Serial.println("Setting Time of the Rv-8523");

  uint8_t seconds = 0;
  uint8_t minutes = 10;
  uint8_t hours = 20;
  uint8_t day = 23;
  uint8_t weekday = 5;
  uint8_t month = 8;
  uint8_t year = 25;

  /* rtc.set12Hours() / rtc.set24Hours() for 12/24 hours mode. 24 hours mode is the default */
  if (rtc.setTime(seconds, minutes, hours, day, weekday, month, year)){
    Serial.println("Time set successfully");
  }
  else{
    Serial.println("Updating the time failed...");
    while(1);
  }
}

void loop() {
  /* Time always has to be updated before getting the time as string or as value. */
  if (!rtc.updateTime()){
    Serial.println("Failed to update the time.");
    while(1);
  }

  /*
  Serial.print("Seconds: ");
  Serial.println(rtc.getSeconds());
  Serial.print("Current hour: ");
  Serial.println(rtc.getHours());
  */
  

  Serial.println(rtc.stringTimeStamp());
  delay(1000);
}
