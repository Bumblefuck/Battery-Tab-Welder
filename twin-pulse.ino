/*
	MicroView Arduino Library
	Copyright (C) 2014 GeekAmmo
 MicroView Twin Pulse Battery Tab Welder
 CopyLEFT (use and abuse as you see fit) 2016 AvE
 This is an Arduino sketch for use with a battery tab / spot welder
 it uses the microview from sparkfun. The microview is a ATTiny arduino-esq board with a built in OLED display.
 tested and working on microview from sparkfun and programmer from sparkfun and Arduino IDE 1.6.8
 Noise from solenoids sometimes cause uview to hang. Use a momentary "dead man" switch as precaution.
 Or not. Be sure to get vid of resulting fire.
*/
#include <MicroView.h>
const int T7sensorPin = A0; // Pin 7 on uview the input pin for the primary on time pulse potentiometer
const int D6sensorPin = A1; // Pin 6  on uview the input pin for the off delay potentiometer
const int T5sensorPin = A2; // Pin 5 on uview the input pin for the primary on time pulse potentiometer

const int weldPin = 2;   // Pin 11 on uview  select the pin for the weld solenoid coil
int T7sensorValue = 0;   // Variable to store the value coming from on time primary pulse potentiometer
int D6sensorValue = 0;   // Variable to store the value coming from off delay potentiometer
int T5sensorValue = 0;   // Variable to store the value coming from on time secondary pulse potentiometer
const int buttonPin = 5; // Pin 13 on uview HEY! I HAVE IDEA! WHY NOT MAKE PIN 5 IN IDE PIN 5 ON uVIEW?
int energize = 0;        // Variable to store the value of the buttonPin that starts the weld pulse sequence

MicroViewWidget *vWidgetT7; // A vertical star trek sick bay (the original) bar graph with triangle pointer
MicroViewWidget *vWidgetD6;
MicroViewWidget *vWidgetT5;

void setup() {
    uView.begin();     // A function in the uview library that initializes the uview
    uView.clear(PAGE); // Clears the uview display
    pinMode(weldPin, OUTPUT);
    pinMode(buttonPin, INPUT);
    pinMode(T7sensorPin, INPUT);
    pinMode(D6sensorPin, INPUT);
    pinMode(T5sensorPin, INPUT);

    digitalWrite(weldPin, LOW); // Make sure the weldPin is LOW before anything gets started
	vWidgetT7 = new MicroViewSlider(10, 0, 50, 1000, WIDGETSTYLE3); // Sets the position of the slider (horizontal, vertical, low value, high value)
    vWidgetD6 = new MicroViewSlider(10, 0, 10,  500, WIDGETSTYLE3); // And the type of gauge displayed
    vWidgetT5 = new MicroViewSlider(10, 0, 50, 1000, WIDGETSTYLE3);
}

void loop() {
    T7sensorValue = analogRead(T7sensorPin); // Read the pot to control the pulse time (ms)

    // Converts the analog input value into the 50 - 1000 ms for the first on pulse time.
    // It can be set as long or short as you need, but you'll need to change the microviewslider value to match.
    T7sensorValue = map(T7sensorValue, 0, 1023, 50, 1000);


    T5sensorValue = analogRead(T5sensorPin); // Read the pot to control the pulse time (ms)

    // Converts the analog input value into the 50 - 1000 ms for the second on pulse time.
    // It can be set as long or short as you need, but you'll need to change the microviewslider value to match.
    T5sensorValue = map(T5sensorValue, 0, 1023, 50, 1000);


    D6sensorValue = analogRead(D6sensorPin); // Read the pot to control the pulse time (ms)

    // Converts the analog input value into the 10 - 1000 ms for the time off delay between pulses.
    // It can be set as long or short as you need, but you'll need to change the microviewslider value to match.
    D6sensorValue = map(D6sensorValue, 0, 1023, 10, 1000);


    // Checks to see if the weld button is pressed and sets the value of energize to LOW or HIGH.
    // You need to press it for a few seconds while the microcontroller is distracted doing other stuff
    // because of the n00b "delay" tactics below. This could be instant with the use of interupts, but CHOOCHUS INTERUPTUS.
    energize = digitalRead(buttonPin);

    vWidgetT7->setValue(T7sensorValue); // Sets the cursor on the bar graph to whatever the pot is reading after conversion to ms
    vWidgetT7->reDraw(); // Re-draws the bar gauge, otherwise it acts weird
    uView.print(" ms1"); // To add units (ms) to the bar graph
    uView.display();     // Display current page buffer
    delay(800);          // How long to dispay the bar graph prior to dispaying next value
    uView.clear(PAGE);   // Clear current page buffer
    delay(2);

    vWidgetD6->setValue(D6sensorValue);
    vWidgetD6->reDraw();
    uView.print(" ms O");
    uView.display();
    delay(800);
    uView.clear(PAGE);
    delay(2);

    vWidgetT5->setValue(T5sensorValue);
    vWidgetT5->reDraw();
    uView.print(" ms2");
    uView.display();
    delay(800);
    uView.clear(PAGE);
    delay(2);


    if (energize == HIGH) {
        // We checked the buttonPin previously and set energize to that value; if it's HIGH it'll start the weld sequence

        uView.clear(PAGE); // Clear page
        delay(2);
        uView.print(" 3");  // Countdown
        uView.display();
        delay(500);

        uView.print(" 2");  // Countdown
        uView.display();

        delay(500);

        uView.print(" 1");  // Countdown
        uView.display();
        delay(500);

        uView.print("        FIRE!");  // SPARKS, YIKES!
        uView.display();

        delay(5);

        // This is where the welding happens:
        digitalWrite(weldPin, HIGH); // Energize the small 5V PCB solenoid to fire the big 12V automotive solenoid.
        delay(T7sensorValue);        // Leaves the weldPin energized for the length of time set on the potentiometer
        digitalWrite(weldPin, LOW);  // Turns the weldPin off
        delay(D6sensorValue);        // Off time delay between initial pulse and second pulse
        digitalWrite(weldPin, HIGH);
        delay(T5sensorValue);        // Leaves the weldPin energized for the length of time set on the potentiometer
        digitalWrite(weldPin, LOW);

        uView.clear(PAGE);
        delay(2);
    }
}
