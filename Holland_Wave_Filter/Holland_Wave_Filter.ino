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

//LEFT FILTERS
AudioFilterStateVariable filter1;        //xy=303,81
AudioFilterStateVariable filter2;        //xy=304,132
AudioFilterStateVariable filter3;        //xy=303,187

//RIGHT FILTERS
AudioFilterStateVariable filter4;        //xy=305,292
AudioFilterStateVariable filter5;        //xy=303,342
AudioFilterStateVariable filter6;        //xy=302,392

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

//new code - adding LFO to filter
AudioSynthWaveformDc     dc1;            //xy=87,227
AudioSynthWaveformDc     dc2;            //xy=87,227
AudioSynthWaveformDc     dc3;            //xy=87,227
AudioSynthWaveformSine   sine1;          //xy=91,177
AudioSynthWaveformSine   sine2;          //xy=91,177
AudioSynthWaveformSine   sine3;          //xy=91,177

// these mixers are used for controlling the LFO amount on the filters
// mixer5 = LFO 1, mixer6 = LFO 2, mixer7 = LFO 3
AudioMixer4              mixer5;         //xy=345,196
AudioMixer4              mixer6;         //xy=345,196
AudioMixer4              mixer7;         //xy=345,196

// Filter 1 (both left & right)
AudioConnection          patchCord17(mixer5, 0, filter1, 1);
AudioConnection          patchCord18(mixer5, 0, filter4, 1);
AudioConnection          patchCord19(dc1, 0, mixer5, 1);
AudioConnection          patchCord20(sine1, 0, mixer5, 0);

// Filter 2 (both left & right)
AudioConnection          patchCord21(mixer6, 0, filter2, 1);
AudioConnection          patchCord22(mixer6, 0, filter5, 1);
AudioConnection          patchCord23(dc2, 0, mixer6, 1);
AudioConnection          patchCord24(sine2, 0, mixer6, 0);

// Filter 3 (both left & right)
AudioConnection          patchCord25(mixer7, 0, filter3, 1);
AudioConnection          patchCord26(mixer7, 0, filter6, 1);
AudioConnection          patchCord27(dc3, 0, mixer7, 1);
AudioConnection          patchCord28(sine3, 0, mixer7, 0);

AudioControlSGTL5000     sgtl5000_1;     //xy=257,491

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

// LFO frequencies
float lfo1Freq = 1.0; 
float lfo2Freq = 1.0;
float lfo3Freq = 1.0;

//variables for tracking the clock timing
int timeBetweenTriggers = 0; 
float triggersPerSecond = 0.0;
float timeBetweenInMillis = 0.0;
float timeBetweenInSeconds = 0.0;
float bpm = 0.0;
float hertz = 0.0;

float timeBetweenInMillis1 = 0.0;
float timeBetweenInMillis2 = 0.0;
float timeBetweenInMillis3 = 0.0;
float averageTimeBetweenInMillis = 0.0;

// variables to keep track of the clock rate, divided by knob position
int rateDivision = 0;
float dividedRate = 0.0;

float inputLevelL = 1.0; //Audio level appears to require a float. see if this can be an int!!!!!!!!!!!!!!
float inputLevelR = 1.0;

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  Serial.begin(9600);
  
  // Get the board ready
  AudioMemory(50);
  codec.enable();
  codec.volume(0.0); //mute it during setup
  AudioNoInterrupts();

  // Attenuate the input signal before it hits the filters
  mixer4.gain(0, 0.1);
  mixer3.gain(0, 0.1);

  //Set up the mixers (for "Level" settings)
  mixer1.gain(0, 1);
  mixer1.gain(1, 1);
  mixer1.gain(2, 1);

  mixer2.gain(0, 1);
  mixer2.gain(1, 1);
  mixer2.gain(2, 1);

  // These mixers combine the LFO and DC offsets for filter frequency
  mixer5.gain(0, 0.5); //Mixer5 
  mixer5.gain(1, 0.5);

  mixer6.gain(0, 0.5); //Mixer5 mixes the LFO and DC offset for filter frequency
  mixer6.gain(1, 0.5);

  mixer7.gain(0, 0.5); //Mixer5 mixes the LFO and DC offset for filter frequency
  mixer7.gain(1, 0.5);

  //we don't use channel 3 on these mixers. Drop it to zero.
  mixer1.gain(3, 0);
  mixer2.gain(3, 0);

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
  startupSequence();

  // unmute the audio output now that setup is done
  codec.volume(0.8); //crank it.
}

