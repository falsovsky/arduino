/*
Based on https://github.com/njguibert/NesJoystickArduino/
Adapted for SNES

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

  // Set joystick initial state
  joySt.xAxis = 127;
  joySt.yAxis = 127;
  joySt.zAxis = 127;
  joySt.buttons = 0;
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
  word controller_data = controllerRead();

  // B - BIT 16
  if (!btn_b && bitRead(controller_data, 15) == 0) {
    btn_b = true;
    joySt.buttons = joySt.buttons | 1;
  }
  if (btn_b && bitRead(controller_data, 15) == 1) {  
    joySt.buttons = joySt.buttons & 254;
    btn_b = false;
  }

  // Y - BIT 15
  if (!btn_y && bitRead(controller_data, 14) == 0) {
    btn_y = true;
    joySt.buttons = joySt.buttons | 2;
  }
  if (btn_y && bitRead(controller_data, 14) == 1) {
    btn_y = false;
    joySt.buttons = joySt.buttons & 253;
  }

  // SELECT - BIT 14
  if (!btn_select && bitRead(controller_data, 13) == 0) {
    btn_select = true;
    joySt.buttons = joySt.buttons | 64;
  }
  if (btn_select && bitRead(controller_data, 13) == 1) {
    btn_select = false;
    joySt.buttons = joySt.buttons & 191;
  }

  // START - BIT 13
  if (!btn_start && bitRead(controller_data, 12) == 0) {
    btn_start = true;
    joySt.buttons = joySt.buttons | 128;
  }
  if (btn_start && bitRead(controller_data, 12) == 1) {
    btn_start = false;
    joySt.buttons = joySt.buttons & 127;
  }

  // UP - BIT 12
  if (!btn_up && bitRead(controller_data, 11) == 0) {
    btn_up = true;
    joySt.yAxis = 0;
  }
  if (btn_up && bitRead(controller_data, 11) == 1) {
    btn_up = false;
    joySt.yAxis = 127;
  }

  // DOWN - BIT 11
  if (!btn_down && bitRead(controller_data, 10) == 0) {
    btn_down = true;
    joySt.yAxis = 255;
  }
  if (btn_down && bitRead(controller_data, 10) == 1) {
    btn_down = false;
    joySt.yAxis = 127;
  }

  // LEFT - BIT 10
  if (!btn_left && bitRead(controller_data, 9) == 0) {
    btn_left = true;
    joySt.xAxis = 0;
  }
  if (btn_left && bitRead(controller_data, 9) == 1) {
    btn_left = false;
    joySt.xAxis = 127;
  }

  // RIGHT - BIT 9
  if (!btn_right && bitRead(controller_data, 8) == 0) {
    btn_right = true;
    joySt.xAxis = 255;
  }
  if (btn_right && bitRead(controller_data, 8) == 1) {
    btn_right = false;
    joySt.xAxis = 127;
  }

  // A - BIT 8
  if (!btn_a && bitRead(controller_data, 7) == 0) {
    btn_a = true;
    joySt.buttons = joySt.buttons | 4;
  }
  if (btn_a && bitRead(controller_data, 7) == 1) {
    btn_a = false;
    joySt.buttons = joySt.buttons & 251;
  }

  // X - BIT 7
  if (!btn_x && bitRead(controller_data, 6) == 0) {
    btn_x = true;
    joySt.buttons = joySt.buttons | 8;
  }
  if (btn_x && bitRead(controller_data, 6) == 1) {
    btn_x = true;
    joySt.buttons = joySt.buttons & 247;
  }

  // L - BIT 6
  if (!btn_l && bitRead(controller_data, 5) == 0) {
    btn_l = true;
    joySt.buttons = joySt.buttons | 16;
  }
  if (btn_l && bitRead(controller_data, 5) == 1) {
    btn_l = false;
    joySt.buttons = joySt.buttons & 239;
  }

  // R - BIT 5
  if (!btn_r && bitRead(controller_data, 4) == 0) {
    btn_r = true;
    joySt.buttons = joySt.buttons | 32;
  }
  if (btn_r && bitRead(controller_data, 4) == 1) {
    btn_r = false;
    joySt.buttons = joySt.buttons & 223;
  }

  Joystick.setState(&joySt);
}
