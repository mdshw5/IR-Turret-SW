#include <Arduino.h>
#include <Servo.h>

#define DECODE_NEC
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> // include the library

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
  Serial.begin(115200);
  while (!Serial)
    ; // Wait for Serial to become available.
  Serial.println("Power On.");
  Serial.println(" ");
  tiltservo.attach(tiltServoPin);
  delay(100);
  tiltservo.write(tiltPosition);
  delay(100);
  tiltservo.detach();

  // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
  Serial.println(F("at pin " STR(IR_RECEIVE_PIN)));
}

void loop(void) {
  currentMillis = millis();

  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
            // We have an unknown protocol here, print extended info
            IrReceiver.printIRResultRawFormatted(&Serial, true);
            IrReceiver.resume(); // Do it here, to preserve raw data for printing with printIRResultRawFormatted()
    } else {
            IrReceiver.resume(); // Early enable receiving of the next IR frame
            defineMode(IrReceiver.decodedIRData.command);
    }
  }

  if (mode == "IDLE") {
    // Wait to receive IR Signal
  }

  if (mode == "RIGHT_TURN") {
    rightTurn(turnInterval);
  }
  if (mode == "LEFT_TURN") {
    leftTurn(turnInterval);
  }
  if (mode == "TILT_UP") {
    tiltUp();
  }
  if (mode == "TILT_DOWN") {
    tiltDown();
  }
  if (mode == "FIRE_ALL") {
    fireAll();
  }
  if (mode == "FIRE_SINGLE") {
    fireSingle();
  }
  if (mode == "STEP_FIRE") {
    stepFire();
  }
  if (mode == "CIRCULAR_FIRE") {
    circularFire();
  }
  if (mode == "SWEEP_TURRET") {
    sweepTurret();
  }
  if (mode == "HEAD_SHAKE") {
    headShake();
  }
}

// =============================================================================
// == Custom Servo Control Functions
// =============================================================================

void leftTurn(int interval) {
  if (!isTurnServoEnabled) {
    previousMillis = millis();
    currentMillis = millis();
    turnservo.attach(turnServoPin);
    delay(100);
    turnservo.write(180);
    isTurnServoEnabled = true;
  }

  if (currentMillis - previousMillis > interval) {
    turnservo.write(90);
    delay(100);
    turnservo.detach();
    isTurnServoEnabled = false;
    if (mode == "LEFT_TURN") {
      mode = "IDLE";
    }
  }
}

void rightTurn(int interval) {
  if (!isTurnServoEnabled) {
    previousMillis = millis();
    currentMillis = millis();
    turnservo.attach(turnServoPin);
    delay(100);
    turnservo.write(0);
    isTurnServoEnabled = true;
  }

  if (currentMillis - previousMillis > interval) {
    turnservo.write(90);
    delay(100);
    turnservo.detach();
    isTurnServoEnabled = false;
    if (mode == "RIGHT_TURN") {
      mode = "IDLE";
    }
  }
}

void sweepTurret() {
  if (!isRollServoEnabled) {
    previousMillis = millis();
    currentMillis = millis();
    sweepCount = 1;
    tiltPosition = 110;
    rollservo.attach(rollServoPin);
    delay(100);
    rollservo.write(180);
    isRollServoEnabled = true;
  }

  if (currentMillis - previousMillis > sweepInterval) {
    previousMillis = millis();
    if (sweepCount > 4) {
      rollservo.write(90);
      tiltPosition = 110;
      tiltservo.attach(tiltServoPin);
      tiltservo.write(tiltPosition);
      delay(100);
      tiltservo.detach();
      rollservo.detach();
      isRollServoEnabled = false;
      mode = "IDLE";
    } else if (sweepCount % 2 == 0) {
      tiltDown();
      tiltPosition = 110;
    } else {
      tiltUp();
      tiltPosition = 110;
    }
    sweepCount += 1;
  }
}

void headShake() {
  tiltservo.attach(tiltServoPin);
  tiltservo.write(100);
  delay(100);
  tiltservo.detach();
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
  turnservo.write(90);
  delay(250);
  turnservo.detach();
  mode = "IDLE";
}

