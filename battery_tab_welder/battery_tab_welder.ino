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
const int sensorPin = A0; // pin 7 on uview select the input pin for the potentiometer
const int weldPin = 2; // pin 11 on uview  select the pin for the LED
int sensorValue = 0; // variable to store the value coming from the potentiometer
const int buttonPin = 5; // pin 13 on uview
int energize = 0; // variable to store the value of the buttonPin that starts the weld pulse sequence

MicroViewWidget * vWidget2; // this is a vertical star trek sick bay (the original) bar graph with triangle pointer

void setup() {
    uView.begin();
    uView.clear(PAGE);
    pinMode(weldPin, OUTPUT);
    pinMode(buttonPin, INPUT);
    digitalWrite(weldPin, LOW); // this is just to make sure the weldPin is LOW before anything gets started
    vWidget2 = new MicroViewSlider(10, 0, 200, 2000, WIDGETSTYLE3); // this sets the position of the slider (horizontal, vertical, low value, high value)
}

void loop() {
    // read the value from the sensor:
    sensorValue = analogRead(sensorPin); // this is the pot to control the pulse time in milliseconds

    sensorValue = map(sensorValue, 0, 1023, 200, 2000); // this converts the analog input value, into the 200 to 2000 milliseconds we'll use for the pulse time. It can be set as long or short as you need. But you'll need to change the microviewslider value to match
    // turn the ledPin off:

    energize = digitalRead(buttonPin); // checks to see if the weld button is pressed and sets the value of energize to LOW or HIGH

    vWidget2->setValue(sensorValue); // sets the cursor on the bar graph to whatever the pot is reading after conversion to milliseconds
    uView.display(); // dunno what this does but it's in every sketch with the microview. it must enable the write of the previous instruction or similar
    uView.print(" ms"); // this is to add units (milliseconds) to the bar graph
    uView.display();

    // we checked the buttonPin previously and set energize to that value; if it's HIGH it'll start the weld sequence
    if (energize == HIGH) {
        uView.clear(PAGE);
        delay(2); // clear page

        // countdown
        uView.print(" 3");
        uView.display();
        delay(500);

        // countdown
        uView.print(" 2");
        uView.display();
        delay(500);

        // countdown
        uView.print(" 1");
        uView.display();
        delay(500);

        uView.print("        FIRE!"); // SPARKS, YIKES!
        uView.display();

        delay(5);
        digitalWrite(weldPin, HIGH); // energize the small 5V PCB solenoid to fire the big 12V automotive solenoid.

        delay(sensorValue); // leaves the weldPin energized for the length of time set on the potentiometer
        digitalWrite(weldPin, LOW); // turns the weldPin off

        delay(5); // makes sure everything has time to do what it is supposed to

        uView.clear(PAGE);
        delay(2);
        vWidget2->reDraw(); // this re-draws the bar gauge, otherwise it acts weird
    }
}
