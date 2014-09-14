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
  //while (!Serial) ;

  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(datin, INPUT);

  digitalWrite(latch, LOW);
  digitalWrite(clock, HIGH);

  joySt.zAxis = 127;
}

word controllerRead()
{
  word data = 0;
 
  digitalWrite(latch, HIGH);
  delayMicroseconds(2);
  digitalWrite(latch, LOW);
  delayMicroseconds(1);
 
  digitalWrite(clock, LOW);
  delayMicroseconds(1);
 
  for (int i = 0; i < 16; i++)
  {
    data >>= 1;
    data |= (digitalRead(datin) ? 0x8000 : 0);
    digitalWrite(clock, HIGH);
    delayMicroseconds(2);
    digitalWrite(clock, LOW);
    delayMicroseconds(1);
  }
  digitalWrite(clock, HIGH);
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
  //Serial.println(controller_data, BIN);

  joySt.buttons = 0; // Reset buttons

  joySt.buttons |= (!bitRead(controller_data, 0) << 0);  // B - bit 1      - bit 1 on the Joystick
  joySt.buttons |= (!bitRead(controller_data, 1) << 1);  // Y - bit 2      - bit 2 on the Joystick
  joySt.buttons |= (!bitRead(controller_data, 2) << 6);  // Select - bit 3 - bit 7 on the Joystick
  joySt.buttons |= (!bitRead(controller_data, 3) << 7);  // Start - bit 4  - bit 8 on the Joystick
  joySt.buttons |= (!bitRead(controller_data, 8) << 2);  // A - bit 9      - bit 6 on the Joystick
  joySt.buttons |= (!bitRead(controller_data, 9) << 3);  // X - bit 10     - bit 5 on the Joystick
  joySt.buttons |= (!bitRead(controller_data, 10) << 4); // L - bit 11     - bit 4 on the Joystick
  joySt.buttons |= (!bitRead(controller_data, 11) << 5); // R - bit 12     - bit 3 on the Joystick


  joySt.xAxis = 127; // Reset x axis
  joySt.yAxis = 127; // Rest y axis

  if (!bitRead(controller_data, 4)) // Up - bit 5
    joySt.yAxis = 0;

  if (!bitRead(controller_data, 5)) // Down - bit 6
    joySt.yAxis = 255;

  if (!bitRead(controller_data, 6))  // Left - bit 7
    joySt.xAxis = 0;

  if (!bitRead(controller_data, 7))  // Right - bit 8
    joySt.xAxis = 255;

  Joystick.setState(&joySt); // Update joystick state
}

