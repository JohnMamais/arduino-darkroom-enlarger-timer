#define DEBUG 1

#if DEBUG
  #define debug(x) Serial.print(x)
  #define debugln(x) Serial.println(x)
#else
  #define debug(x)
  #define debugln(x)
#endif

#include "LedControl.h"
#include <Wire.h>



const int START_BUTTON = 11;
const int FOCUS_BUTTON = 12;

const int RELAY_PIN = 2;

const int SUB_STOP = 4;
const int SUB_SEC = 5;
const int ADD_SEC = 6;
const int ADD_STOP = 7;

const int DATA_IN = 8;
const int CLK = 9;
const int LOAD = 10;

// dat, clk, load, num of MAX72xx modules
LedControl lc=LedControl(DATA_IN, CLK, LOAD, 1);


unsigned int time = 10;
// time_c is for the countdown time, keeping the original time intact
unsigned int time_c;

unsigned long buttonDelay = 400; // Minimum delay between button presses (in milliseconds)
unsigned long lastButtonPress = 0; // Store the last time a button was pressed

unsigned long previousMillis = 0; // Will store the last time the countdown updated
bool countdownRunning = false; // Flag to track whether countdown is running

void setup() {

  pinMode(RELAY_PIN, OUTPUT);

  pinMode(START_BUTTON, INPUT);
  pinMode(FOCUS_BUTTON, INPUT);

  pinMode(SUB_STOP, INPUT);
  pinMode(ADD_STOP, INPUT);
  pinMode(SUB_SEC, INPUT);
  pinMode(ADD_SEC, INPUT);

  /*
  
  // The MAX72XX is in power-saving mode on startup,
  // we have to do a wakeup call
  
  lc.shutdown(0,false);
  // Set the brightness to a medium values 
  lc.setIntensity(0,8);
  // and clear the display 
  lc.clearDisplay(0);

  displayTime(time);
  
  */

  Serial.begin(9600);
  debug("Setup complete");
}

void loop() {
  
  unsigned long currentMillis = millis();

  // Prioritize countdown
  // If countdown is running, update the display every second
  /*if (countdownRunning) {
    
    // Only update every second
    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
      time_c--;

      if (time_c <= 0) {
        countdownRunning = false; // Stop the countdown when it reaches 0
        
        // turn off enlarger head
        stopEnlarger();

        // reset the displayed time
        time_c = time;
      }

      displayTime(time_c); // Update the display
    }
  } else {*/
    // Check if button is pressed to start the countdown
    if (digitalRead(START_BUTTON) == HIGH) {
      debug("timer started");
      // The countdown is started here and anything that needs to be initialized should be put in here
      countdownRunning = true;
      time_c = time;
      startEnlarger();
      previousMillis = millis(); // Start the countdown
    }

    if (digitalRead(FOCUS_BUTTON) == HIGH){

      debug("focus button pressed");
      toggleEnlarger();
    }

    // Handle buttons 
    // Adding minimum delay to avoid registering the same press multiple times
    if (currentMillis - lastButtonPress > buttonDelay){
      
      if (digitalRead(SUB_SEC) == HIGH && time>0) {
        lastButtonPress = currentMillis;
        time--;
        displayTime(time);
      }
      if (digitalRead(ADD_SEC) == HIGH && time<9999){
        lastButtonPress = currentMillis;
        time++;
        displayTime(time);
      }
      if (digitalRead(SUB_STOP) == HIGH && time>0){
        lastButtonPress = currentMillis;
        time/=2;
        displayTime(time);
      }
      if (digitalRead(ADD_STOP) == HIGH && time<4999){
        lastButtonPress = currentMillis;
        time*=2;
        displayTime(time);
      }
    }
  //}

}


/* 
void displayTime(int x) {
  tm.display(0, (x / 1000) % 10);
  tm.display(1, (x / 100) % 10);
  tm.display(2, (x / 10) % 10);
  tm.display(3, x % 10);
}
*/

void displayTime(int x) {
  int minutes = x / 60;
  int seconds = x % 60;

  int m1 = minutes / 10;
  int m2 = minutes % 10;
  int s1 = seconds / 10;
  int s2 = seconds % 10;

  // Example: 03:45 → m1=0, m2=3, s1=4, s2=5
  // We'll display it on digits 7 to 4 (left side of the display)

  lc.setDigit(0, 7, m1, false);            // Minutes tens
  lc.setDigit(0, 6, m2, true);             // Minutes units + colon (using decimal point)
  lc.setDigit(0, 5, s1, false);            // Seconds tens
  lc.setDigit(0, 4, s2, false);            // Seconds units

  // Optional: Clear remaining digits (3 to 0)
  for (int i = 0; i < 4; i++) {
    lc.setChar(0, i, ' ', false);          // Blank
  }
}

void startEnlarger(){
  digitalWrite(RELAY_PIN, HIGH);
}

void stopEnlarger(){
  digitalWrite(RELAY_PIN, LOW);
}

void toggleEnlarger(){
  if(digitalRead(RELAY_PIN) == HIGH){
    digitalWrite(RELAY_PIN, LOW);
  } else {
    digitalWrite(RELAY_PIN, HIGH);
  }
}