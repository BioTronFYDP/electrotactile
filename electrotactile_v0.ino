// Pulse control code for electrotactile stimulation
// code written for Teensy LC
#include "Timer.h"
 
// Pin 13 has an LED connected on most Arduino boards.
// Pin 11 has the LED on Teensy 2.0
// Pin 6  has the LED on Teensy++ 2.0
// Pin 13 has the LED on Teensy 3.0 and LC

// pins for switching control
// names are for arbitrary forwards, backwards directions
const int fwds = 12;
const int bwds = 11;
// only 1 pin for DAC output
const int alvl = 26;
const int vsense = 14;

// 
const int freq = 10;

// delays for pulse control
int mult = 2;
const int anmag = 20*mult; // 22 is properly balanced
const int catmag = 410*mult;

const int catwidth = 600;
int anwidth = 0;

int msCatPeriod = 83;
int msAnPeriod = 167;

const int uspulseoff = 313;

//sensed voltage
int vlvl = 0;

// timer for turning on the anodic pulse
// for charge balance
Timer t;

// the setup routine runs once when you press reset:
void setup() {                
  Serial.begin(9600);

  analogReference(DEFAULT);
  
  // initialize the digital pin as an output.
  pinMode( fwds, OUTPUT);
  pinMode( bwds, OUTPUT);
  analogWriteResolution(12);

  // Assuming anodic and cathodic
  // one pulse after the other,
  // at the same frequency of stimulation
  msCatPeriod = int( 1000 / freq);
  msAnPeriod = int( 1000 / freq);
  
  // anodic pulse width in microseconds
  anwidth = int( catmag * catwidth / anmag);
  
  // create two timing "tasks"
  int catpulse = t.every( msCatPeriod, cathodicPulse);
  int anpulse = t.every(  msAnPeriod, anodicPulse);
}

// Update scheduler ("scheduler") inside loop
void loop() {
  t.update();  
    //analogWrite( alvl, anmag); 
}

// turns the cathodic pulse on and then
// delay until turning it off
// reasoning: short delay for pulses
// so can hold up inside function
void cathodicPulse()
{
  analogWrite( alvl, catmag);
  digitalWrite( fwds, HIGH);
  delayMicroseconds( catwidth);
  digitalWrite( fwds, LOW);
  // turn analog level control to 0 to save power
  analogWrite( alvl, 0);

  // print out the sensed voltage
  vlvl = analogRead(vsense);
  Serial.print("Cathodic voltage: ");
  Serial.println(vlvl);

  delayMicroseconds( 100);
  
}

// turns the anodic pulse on and then
// delay until turning it off
// reasoning: short delay for pulses
// so can hold up inside function
void anodicPulse()
{
  analogWrite( alvl, anmag);
  digitalWrite( bwds, HIGH);
  delayMicroseconds( anwidth);
  digitalWrite( bwds, LOW);
  // turn analog level control to 0 to save power
  analogWrite( alvl, 0);

  // print out the sensed voltage
  vlvl = analogRead(vsense);
  Serial.print("Anodic voltage: ");
  Serial.println(vlvl);
}

