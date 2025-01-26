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

  Serial.println("Setting Power Management of the Rv-8523");

  /* Configure rtc to enable direct battery switching and battery low detection. */
  if(!rtc.setBatterySwitchover(RV8523_BATTERY_SWITCHOVER_EN_DRCT__BAT_LOW_EN)){
    Serial.println("Configuring battery switchover failed...");
    while(1);
  }
}

void loop() {}
