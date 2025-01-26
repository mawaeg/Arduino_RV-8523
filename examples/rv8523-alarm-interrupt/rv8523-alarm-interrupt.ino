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

  if (rtc.setTime(0, 10, 20, 23, 5, 8, 25)){
    Serial.println("Time set successfully");
  }
  else{
    Serial.println("Updating the time failed...");
    while(1);
  }

  Serial.println("Setting up the alarm");
  if(!
  rtc.setAlarm(11, 20, 23, 5,                      /* Setting alarm to trigger at the 23rd of the month on weekday 5 at 20:11 */
               RV8523_ALARM_MODE_MIN_HOUR_DAY_WDAY /* Instruct alarm that minute hour day and weekday must match*/
               /* When we would set RV8523_ALARM_MODE_MIN instead, only the minute value must match to trigger the alarm */ 
               )
   
  ){
    Serial.println("Setting up the alarm failed...");
    while(1);
  }
}

void loop() {
  if(rtc.readAlarmInterruptFlag()){
    Serial.println("The alarm triggered and a interrupt has been generated. Clearing the interrupt flag.");
    rtc.clearAlarmInterruptFlag();
  }
  delay(100);
}