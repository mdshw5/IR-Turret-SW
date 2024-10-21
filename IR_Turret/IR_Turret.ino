#include <Arduino.h>
#include <Servo.h>

#include "IR_Signals.h"
#include "IR_Logic.h"

const int turnServoPin =  9;
const int tiltServoPin =  10;
const int rollServoPin =  11;

Servo turnservo;
Servo tiltservo;
Servo rollservo;

bool isTurnServoEnabled = false;
bool isRollServoEnabled = false;

int tiltPosition = 110;
int turnInterval = 400;
int fullTurnInterval = 1700;
int sweepInterval = 400;
int sweepCount = 1;
int stepCount = 0;
int fireAllInterval = 2000;
int fireSingleInterval = 280;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

String mode = "IDLE";


void setup(void) {
  Serial.begin(9600);
  Serial.println("Power On.");
  Serial.println(" ");
  tiltservo.attach(tiltServoPin);
  tiltservo.write(tiltPosition);
  delay(50);
}

void loop(void) {

  currentMillis = millis();

  if(mode == "IDLE"){
    //Wait to recieve IR Signal
    Serial.println("Listening...");
    Serial.println(" ");
    numberpulses = listenForIR();
    defineMode();
  }

  if(mode == "RIGHT_TURN"){
    rightTurn(turnInterval);
  }
  if(mode == "LEFT_TURN"){
    leftTurn(turnInterval);
  }
  if(mode == "TILT_UP"){
    tiltUp();
  }
  if(mode == "TILT_DOWN"){
    tiltDown();
  }
  if(mode == "FIRE_ALL"){
    fireAll();
  }
  if(mode == "FIRE_SINGLE"){
    fireSingle();
  }
  if(mode == "STEP_FIRE"){
    stepFire();
  }
  if(mode == "CIRCULAR_FIRE"){
    circularFire();
  }
  if(mode == "SWEEP_TURRET"){
    sweepTurret();
  }
  if(mode == "HEAD_SHAKE"){
    headShake();
  }
  
}

// ==============================================================================
// == Custom Servo Control Functions
// ==============================================================================

void leftTurn(int interval){
  if(!isTurnServoEnabled){
      previousMillis = millis();
      currentMillis = millis();
      turnservo.attach(turnServoPin);
      delay(100);
      turnservo.write(180);
      isTurnServoEnabled = true;
    }

    if(currentMillis - previousMillis > interval){
      turnservo.write(90);
      delay(100);
      turnservo.detach();
      isTurnServoEnabled = false;
      if(mode == "LEFT_TURN"){
        mode = "IDLE";
      }
    }
}

void rightTurn(int interval){
  if(!isTurnServoEnabled){
      previousMillis = millis();
      currentMillis = millis();
      turnservo.attach(turnServoPin);
      delay(100);
      turnservo.write(0);
      isTurnServoEnabled = true;
    }

    if(currentMillis - previousMillis > interval){
      turnservo.write(90);
      delay(100);
      turnservo.detach();
      isTurnServoEnabled = false;
      if(mode == "RIGHT_TURN"){
        mode = "IDLE";
      }
    }
}

void sweepTurret(){
    if(!isRollServoEnabled){
      previousMillis = millis();
      currentMillis = millis();
      sweepCount = 1;
      tiltPosition = 110;
      rollservo.attach(rollServoPin);
      delay(100);
      rollservo.write(180);
      isRollServoEnabled = true;
    }

    if(currentMillis - previousMillis > sweepInterval){
      previousMillis = millis();
      if(sweepCount > 4){
        rollservo.write(90);
        tiltPosition = 110;
        tiltservo.write(tiltPosition);
        delay(100);
        rollservo.detach();
        isRollServoEnabled = false;
        mode = "IDLE";
      }
      else if(sweepCount % 2 == 0){
        tiltDown();
        tiltPosition = 110;
      }else{
        tiltUp();
        tiltPosition = 110;
      }
      sweepCount += 1;
    }
}

void headShake(){
  tiltservo.write(100);
  delay(3000);
  turnservo.attach(turnServoPin);
  delay(100);
  turnservo.write(0);
  delay(600);
  turnservo.write(90);
  delay(500);
  tiltUp();
  delay(500);
  turnservo.write(180);
  delay(300);
  turnservo.write(90);
  delay(250);
  turnservo.write(0);
  delay(400);
  turnservo.write(90);
  delay(250);
  turnservo.write(180);
  delay(400);
  // turnservo.write(90);
  // delay(250);
  // turnservo.write(0);
  // delay(400);
  // turnservo.write(90);
  // delay(250);
  // turnservo.write(180);
  // delay(400);
  turnservo.write(90);
  delay(250);
  turnservo.detach();
  // tiltDown();
  mode = "IDLE";
}

void tiltDown(){
  if(tiltPosition > 90){
    tiltPosition = tiltPosition - 20;
    tiltservo.write(tiltPosition);
    delay(50);
  }
  if(mode == "TILT_DOWN"){
    mode = "IDLE";
  }
}

