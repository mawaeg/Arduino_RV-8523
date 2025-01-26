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

  Serial.println("Setting up Timer A");
  if(!
  rtc.setTimerA(RV8523_TIMER_IRQ_MODE_PERMANENT,   /* Once triggered, the interrupt should be active until it is cleared */
                RV8523_SOURCE_CLOCK_FREQUENCY_1HZ, /* Choose a source clock frequency of 1 Hz */
                30,                                /* Due to the source frequency of 1Hz a value of 30 sets a timer to 30 seconds */
                true,                              /* Enables the interrupt for the Timer */
                RV8523_TIMER_A_MODE_COUNTDOWN      /* Enables countdown mode of the timer */
                )
  ){
    Serial.println("Setting up Timer A failed...");
    while(1);
  }
}

void loop() {
  if(rtc.readTimerACDInterruptFlag()){
    Serial.println("TimerA has expired and a interrupt has been generated. Clearing the interrupt flag.");
    rtc.clearTimerAInterruptFlag();
  }
  delay(100);
}
