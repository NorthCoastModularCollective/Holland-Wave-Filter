#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=81,234

// mixers pre-filter (input goes directly here) - used for attenuating signal before filtering
AudioMixer4              mixer4;         //xy=173,338
AudioMixer4              mixer3;         //xy=174,134

AudioFilterStateVariable filter1;        //xy=303,81
AudioFilterStateVariable filter6;        //xy=302,392
AudioFilterStateVariable filter3;        //xy=303,187
AudioFilterStateVariable filter5;        //xy=303,342
AudioFilterStateVariable filter2;        //xy=304,132
AudioFilterStateVariable filter4;        //xy=305,292
AudioMixer4              mixer1;         //xy=452,135
AudioMixer4              mixer2;         //xy=452,339
AudioOutputI2S           i2s2;           //xy=627,232
AudioConnection          patchCord1(i2s1, 0, mixer3, 0);
AudioConnection          patchCord2(i2s1, 1, mixer4, 0);
AudioConnection          patchCord3(mixer4, 0, filter4, 0);
AudioConnection          patchCord4(mixer4, 0, filter5, 0);
AudioConnection          patchCord5(mixer4, 0, filter6, 0);
AudioConnection          patchCord6(mixer3, 0, filter1, 0);
AudioConnection          patchCord7(mixer3, 0, filter2, 0);
AudioConnection          patchCord8(mixer3, 0, filter3, 0);
AudioConnection          patchCord9(filter1, 1, mixer1, 0);
AudioConnection          patchCord10(filter6, 1, mixer2, 2);
AudioConnection          patchCord11(filter3, 1, mixer1, 2);
AudioConnection          patchCord12(filter5, 1, mixer2, 1);
AudioConnection          patchCord13(filter2, 1, mixer1, 1);
AudioConnection          patchCord14(filter4, 1, mixer2, 0);
AudioConnection          patchCord15(mixer1, 0, i2s2, 0);
AudioConnection          patchCord16(mixer2, 0, i2s2, 1);
// GUItool: end automatically generated code


AudioControlSGTL5000 codec;
//  Set up all the pin number variables
//trigger inputs
int Trigger1Pin = 5; 
int Trigger2Pin = 8; 
int Trigger3Pin = 28; 
//LEDs
int LEDA = 29;
int LEDB = 30;
int LEDC = 31;
int LEDD = 32;
int LEDE = 33;
int Trig1LED = 27;
int Trig2LED = 26;
int Trig3LED = 25;
//Switches
int SwitchA1 = 0;
int SwitchB1 = 1;
int SwitchC1 = 2;
int SwitchD1 = 3;
int SwitchE1 = 4;
//Potentiometers
int pA2 = A13;
int pB2 = A0;
int pC2 = A12;
int pD2 = A7;
int pE2 = A6;

int pA3 = A3; 
int pB3 = A11;
int pC3 = A10;
int pD3 = A1;
int pE3 = A2;

//variables for tracking the trigger states
int playTimes = 0; 
int playTimes2 = 0; 
int playTimes3 = 0;