void tiltDown() {
  if (tiltPosition > 90) {
    tiltPosition = tiltPosition - 10;
    tiltservo.attach(tiltServoPin);
    tiltservo.write(tiltPosition);
    delay(50);
    tiltservo.detach();
  }
  if (mode == "TILT_DOWN") {
    mode = "IDLE";
  }
}

void tiltUp() {
  if (tiltPosition < 130) {
    tiltPosition = tiltPosition + 10;
    tiltservo.attach(tiltServoPin);
    tiltservo.write(tiltPosition);
    delay(50);
    tiltservo.detach();
  }
  if (mode == "TILT_UP") {
    mode = "IDLE";
  }
}

void fireAll() {
  if (!isRollServoEnabled) {
    previousMillis = millis();
    currentMillis = millis();
    rollservo.attach(rollServoPin);
    delay(100);
    rollservo.write(180);
    isRollServoEnabled = true;
  }

  if (currentMillis - previousMillis > fireAllInterval) {
    rollservo.write(90);
    delay(100);
    rollservo.detach();
    isRollServoEnabled = false;
    mode = "IDLE";
  }
}

void fireSingle() {
  if (!isRollServoEnabled) {
    previousMillis = millis();
    currentMillis = millis();
    rollservo.attach(rollServoPin);
    delay(100);
    rollservo.write(180);
    isRollServoEnabled = true;
  }

  if (currentMillis - previousMillis > fireSingleInterval) {
    rollservo.write(90);
    delay(100);
    rollservo.detach();
    isRollServoEnabled = false;
    if (mode == "FIRE_SINGLE") {
      mode = "IDLE";
    }
  }
}

void circularFire() {
  if (!isRollServoEnabled) {
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

  if (currentMillis - previousMillis > fireAllInterval - 250) {
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

void stepFire() {
  if (!isTurnServoEnabled && !isRollServoEnabled) {
    Serial.println("Turn!");
    previousMillis = millis();
    currentMillis = millis();
    turnservo.attach(turnServoPin);
    delay(100);
    turnservo.write(0);
    isTurnServoEnabled = true;
  }

  if (currentMillis - previousMillis > 400) {
    if (isTurnServoEnabled && !isRollServoEnabled) {
      Serial.println("Stop Turn, Start Fire");
      turnservo.write(90);
      delay(100);
      turnservo.detach();
      // Just Turned, Now Fire Single
      previousMillis = currentMillis;
      rollservo.attach(rollServoPin);
      delay(100);
      rollservo.write(180);
      isRollServoEnabled = true;
      isTurnServoEnabled = false;
    } else {
      Serial.println("Stop Fire, Restart");
      rollservo.write(90);
      delay(100);
      rollservo.detach();
      isTurnServoEnabled = false;
      isRollServoEnabled = false;
      stepCount += 1;
    }
  }
  if (stepCount == 6) {
    mode = "IDLE";
    turnservo.write(90);
    delay(100);
    turnservo.detach();
    isTurnServoEnabled = false;
    isRollServoEnabled = false;
    stepCount = 0;
  }
}

void defineMode(unsigned long value) {
  switch (value) {
    case 68: // Example IR code for VolDownSignal
      Serial.println("Rotate Left!");
      Serial.println(" ");
      mode = "LEFT_TURN";
      break;
    case 64: // Example IR code for VolUpSignal
      Serial.println("Rotate Right!");
      Serial.println(" ");
      mode = "RIGHT_TURN";
      break;
    case 7: // Example IR code for PrevSignal
      Serial.println("Previous");
      mode = "TILT_DOWN";
      break;
    case 21: // Example IR code for NextSignal
      Serial.println("Next");
      mode = "TILT_UP";
      break;
    case 67: // Example IR code for PlaySignal
      Serial.println("Fire All!");
      mode = "FIRE_ALL";
      break;
    case 70: // Example IR code for ChannelSignal
      Serial.println("Fire!");
      mode = "FIRE_SINGLE";
      break;
    case 12: // Example IR code for NumberOne
      Serial.println("Step Fire!");
      mode = "STEP_FIRE";
      break;
    case 8: // Example IR code for NumberFour
      Serial.println("Shake!");
      mode = "HEAD_SHAKE";
      break;
    default:
      Serial.println(value);
      mode = "IDLE";
      break;
  }
}