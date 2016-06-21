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
const int T7sensorPin = A0; // pin 7 on uview the input pin for the primary on time pulse potentiometer
const int D6sensorPin = A1; // pin 6  on uview the input pin for the off delay potentiometer
const int T5sensorPin = A2; // pin 5 on uview the input pin for the primary on time pulse potentiometer

const int weldPin = 2;      // pin 11 on uview  select the pin for the weld solenoid coil
int T7sensorValue = 0;  // variable to store the value coming from on time primary pulse potentiometer
int D6sensorValue = 0;  // variable to store the value coming from off delay potentiometer
int T5sensorValue = 0;  // variable to store the value coming from on time secondary pulse potentiometer
const int buttonPin = 5; // pin 13 on uview HEY! I HAVE IDEA! WHY NOT MAKE PIN 5 IN IDE PIN 5 ON uVIEW?
int energize = 0; // variable to store the value of the buttonPin that starts the weld pulse sequence

MicroViewWidget *vWidgetT7; // this is a vertical star trek sick bay (the original) bar graph with triangle pointer
MicroViewWidget *vWidgetD6;
MicroViewWidget *vWidgetT5;

void setup() {
uView.begin(); // this is a function in the uview library that initializes teh uview
uView.clear(PAGE); // this clears the uview display
pinMode(weldPin, OUTPUT); // setting up the pins as innies or outies 
pinMode(buttonPin, INPUT);
pinMode (T7sensorPin, INPUT);
pinMode(D6sensorPin, INPUT);
pinMode(T5sensorPin, INPUT);

digitalWrite(weldPin, LOW); // this is just to make sure the weldPin is LOW before anything gets started
		vWidgetT7 = new MicroViewSlider(10, 0, 50, 1000, WIDGETSTYLE3); // this sets the position of the slider (horizontal, vertical, low value, high value)
    vWidgetD6 = new MicroViewSlider(10, 0, 10, 500, WIDGETSTYLE3); // and the type of gauge displayed
    vWidgetT5 = new MicroViewSlider(10, 0, 50, 1000, WIDGETSTYLE3); 

void loop() {
   // read the value from the sensors:
T7sensorValue = analogRead(T7sensorPin); // this is the pot to control the pulse time in milliseconds
T7sensorValue = map(T7sensorValue, 0, 1023, 50, 1000); // this converts the analog input value, into the 
//50 to 1000 milliseconds we'll use for the first on pulse time. It can be set as long or short as you 
//need. But you'll need to change the microviewslider value to match

// read the value from the sensors:
T5sensorValue = analogRead(T5sensorPin); // this is the pot to control the pulse time in milliseconds
T5sensorValue = map(T5sensorValue, 0, 1023, 50, 1000); // this converts the analog input value, into the 50 to 1000 milliseconds we'll use 
//for the second on pulse time. It can be set as long or short as you need. But you'll need to change the microviewslider value to match

D6sensorValue = analogRead(D6sensorPin); // this is the pot to control the pulse time in milliseconds
D6sensorValue = map(D6sensorValue, 0, 1023, 10, 1000); // this converts the analog input value, into the 
//10 to 1000 milliseconds we'll use for time off delay between pulses. It can be set as long or short as you need.
//But you'll need to change the microviewslider value to match
 
energize = digitalRead (buttonPin); // checks to see if the weld button is pressed and sets the value of energize to LOW or HIGH 
// you need to press it for a few seconds while the microcontroller is distracted doing other stuff. Because of the n00b "delay" 
// tacticts below. This could be instant with the use of interupts, but, CHOOCHUS INTERUPTUS.
		
vWidgetT7->setValue(T7sensorValue); // sets the cursor on the bar graph to whatever the pot is reading after conversion to milliseconds
vWidgetT7 ->reDraw(); // this re-draws the bar gauge, otherwise it acts weird
uView.display(); // dunno what this does but it's in every sketch with the microview. it must enable the write of the previous instruction or similar
uView.print(" ms1");  // this is to add units (milliseconds) to the bar graph for pulse 1
uView.display();
  delay (800); // this is how long to dispay the bar graph prior to dispaying next value
    uView.clear(PAGE); 
       delay (2);
 
 vWidgetD6->setValue(D6sensorValue); // sets the cursor on the bar graph to whatever the pot is reading after conversion to milliseconds
  vWidgetD6 ->reDraw();
 uView.display(); // dunno what this does but it's in every sketch with the microview. it must enable the write of the previous instruction or similar
    uView.print(" ms O");  // this is to add units (milliseconds) to the bar graph for off time 
  uView.display();
  delay (800);
    uView.clear(PAGE); 
       delay (2);
       
   vWidgetT5->setValue(T5sensorValue); // sets the cursor on the bar graph to whatever the pot is reading after conversion to milliseconds
    vWidgetT5 ->reDraw();
    uView.display(); // dunno what this does but it's in every sketch with the microview. it must enable the write of the previous instruction or similar
    uView.print(" ms2");  // this is to add units (milliseconds) to the bar graph for pulse 2
  uView.display();
  delay (800);
    uView.clear(PAGE); 
       delay (2);


  if (energize == HIGH) { // we checked the buttonPin previously and set energize to that value; if it's HIGH it'll start the weld sequence
   uView.clear(PAGE); 
     delay (2);// clear page
  uView.print(" 3");  // countdown
  uView.display();
  delay (500);

  uView.print(" 2");  // countdown
  uView.display();
 
  delay (500);

  uView.print(" 1");  // countdown
  uView.display();
  delay (500);

  uView.print("        FIRE!");  // SPARKS, YIKES!
  uView.display();

  delay(5); 
  
  // this is where the welding happens:
      digitalWrite (weldPin, HIGH); // energize the small 5V PCB solenoid to fire the big 12V automotive solenoid.
      delay (T7sensorValue); // leaves the weldPin energized for the length of time set on the potentiometer
      digitalWrite (weldPin, LOW); // turns the weldPin off
      delay (D6sensorValue); // Off time delay between initial pulse and second pulse
      digitalWrite (weldPin, HIGH); // energize the small 5V PCB solenoid to fire the big 12V automotive solenoid.
      delay (T5sensorValue); // leaves the weldPin energized for the length of time set on the potentiometer
      digitalWrite (weldPin, LOW); // turns the weldPin off
      
       uView.clear(PAGE); 
       delay (2);
     
    }


	
}
