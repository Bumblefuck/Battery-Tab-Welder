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

const int weldPin = 2;      // pin 11 on uview  select the pin for the weld solenoid coil
const int buttonPin = 5; // pin 13 on uview HEY! I HAVE IDEA! WHY NOT MAKE PIN 5 IN IDE PIN 5 ON uVIEW?

const int ROTATE_INTERVAL = 800; // milliseconds to show each sensor on the display

struct Range {
  int min;
  int max;
};

// holds all the relevant data for a given sensor
struct Sensor {
  int pin;                   // where it gets its input value
  Range range;               // the range it represents
  MicroViewWidget* widget;   // the widget that displays it
  int value;                 // its current value
  String units;              // the units (for display)
}

// sensor IDs - the indexes into the sensor array
const PULSE1 = 0;
const DELAY1 = 1;
const PULSE2 = 2;

// the sensor array
const NUM_SENSORS = 3;
Sensor sensor[NUM_SENSORS];
int lastDisplayedSensor = -1;

// Shorthand for defining widgets -- they share many of the same parameters
MicroViewWidget* makeSlider(Range range) {
    return new MicroViewSlider(10, 0,                 // position of slider
                               range.min, range.max,  // low value, high value
                               WIDGETSTYLE3);         // type of gauge displayed: vertical star trek sick bay (the original) bar graph with triangle pointer
}

void setup() {
    // initialize pins
    pinMode(buttonPin, INPUT);
    pinMode(weldPin, OUTPUT); // setting up the pins as innies or outies
    digitalWrite(weldPin, LOW); // this is just to make sure the weldPin is LOW before anything gets started

    // initialize uView
    uView.begin(); // this is a function in the uview library that initializes teh uview
    uView.clear(PAGE); // this clears the uview display

    // define all the sensor parameters by name
    sensor[PULSE1].pin = A0;          // pin 7 on uview
    sensor[PULSE1].range.min = 50;
    sensor[PULSE1].range.max = 1000;
    sensor[PULSE1].units = "ms1";

    sensor[DELAY1].pin = A1;          // pin 6  on uview
    sensor[DELAY1].range.min = 10;
    sensor[DELAY1].range.max = 1000;
    sensor[DELAY1].units = "ms 0";

    sensor[PULSE2].pin = A2;          // pin 5 on uview
    sensor[PULSE2].range.min = 50;
    sensor[PULSE2].range.max = 1000;
    sensor[PULSE2].units = "ms2";

    // initialize sensors
    for (int i = 0; i < NUM_SENSORS; ++i) {
        pinMode(sensor[i].pin, INPUT);                       // set up pin
        sensor[i].widget = makeSlider(sensor[i].range);  // set up widget
    }
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

// get the analog input (potentiometer input) from a pin and scale it to the desired range
void getScaledInput(int pin, Range range) {
    int rawValue = analogRead(pin);
    return map(rawValue, 0, 1023, range.min, range.max);
}

void rotateSensorDisplay() {
    int whoseTurn = int(millis() / ROTATE_INTERVAL) % NUM_SENSORS;  // modular arithmetic, tied to the clock

    // if this turn is different than the last one, redraw the screen
    if (lastDisplayedSensor != whoseTurn) {
        uView.clear(PAGE);
        sensor.widget ->reDraw();
        lastDisplayedSensor = whoseTurn;
    }

    updateSensorDisplay(sensor[whoseTurn]);
}

// update the display of one sensor
void updateSensorDisplay(Sensor sensor) {
    widget->setValue(sensor.value); // sets the cursor on the bar graph
    uView.display();   // dunno what this does but it's in every sketch with the microview. it must enable the write of the previous instruction or similar
    uView.print(sensor.units);  // this is to add units (milliseconds) to the bar graph for off time
    uView.display();
}


// FIRE THE PIXIES
// this is where the welding happens
void spotWeld(int pulse1Time, int delay1Time, int pulse2Time) {
    digitalWrite(weldPin, HIGH); // energize the small 5V PCB solenoid to fire the big 12V automotive solenoid.
    delay(pulse1Time);           // leaves the weldPin energized for the length of time set on the potentiometer
    digitalWrite(weldPin, LOW);  // turns the weldPin off
    delay(delay1Time);           // Off time delay between initial pulse and second pulse
    digitalWrite(weldPin, HIGH); // energize the small 5V PCB solenoid to fire the big 12V automotive solenoid.
    delay(pulse2Time);           // leaves the weldPin energized for the length of time set on the potentiometer
    digitalWrite(weldPin, LOW);  // turns the weldPin off
}

void loop() {
    // read the value from the sensors
    int inputValue[NUM_WIDGETS];
    for (int i = 0; i < NUM_WIDGETS; ++i) {
        sensor[i].value = getScaledInput(sensor[i].pin, sensor[i].range);
    }

    // display the values, round-robin style
    rotateSensorDisplay();

    int energize = digitalRead (buttonPin); // checks to see if the weld button is pressed and sets the value of energize to LOW or HIGH
    if (energize == HIGH) { // we checked the buttonPin previously and set energize to that value; if it's HIGH it'll start the weld sequence
        displayCountdown(3);
        spotWeld(sensor[PULSE1].value, sensor[DELAY1].value, sensor[PULSE2].value);
        lastDisplayedSensor = -1; // indicate that we need to redraw the whole screen -- none of the sensor indexes match -1
        delay (2);
    }

}
