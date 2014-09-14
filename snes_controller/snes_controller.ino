/*
Based on https://github.com/njguibert/NesJoystickArduino/
Adapted for SNES with info from http://www.gamefaqs.com/snes/916396-super-nintendo/faqs/5395

Snes Controller - Arduino
White 5V        - 5V
Brown GND       - GND
Yellow (Clock)  - Pin 13
Orange (Latch)  - Pin 12
Red    (Data)   - Pin 11
*/

static int clock   = 13; // set the clock pin
static int latch   = 12; // set the latch pin
static int datin   = 11; // set the data in pin

boolean btn_up     = false;
boolean btn_down   = false;
boolean btn_left   = false;
boolean btn_right  = false;
boolean btn_a      = false;
boolean btn_b      = false;
boolean btn_x      = false;
boolean btn_y      = false;
boolean btn_l      = false;
boolean btn_r      = false;
boolean btn_select = false;
boolean btn_start  = false;

JoyState_t joySt;

/* SETUP */
void setup() {
  //Serial.begin(9600);
  
  pinMode(latch,OUTPUT);
  pinMode(clock,OUTPUT);
  pinMode(datin,INPUT);

  digitalWrite(latch,HIGH);
  digitalWrite(clock,HIGH);

  joySt.zAxis = 127;
}

/* THIS READS DATA FROM THE CONTROLLER */
word controllerRead() {
  word data = 0;

  digitalWrite(latch,LOW);
  digitalWrite(clock,LOW);

  digitalWrite(latch,HIGH);
  delayMicroseconds(2);
  digitalWrite(latch,LOW);

  data = digitalRead(datin);

  for (int i = 1; i <= 15; i ++) {
    digitalWrite(clock,HIGH);
    delayMicroseconds(2);
    data = data << 1;
    data = data + digitalRead(datin) ;
    delayMicroseconds(2);
    digitalWrite(clock,LOW);
  }

  return data;
}

void loop() {
  /*
  Clock Cycle     Button Reported
  ===========     ===============
  1               B
  2               Y
  3               Select
  4               Start
  5               Up on joypad
  6               Down on joypad
  7               Left on joypad
  8               Right on joypad
  9               A
  10              X
  11              L
  12              R
  13              none (always high)
  14              none (always high)
  15              none (always high)
  16              none (always high)
  */

  word controller_data = controllerRead();

  joySt.buttons = 0;

  joySt.buttons |= (!bitRead(controller_data, 15) << 0);
  joySt.buttons |= (!bitRead(controller_data, 14) << 1);
  joySt.buttons |= (!bitRead(controller_data, 7) << 2);
  joySt.buttons |= (!bitRead(controller_data, 6) << 3);
  joySt.buttons |= (!bitRead(controller_data, 5) << 4);
  joySt.buttons |= (!bitRead(controller_data, 4) << 5);
  joySt.buttons |= (!bitRead(controller_data, 13) << 6);
  joySt.buttons |= (!bitRead(controller_data, 12) << 7);

  
  joySt.xAxis = 127;
  joySt.yAxis = 127;
  
  // UP - BIT 12
  if (!bitRead(controller_data, 11))
    joySt.yAxis = 0;

  // DOWN - BIT 11
  if (!bitRead(controller_data, 10))
    joySt.yAxis = 255;

  // LEFT - BIT 10
  if (!bitRead(controller_data, 9))
    joySt.xAxis = 0;

  // RIGHT - BIT 9
  if (!bitRead(controller_data, 8))
    joySt.xAxis = 255;

  Joystick.setState(&joySt);
}
