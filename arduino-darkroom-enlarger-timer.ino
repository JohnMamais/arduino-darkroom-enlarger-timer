/*
* This project is part of a DIY darkroom enlarger timer.
* It is licensed under the GNU General Public License V3 License.
*
* Copyright (c) 2025 Ioannis Mamais
*
*/

#include "LedControl.h"
#include <Wire.h>


#define DEBUG 1

#if DEBUG
  #define debug(x) Serial.print(x)
  #define debugln(x) Serial.println(x)
#else
  #define debug(x)
  #define debugln(x)
#endif


const int START_BUTTON = 11;
const int FOCUS_BUTTON = 12;

const int RELAY_PIN = 2;

const int SUB_STOP = 3;
const int SUB_SEC = 4;
const int ADD_SEC = 7;
const int ADD_STOP = 8;

const int LOAD = 10; //CS
const int DATA_IN = 9;
const int CLK = 6;

// init the 8-digit display
// dat, clk, load, num of MAX72xx modules
LedControl lc=LedControl(DATA_IN, CLK, LOAD, 1);

unsigned int time = 10;

// time_c is for the countdown time, keeping the original time intact for reference
unsigned int time_c;

unsigned long buttonDelay = 300; // Minimum delay between button presses (in milliseconds)
unsigned long lastButtonPress = 0; // Store the last time a button was pressed
unsigned long currentMillis = 0; // current time

unsigned long previousMillis = 0; // Will store the last time the countdown updated
bool countdownRunning = false; // Flag to track whether countdown is running

void setup() {

  pinMode(RELAY_PIN, OUTPUT);

  pinMode(START_BUTTON, INPUT_PULLUP);
  pinMode(FOCUS_BUTTON, INPUT_PULLUP);

  pinMode(SUB_STOP, INPUT_PULLUP);
  pinMode(ADD_STOP, INPUT_PULLUP);
  pinMode(SUB_SEC, INPUT_PULLUP);
  pinMode(ADD_SEC, INPUT_PULLUP);

  
  
  // The MAX72XX is in power-saving mode on startup,
  // we have to do a wakeup call
  
  lc.shutdown(0,false);
  // Set the brightness to lowest setting to avoid burning your paper 
  lc.setIntensity(0,0);
  // and clear the display 
  lc.clearDisplay(0);

  displayTime(time);

  Serial.begin(9600);
  debug("Setup complete");
}

void loop() {
  
  currentMillis = millis();

  // Prioritize countdown
  // If countdown is running, update the display every second
  if (countdownRunning) {
    
    // countdown check
    if (time_c < 1 || time_c > time) { // time_c > time is usefull to account for overflow since time variables are unsigned
      countdownRunning = false; // Stop the countdown when it reaches 0
      
      // turn off enlarger head
      stopEnlarger();

      // start debounce timer
      debounceStart();

      // reset the displayed time
      time_c = time;
    }

    // Only update display clock every second
    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
      time_c--;
      displayTime(time_c); // Update the display
    }

  } else {
    
    // Button handling

    // Adding minimum delay to avoid registering the same press multiple times
    if (debounceOk()){

      // Check start button
      if (digitalRead(START_BUTTON) == LOW && time>0) {
        // The countdown is started here and anything that needs to be initialized should be put in here
        debounceStart()
        debugln("timer started");
        
        countdownRunning = true; // start countdown
        time_c = time;
        startEnlarger();
        previousMillis = millis(); 
      }

      // Check focus button
      if (digitalRead(FOCUS_BUTTON) == LOW){
        debounceStart()

        debug("focus button pressed");
        toggleEnlarger();
      }
      
      // Check time buttons
      if (digitalRead(SUB_SEC) == LOW && time>0) {
        debounceStart();
        time--;
        displayTime(time);
      }
      if (digitalRead(ADD_SEC) == LOW && time<9999){
        debounceStart();
        time++;
        displayTime(time);
      }
      if (digitalRead(SUB_STOP) == LOW && time>0){
        debounceStart();
        time/=2;
        displayTime(time);
      }
      if (digitalRead(ADD_STOP) == LOW && time<4999){
        debounceStart();
        time*=2;
        displayTime(time);
      }
    }

  }

}

void displayTime(int x) {

  debug("time remaining: ");
  debugln(x);

  lc.clearDisplay(0);

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

void debounceStart(){
  lastButtonPress = currentMillis;
}

bool debounceOk(){
  return currentMillis - lastButtonPress > buttonDelay;
}

void startEnlarger(){
  digitalWrite(RELAY_PIN, HIGH);
}

void stopEnlarger(){
  digitalWrite(RELAY_PIN, LOW);
}

void toggleEnlarger(){
  // oneliner toggle
  digitalWrite(RELAY_PIN, !digitalRead(RELAY_PIN));
}