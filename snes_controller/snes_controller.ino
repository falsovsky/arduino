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

static int clock = 13; // set the clock pin
static int latch = 12; // set the latch pin
static int datin = 11; // set the data in pin

JoyState_t joySt;

void setup() {
  //Serial.begin(9600);
  
  pinMode(latch,OUTPUT);
  pinMode(clock,OUTPUT);
  pinMode(datin,INPUT);

  digitalWrite(latch,HIGH);
  digitalWrite(clock,HIGH);

  joySt.zAxis = 127;
}

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
    data = data | digitalRead(datin) ;
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

  word controller_data = controllerRead(); // Read 16 bit of data

  joySt.buttons = 0; // Reset buttons

  joySt.buttons |= (!bitRead(controller_data, 15) << 0); // B - bit 16 - bit 1 on the Joystick
  joySt.buttons |= (!bitRead(controller_data, 14) << 1); // Y - bit 15 - bit 2 on the Joystick
  joySt.buttons |= (!bitRead(controller_data, 13) << 6); // Select - bit 14 - bit 7 on the Joystick
  joySt.buttons |= (!bitRead(controller_data, 12) << 7); // Start - bit 13 - bit 8 on the Joystick
  joySt.buttons |= (!bitRead(controller_data, 7) << 2);  // A - bit 8 - bit 6 on the Joystick
  joySt.buttons |= (!bitRead(controller_data, 6) << 3);  // X - bit 7 - bit 5 on the Joystick
  joySt.buttons |= (!bitRead(controller_data, 5) << 4);  // L - bit 6 - bit 4 on the Joystick
  joySt.buttons |= (!bitRead(controller_data, 4) << 5);  // R - bit 5 - bit 3 on the Joystick

  
  joySt.xAxis = 127; // Reset x axis
  joySt.yAxis = 127; // Rest y axis
  
  if (!bitRead(controller_data, 11)) // Up - bit 12
    joySt.yAxis = 0;

  if (!bitRead(controller_data, 10)) // Down - bit 11
    joySt.yAxis = 255;

  if (!bitRead(controller_data, 9))  // Left - bit 10
    joySt.xAxis = 0;

  if (!bitRead(controller_data, 8))  // Right - bit 9
    joySt.xAxis = 255;

  Joystick.setState(&joySt); // Update joystick state
}