void loop() {

  setChannelLEDs();
  
  //Reset phase of LFOs based on trigger in T2
  if (digitalRead(Trigger2Pin) == LOW) 
  {
    sine1.phase(0);
    sine2.phase(0);
    sine3.phase(0);
  }
  
  //Start listening for clock pulses (gate inputs for clock signal); time out after 1 second (pulseIn takes 3rd argument in microseconds)
  timeBetweenTriggers = pulseIn(Trigger1Pin, HIGH, 2000000);

  // pulseIn will return 0 if no pulses received in 1 second
  // Check to see if we're getting clock input from Trigger1
  if (timeBetweenTriggers > 0) {
    timeBetweenInMillis = timeBetweenTriggers / 1000;
    timeBetweenInSeconds = timeBetweenInMillis / 1000;

    bpm = 60 / timeBetweenInSeconds;  
    hertz = 1 / timeBetweenInSeconds;  
    
    //Frequency rate division from knob E2
    rateDivision = map(analogRead(pE2), 0, 1023, 8, 1); 

    if(rateDivision == 1) {
        dividedRate = hertz * 8;
    } else {
        dividedRate = (hertz / pow(2, rateDivision) * 8);
    }

    //Set LFO frequencies based on clock tempo & knob position
    lfo1Freq = dividedRate; 
    lfo2Freq = dividedRate; 
    lfo3Freq = dividedRate;

    digitalWrite(Trig1LED, HIGH);
    delay(10);
    digitalWrite(Trig1LED, LOW);    

    //DEBUGGING - REMOVE BEFORE PRODUCTION USE
    /*
    Serial.print("Beats per minute: ");
    Serial.print(bpm);
    Serial.print("\n");

    Serial.print("Divided rate: ");
    Serial.print(dividedRate, 8);
    Serial.print("\n");
    */

  } else {
    //DEBUGGING - REMOVE BEFORE PRODUCTION USE
    /*
    Serial.print("NO CLOCK RECEIVED - go back to using raw knob value");
    Serial.print("\n");
    */
    
    //Get LFO frequencies manually from knobs
    lfo1Freq = mapf(analogRead(pE2), 0, 1023, 0.001, 20); 
    lfo2Freq = mapf(analogRead(pD3), 0, 1023, 0.001, 20); 
    lfo3Freq = mapf(analogRead(pE3), 0, 1023, 0.001, 20);
  }

    //Set LFO frequencies
    sine1.frequency(lfo1Freq);  
    sine2.frequency(lfo2Freq);
    sine3.frequency(lfo3Freq);

  //Filter resonance - Universal for all 6 filters
  float filterRes = mapf(analogRead(pD2), 0, 1023, 0.7, 3.0);
  filter1.resonance(filterRes);
  filter2.resonance(filterRes);
  filter3.resonance(filterRes);
  filter4.resonance(filterRes);
  filter5.resonance(filterRes);
  filter6.resonance(filterRes);

  filter1.octaveControl(5);
  filter2.octaveControl(5);
  filter3.octaveControl(5);
  filter4.octaveControl(5);
  filter5.octaveControl(5);
  filter6.octaveControl(5);

  //LFO centers - behaves like filter center frequency
  float lfo1Center = mapf(analogRead(pA2), 0, 1023, -1.0, 1.0);
  dc1.amplitude(lfo1Center);

  float lfo2Center = mapf(analogRead(pB2), 0, 1023, -1.0, 1.0);
  dc2.amplitude(lfo2Center);

  float lfo3Center = mapf(analogRead(pC2), 0, 1023, -1.0, 1.0);
  dc3.amplitude(lfo3Center);
  
  //LFO 1 width
  float lfo1Width = mapf(analogRead(pA3), 0, 1023, 0.0, 1.0); 
  sine1.amplitude(lfo1Width);

  //LFO 2 width
  float lfo2Width = mapf(analogRead(pB3), 0, 1023, 0.0, 1.0); 
  sine2.amplitude(lfo2Width);

  //LFO 3 width
  float lfo3Width = mapf(analogRead(pC3), 0, 1023, 0.0, 1.0); 
  sine3.amplitude(lfo3Width);
}

void startupSequence() {
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

void setChannelLEDs() {
  // Switch A
  if (digitalRead(SwitchA1) == 0) {
    digitalWrite(LEDA, HIGH);
  } else {
    digitalWrite(LEDA, LOW);
  }

  // Switch B
  if (digitalRead(SwitchB1) == 0) {
    digitalWrite(LEDB, HIGH);
  } else {
    digitalWrite(LEDB, LOW);
  }

  // Switch C
  if (digitalRead(SwitchC1) == 0) {
    digitalWrite(LEDC, HIGH);
  } else {
    digitalWrite(LEDC, LOW);
  }
}

