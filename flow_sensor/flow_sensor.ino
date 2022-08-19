// UTFT_Demo_480x320 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of how to use most of the functions
// of the library with a supported display modules.
//
// This demo was made for modules with a screen resolution 
// of 480x320 pixels.
//
// This program requires the UTFT library.
//

#include <UTFT.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];

// Set the pins to the correct ones for your development shield
// ------------------------------------------------------------
// Arduino Uno / 2009:
// -------------------
// Standard Arduino Uno/2009 shield            : <display model>,A5,A4,A3,A2
// DisplayModule Arduino Uno TFT shield        : <display model>,A5,A4,A3,A2
//
// Arduino Mega:
// -------------------
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Mega      : <display model>,38,39,40,41
//
// Remember to change the model parameter to suit your display module!

volatile int flow_frequency; // Measures flow sensor pulses

// Calculated litres/hour

float one_minute = 60 * 1000;
float vol = 0.0;
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;


UTFT myGLCD(ILI9486,38,39,40,41);

void flow () // Interrupt function
{
   flow_frequency++;
}


void setup()
{
  randomSeed(analogRead(0));
  
// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(BigFont);

// Setup the flow sensor
  pinMode(flowsensor, INPUT);
  digitalWrite(flowsensor, HIGH); 
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt
  myGLCD.clrScr();
  currentTime = millis();
  cloopTime = currentTime;

}

void loop()
{
  int buf[478];
  int x, x2;
  int y, y2;
  int r;

// Clear the screen and draw the frame
  myGLCD.print("* TEST *", CENTER, 50);
  myGLCD.setBackColor(64, 64, 64);
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 479, 13);
  myGLCD.setColor(64, 64, 64);
  myGLCD.fillRect(0, 306, 479, 319);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.setBackColor(64, 64, 64);
  myGLCD.setColor(255,255,0);
  myGLCD.setColor(0, 0, 255);
  myGLCD.drawRect(0, 14, 479, 305);
  myGLCD.print("Water Flow Meter", CENTER, 1);
  myGLCD.print("Circuit Digest", CENTER, 307);

 currentTime = millis();
 if(currentTime >= (cloopTime+1000))
 {
  cloopTime = currentTime; // Updates cloopTime
  if(flow_frequency != 0){

    // Pulse frequency (hz) = 7.5Q, q is a flow rate in L/min
    one_minute = (flow_frequency / 7.5); // (Pulse Frequency x 60min) / 7.5Q = flow rate in L/hour
    myGLCD.clrScr();
    myGLCD.print("Rate: ", LEFT, 80);
    
    myGLCD.printNumF(one_minute, 3, CENTER, 80);
    
    myGLCD.print("L/M", RIGHT, 80);
    
    one_minute = one_minute / 60;
    vol = vol + one_minute;
    
    myGLCD.print("Vol:", LEFT, 150);
    
    myGLCD.printNumF(vol, 3, CENTER, 150);
    
    myGLCD.print("L", RIGHT, 120);
    
    flow_frequency = 0; // Reset counter
    Serial.print(one_minute, DEC); // Print litres/hour
    Serial.println(" L/sec");
  }
  else{
    Serial.println("flow rate = 0");
    myGLCD.clrScr();
    myGLCD.print("Rate: ", LEFT, 80);
    myGLCD.printNumF(flow_frequency, 3, CENTER, 80);
    myGLCD.print(" L/M", RIGHT, 80);
    myGLCD.print("Vol: ", LEFT, 120);
    myGLCD.printNumF(vol, 3, CENTER, 120);
    myGLCD.print("Liters", RIGHT, 120); 
  }
 }
}
