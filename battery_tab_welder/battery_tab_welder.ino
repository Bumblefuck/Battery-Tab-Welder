/*
	MicroView Arduino Library
	Copyright (C) 2014 GeekAmmo
 MicroView Battery Tab Welder
 
 CopyLEFT (use and abuse as you see fit) 2016 AvE
 
 This is an Arduino sketch for use with a battery tab / spot welder
 it uses the microview from sparkfun. The microview is a ATTiny arduino-esq board with a built in OLED display.
 tested and working on microview from sparkfun and programmer from sparkfun and Arduino IDE 1.6.8
 Noise from solenoids sometimes cause uview to hang. Use a momentary "dead man" switch as precaution. Or not. Be sure to get vid of resulting fire.
*/
#include < MicroView.h >

// pins
const int sensorPin = A0; // pin 7 on uview - the input pin for the pulse time potentiometer
const int weldPin = 2;    // pin 11 on uview - the pin for the LED
const int buttonPin = 5;  // pin 13 on uview

// customize your welder
const int PULSE_MILLIS_MIN = 200;
const int PULSE_MILLIS_MAX = 2000;

// state variables
int sensorValue = 0;       // for the value coming from the potentiometer
MicroViewWidget* vWidget2; // this is a vertical star trek sick bay (the original) bar graph with triangle pointer

void displayPulseTime() {
    uView.clear(PAGE);
    delay(2);
    vWidget2->reDraw(); // this re-draws the bar gauge, otherwise it acts weird
}

void setup() {
    pinMode(weldPin, OUTPUT);
    pinMode(buttonPin, INPUT);
    digitalWrite(weldPin, LOW); // this is just to make sure the weldPin is LOW before anything gets started

    // set up uView
    uView.begin();
    vWidget2 = new MicroViewSlider(10,                // horizontal position
                                   0,                 // vertical position
                                   PULSE_MILLIS_MIN,  // low value
                                   PULSE_MILLIS_MAX,  // high value
                                   WIDGETSTYLE3); 
    displayPulseTime();
}

// read the value from the sensor and convert to the number of milliseconds we'll use for the pulse time. 
int getSensorValue() {
    int rawSensorValue = analogRead(sensorPin); 

    return map(rawSensorValue, 
               0, 1023,                               // input range
               PULSE_MILLIS_MIN, PULSE_MILLIS_MAX);   // output range

}

// update the OLED display with the desired pulse Time
void updatePulseTime(int milliseconds) {
    vWidget2->setValue(milliseconds); // sets the cursor on the bar graph
    uView.display();                  // enable the write of the previous instruction or similar
    uView.print(" ms");               // this is to add units (milliseconds) to the bar graph
    uView.display();
}

// Display a (delayed) 3....2....1... for the given number of steps
//    followed by "FIRE!"
void displayCountdown(int steps) {
    uView.clear(PAGE);    // remove what was on the screen before
    delay(2); 

    // display the countdown steps
    for (int i = steps; i > 0; --i) {
        uView.print(" " + String(i));
        uView.display();
        delay(500);    
    }

    uView.print("        FIRE!"); // SPARKS, YIKES!
    uView.display();
    delay(5);
}

// FIRE THE PIXIES
void spotWeld(int pulseTime) {
    digitalWrite(weldPin, HIGH);  // energize the small 5V PCB solenoid to fire the big 12V automotive solenoid.
    delay(pulseTime);             // leaves the weldPin energized for the length of time set on the potentiometer
    digitalWrite(weldPin, LOW);   // turns the weldPin off
    delay(5);                     // makes sure everything has time to do what it is supposed to
}

void loop() {
    // read and display the value from the sensor:
    sensorValue = getSensorValue();
    updatePulseTime(sensorValue);

    // turn the ledPin off:

    // if the weld button is pressed, start the weld sequence then go back to the pulse time display
    int energize = digitalRead(buttonPin); 
    if (energize == HIGH) {
        displayCountdown(3);
        spotWeld(sensorValue);
        displayPulseTime();
    }
}
