/*
 * Arduino code for Staff Weapon project
 * 2017 HaD sci-fi contest (shlonkin)
 * 
 * Uses a 5V 16MHz Pro Trinket
 * 
 * This code is in the public domain. Enjoy
 */
 
#include <Servo.h>

// LED pins
int leds[4] = {6, 5, 11, 3};

// trigger switch pins
int armSwitch = 14;
int fireSwitch = 15;

Servo firingServo;
// angles for the firing pin
int pinIn = 180;
int pinOut = 150;

// variables
int state; //0=disarmed, 1=armed, 2=fired

void setup() {

  // servo for shooting
  firingServo.attach(9);
  firingServo.write(pinIn);
  
  // pins
  pinMode(leds[0], OUTPUT);
  digitalWrite(leds[0], LOW);
  pinMode(leds[1], OUTPUT);
  digitalWrite(leds[1], LOW);
  pinMode(leds[2], OUTPUT);
  digitalWrite(leds[2], LOW);
  pinMode(leds[3], OUTPUT);
  digitalWrite(leds[3], LOW);

  pinMode(armSwitch, INPUT_PULLUP);
  pinMode(fireSwitch, INPUT_PULLUP);
  pinMode(16, OUTPUT); // provides ground for the switches
  digitalWrite(16, LOW);

  state = 0;
}

void loop() {
  // Wait for a switch press
  // If disarmed: arming switch will arm, fire switch will do nothing
  // If armed: arming switch will disarm, fire switch will fire
  // If fired: arming switch will disarm and insert firing pin, fire switch will insert firing pin
  if(!digitalRead(armSwitch)){
    if(state == 0){
      state = 1;
      playArm();
      // wait for switch release, then debounce
      delay(100);
      while(!digitalRead(armSwitch));
      delay(100);
    }else if(state == 1){
      state = 0;
      digitalWrite(leds[3], HIGH);
      delay(100);
      digitalWrite(leds[3], LOW);
      // wait for switch release, then debounce
      delay(100);
      while(!digitalRead(armSwitch));
      delay(100);
    }else if(state == 2){
      state = 0;
      digitalWrite(leds[3], HIGH);
      delay(100);
      digitalWrite(leds[3], LOW);
      firingServo.write(pinIn);
      // wait for switch release, then debounce
      delay(100);
      while(!digitalRead(armSwitch));
      delay(100);
    }
  }else if(!digitalRead(fireSwitch)){
    if(state == 0){
      // wait for switch release, then debounce
      delay(100);
      while(!digitalRead(fireSwitch));
      delay(100);
    }else if(state == 1){
      state = 2;
      playFire();
      firingServo.write(pinOut);
      // wait for switch release, then debounce
      delay(100);
      while(!digitalRead(fireSwitch));
      delay(500);
    }else if(state == 2){
      state = 1;
      firingServo.write(pinIn);
      // wait for switch release, then debounce
      delay(100);
      while(!digitalRead(fireSwitch));
      delay(1000);
    }
  }
}

void playArm(){
  // arming sequence
  // total sequence is about 1s long
  // chopped into 20ms parts (48 total)
  // each led does a triange up and down 24 parts long
  // each part changes 20 pwm vals
  int vals[4] = {0, 0, 0, 0};
  int dt = 15;
  for(int i=0; i<8; i++){
    vals[0] += 20;
    analogWrite(leds[0], vals[0]);
    delay(dt);
  }
  for(int i=8; i<12; i++){
    vals[0] += 20;
    vals[1] += 20;
    analogWrite(leds[0], vals[0]);
    analogWrite(leds[1], vals[1]);
    delay(dt);
  }
  for(int i=12; i<16; i++){
    vals[0] -= 20;
    vals[1] += 20;
    analogWrite(leds[0], vals[0]);
    analogWrite(leds[1], vals[1]);
    analogWrite(leds[2], vals[2]);
    delay(dt);
  }
  for(int i=16; i<20; i++){
    vals[0] -= 20;
    vals[1] += 20;
    vals[2] += 20;
    analogWrite(leds[0], vals[0]);
    analogWrite(leds[1], vals[1]);
    analogWrite(leds[2], vals[2]);
    delay(dt);
  }
  for(int i=20; i<24; i++){
    vals[0] -= 20;
    vals[1] -= 20;
    vals[2] += 20;
    analogWrite(leds[0], vals[0]);
    analogWrite(leds[1], vals[1]);
    analogWrite(leds[2], vals[2]);
    delay(dt);
  }
  analogWrite(leds[0], 0);
  for(int i=24; i<28; i++){
    vals[1] -= 20;
    vals[2] += 20;
    vals[3] += 20;
    analogWrite(leds[1], vals[1]);
    analogWrite(leds[2], vals[2]);
    analogWrite(leds[3], vals[3]);
    delay(dt);
  }
  for(int i=28; i<32; i++){
    vals[1] -= 20;
    vals[2] -= 20;
    vals[3] += 20;
    analogWrite(leds[1], vals[1]);
    analogWrite(leds[2], vals[2]);
    analogWrite(leds[3], vals[3]);
    delay(dt);
  }
  analogWrite(leds[1], 0);
  for(int i=32; i<36; i++){
    vals[2] -= 20;
    vals[3] += 20;
    analogWrite(leds[2], vals[2]);
    analogWrite(leds[3], vals[3]);
    delay(dt);
  }
  for(int i=36; i<40; i++){
    vals[2] -= 20;
    vals[3] -= 20;
    analogWrite(leds[2], vals[2]);
    analogWrite(leds[3], vals[3]);
    delay(dt);
  }
  analogWrite(leds[2], 0);
  for(int i=40; i<48; i++){
    vals[3] -= 20;
    analogWrite(leds[0], vals[0]);
    analogWrite(leds[3], vals[3]);
    delay(dt);
  }
  analogWrite(leds[3], 0);
}

void playFire(){
  // firing sequence
  digitalWrite(leds[3], HIGH);
  delay(20);
  digitalWrite(leds[2], HIGH);
  delay(20);
  digitalWrite(leds[1], HIGH);
  delay(20);
  digitalWrite(leds[0], HIGH);
  delay(200);
  digitalWrite(leds[3], LOW);
  delay(20);
  digitalWrite(leds[2], LOW);
  delay(20);
  digitalWrite(leds[1], LOW);
  delay(20);
  digitalWrite(leds[0], LOW);
}