float inputLevelL = 1.0; //Audio level appears to require a float. see if this can be an int!!!!!!!!!!!!!!
float inputLevelR = 1.0;

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  // Get the board ready
  AudioMemory(50);
  codec.enable();
  codec.volume(1); //crank it. We'll control volume per drum channel with the mixers.

  // Attenuate the input signal before it hits the filters
  mixer4.gain(0, 0.2);
  mixer3.gain(0, 0.2);

  //Now set up the filters  
  AudioNoInterrupts();

  //LEFT INPUT FILTERS
  filter1.frequency(440);
  filter2.frequency(880);
  filter3.frequency(1660);

  filter1.resonance(5.0);
  filter2.resonance(5.0);
  filter3.resonance(5.0);

  //RIGHT INPUT FILTERS
  filter4.frequency(440);
  filter5.frequency(880);
  filter6.frequency(1660);

  filter4.resonance(5.0);
  filter5.resonance(5.0);
  filter6.resonance(5.0);

  //Set up the mixers (for "Level" settings)
  mixer1.gain(0, 1); //just pass the audio through ("level" may be any floating point number from 0 to 32767)
  mixer2.gain(0, 1);

  //Set up Digital Pins
  //triggers
  pinMode(Trigger1Pin, INPUT); //D1 is a digital input
  pinMode(Trigger2Pin, INPUT); //D2 is a digital input
  pinMode(Trigger3Pin, INPUT); //D3 is a digital input

  //LEDs  
  pinMode(LEDA, OUTPUT); 
  pinMode(LEDB, OUTPUT); 
  pinMode(LEDC, OUTPUT); 
  pinMode(LEDD, OUTPUT);
  pinMode(LEDE, OUTPUT); 
  pinMode(Trig1LED, OUTPUT);
  pinMode(Trig2LED, OUTPUT);         
  pinMode(Trig3LED, OUTPUT); 

  //switches 
  pinMode(SwitchA1, INPUT_PULLUP); //Switch input with internal pullup resistor  
  pinMode(SwitchB1, INPUT_PULLUP); //Switch input with internal pullup resistor
  pinMode(SwitchC1, INPUT_PULLUP); //Switch input with internal pullup resistor
  pinMode(SwitchD1, INPUT_PULLUP); //Switch input with internal pullup resistor       
  pinMode(SwitchE1, INPUT_PULLUP); //Switch input with internal pullup resistor 

  AudioInterrupts();

  //Board ready, flash LEDs to indicate ready state on boot up
  digitalWrite(LEDA, HIGH); //animate cycling the LEDs on from left to right
  delay(100);              
  digitalWrite(LEDA, LOW);
  digitalWrite(LEDB, HIGH);
  delay(100);              
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDC, HIGH);
  delay(100);              
  digitalWrite(LEDC, LOW);
  digitalWrite(LEDD, HIGH);
  delay(100);              
  digitalWrite(LEDD, LOW);
  digitalWrite(LEDE, HIGH);
  delay(100);              
  digitalWrite(LEDE, LOW);
  digitalWrite(LEDD, HIGH);  //now animate cycling the LEDs on from right to left
  delay(100);              
  digitalWrite(LEDD, LOW);
  digitalWrite(LEDC, HIGH);
  delay(100);              
  digitalWrite(LEDC, LOW);
  digitalWrite(LEDB, HIGH);
  delay(100);              
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDA, HIGH);
  delay(100); 
  digitalWrite(LEDA, LOW);
  delay(100);             
  digitalWrite(LEDA, HIGH); //flash them all on
  digitalWrite(LEDB, HIGH);
  digitalWrite(LEDC, HIGH);
  digitalWrite(LEDD, HIGH);
  digitalWrite(LEDE, HIGH);
  delay(200);              
  digitalWrite(LEDA, LOW); //flash them all off
  digitalWrite(LEDB, LOW); 
  digitalWrite(LEDC, LOW); 
  digitalWrite(LEDD, LOW); 
  digitalWrite(LEDE, LOW);  
}

void loop() {
  //Get values from the knobs and pass them on to the filter parameters

  //LEFT FILTERS
  int filter1Freq = map(analogRead(pA2), 0, 1023, 20, 5000); 
  filter1.frequency(filter1Freq);

  float filter1Res = mapf(analogRead(pA3), 0, 1023, 0.7, 5.0); 
  filter1.resonance(filter1Res);
  filter2.resonance(filter1Res);
  filter3.resonance(filter1Res);
  Serial.println("filter resonance is: ");
  Serial.println(filter1Res);

  int filter2Freq = map(analogRead(pB2), 0, 1023, 20, 5000); 
  filter2.frequency(filter2Freq);

  int filter3Freq = map(analogRead(pC2), 0, 1023, 20, 5000); 
  filter3.frequency(filter3Freq);

  //RIGHT FILTERS
  //RIGHT NOW, THESE ARE MAPPED TO THE SAME KNOBS AS THE LEFT FILTERS!!!
  int filter4Freq = map(analogRead(pA2), 0, 1023, 20, 5000); 
  filter4.frequency(filter4Freq);

  int filter5Freq = map(analogRead(pB2), 0, 1023, 20, 5000); 
  filter5.frequency(filter5Freq);

  int filter6Freq = map(analogRead(pC2), 0, 1023, 20, 5000); 
  filter6.frequency(filter6Freq);

}