void tiltUp(){
  if(tiltPosition < 130){
    tiltPosition = tiltPosition + 20;
    tiltservo.write(tiltPosition);
    delay(50);
  }
  if(mode == "TILT_UP"){
    mode = "IDLE";
  }
}


void fireAll(){
    if(!isRollServoEnabled){
      previousMillis = millis();
      currentMillis = millis();
      rollservo.attach(rollServoPin);
      delay(100);
      rollservo.write(180);
      isRollServoEnabled = true;
    }

    if(currentMillis - previousMillis > fireAllInterval){
      rollservo.write(90);
      delay(100);
      rollservo.detach();
      isRollServoEnabled = false;
      mode = "IDLE";
    }
}

void fireSingle(){
  if(!isRollServoEnabled){
      previousMillis = millis();
      currentMillis = millis();
      rollservo.attach(rollServoPin);
      delay(100);
      rollservo.write(180);
      isRollServoEnabled = true;
    }

    if(currentMillis - previousMillis > fireSingleInterval){
      rollservo.write(90);
      delay(100);
      rollservo.detach();
      isRollServoEnabled = false;
      if(mode == "FIRE_SINGLE"){
        mode = "IDLE";
      }
    }
}

void circularFire(){
    if(!isRollServoEnabled){
      previousMillis = millis();
      currentMillis = millis();
      turnservo.attach(turnServoPin);
      rollservo.attach(rollServoPin);
      delay(100);
      rollservo.write(180);
      turnservo.write(0);
      isTurnServoEnabled = true;
      isRollServoEnabled = true;
    }

    if(currentMillis - previousMillis > fireAllInterval - 250){
      rollservo.write(90);
      turnservo.write(90);
      delay(100);
      rollservo.detach();
      turnservo.detach();
      isTurnServoEnabled = false;
      isRollServoEnabled = false;
      mode = "IDLE";
    }
}

void stepFire(){
    if(!isTurnServoEnabled && !isRollServoEnabled){
      Serial.println("Turn!");
      previousMillis = millis();
      currentMillis = millis();
      turnservo.attach(turnServoPin);
      delay(100);
      turnservo.write(0);
      isTurnServoEnabled = true;
    }

    if(currentMillis - previousMillis > 400){
      if(isTurnServoEnabled && !isRollServoEnabled){
        Serial.println("Stop Turn, Start Fire");
        turnservo.write(90);
        delay(100);
        turnservo.detach();
        //Just Turned, Now Fire Single
        previousMillis = currentMillis;
        rollservo.attach(rollServoPin);
        delay(100);
        rollservo.write(180);
        isRollServoEnabled = true;
        isTurnServoEnabled = false;
      }
      else {
        Serial.println("Stop Fire, Restart");
        rollservo.write(90);
        delay(100);
        rollservo.detach();
        isTurnServoEnabled = false;
        isRollServoEnabled = false;
        stepCount += 1;
      }
    }
  if(stepCount == 6){
    mode = "IDLE";
    turnservo.write(90);
    delay(100);
    turnservo.detach();
    isTurnServoEnabled = false;
    isRollServoEnabled = false;
    stepCount = 0;
  }
}

void defineMode(){
    if (IRcompare(numberpulses, VolDownSignal,sizeof(VolDownSignal)/4)) {
      Serial.println("Rotate Left!");
      Serial.println(" ");
      mode = "LEFT_TURN";
    }

    if (IRcompare(numberpulses, VolUpSignal,sizeof(VolUpSignal)/4)) {
      Serial.println("Rotate Right!");
      Serial.println(" ");
      mode = "RIGHT_TURN";
    }

    if (IRcompare(numberpulses, PrevSignal,sizeof(PrevSignal)/4)) {
      Serial.println("Previous");
      mode = "TILT_DOWN";
    }

    if (IRcompare(numberpulses, NextSignal,sizeof(NextSignal)/4)) {
      Serial.println("Next");
      mode = "TILT_UP";
    }

    if (IRcompare(numberpulses, PlaySignal,sizeof(PlaySignal)/4)) {
      Serial.println("Fire All!");
      mode = "FIRE_ALL";
    }

    if (IRcompare(numberpulses, ChannelSignal,sizeof(ChannelSignal)/4)) {
      Serial.println("Channel");
      mode = "FIRE_SINGLE";
    }
    if (IRcompare(numberpulses, NumberOne,sizeof(NumberOne)/4)) {
      Serial.println("Number 1");
      mode = "STEP_FIRE";
    }
    // if (IRcompare(numberpulses, NumberTwo,sizeof(NumberTwo)/4)) {
    //   Serial.println("Number 2");
    //   mode = "CIRCULAR_FIRE";
    // }
    // if (IRcompare(numberpulses, NumberThree,sizeof(NumberThree)/4)) {
    //   Serial.println("Number 3");
    //   mode = "SWEEP_TURRET";
    // }
    if (IRcompare(numberpulses, NumberFour,sizeof(NumberFour)/4)) {
      Serial.println("Number 4");
      mode = "HEAD_SHAKE";
    }
}