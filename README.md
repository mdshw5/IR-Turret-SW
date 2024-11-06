# How to Program the IR Turret

*Watch how to build a hack pack inspired IR turret* - [Video](https://youtu.be/k28ID4FwFjI)

Download 3D files [here](https://www.herreraengineering.com/)

## Wire Diagram

<img src="gifs\Wire_Diagram.png" alt="wire diagram" width="700px">

## Step 1: Download Sketches

<img src="gifs\download_sketches.gif" alt="download sketches" width="700px">

1. Download zip file
2. Extract files
3. Copy both folders
4. Paste folders in local Arduino folder

## Step 2: Decode IR Remote

<img src="gifs\decode_ir_signal.gif" alt="decode ir signal" width="700px">

1. Open IR_Reader.ino sketch in Arduino IDE
2. Connect arduino to the computer using a usb cable
3. Upload sketch to the arduino board on selected port
4. Open the serial monitor
5. Point IR remote at IR sensor and press desired button to decode
6. IR signal values will print on the serial monitor when signal is received
7. Copy IR signal values only

## Step 3: Program IR Signal Function

<img src="gifs\program_turret.gif" alt="program turret" width="700px">

1. Open IR_Turret.ino sketch in Arduino IDE
2. Connect arduino to the computer using a usb cable
3. Upload sketch to the arduino board on selected port
4. Switch to IR_Signals.h tab in IDE
5. Create new variable to store IR signal values
6. Paste in IR signal values from step 2
7. Switch back to IR_Turret.ino tab and scroll to bottom
8. Copy and paste if statement
9. Change NumberFour to new IR signal value name created in step 5
8. Change print statement to match button name
9. Change mode to match robot movement function
- See example where `mode = "RIGHT_TURN"` calls `rightTurn(turnInterval)` function
11. Upload sketch to the arduino board on selected port
12. Open the serial monitor
13. Press decoded button
- Button name prints to serial monitor when signal is received
- Robot performs movement function


