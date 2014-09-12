static int latch = 12; // set the latch pin
static int clock = 13; // set the clock pin
static int datin = 11;// set the data in pin

boolean btn_up    = false;
boolean btn_down  = false;
boolean btn_left  = false;
boolean btn_right = false;

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
  if (bitRead(controller_data, 15) == 0)
    joySt.buttons = joySt.buttons | 1;
  else
    joySt.buttons = joySt.buttons & 254;

  // Y - BIT 15
  if (bitRead(controller_data, 14) == 0)
    joySt.buttons = joySt.buttons | 2;
  else
    joySt.buttons = joySt.buttons & 253;

  // SELECT - BIT 14
  if (bitRead(controller_data, 13) == 0)
    joySt.buttons = joySt.buttons | 64;
  else
    joySt.buttons = joySt.buttons & 191;

  // START - BIT 13
  if (bitRead(controller_data, 12) == 0)
    joySt.buttons = joySt.buttons | 128;
  else
    joySt.buttons = joySt.buttons & 127;

  // UP - BIT 12
  if (bitRead(controller_data, 11) == 0 && !btn_up) {
    btn_up = true;
    joySt.yAxis = 0;
  }

  if (bitRead(controller_data, 11) == 1 && btn_up) {
    btn_up = false;
    joySt.yAxis = 127;
  }

  // DOWN - BIT 11
  if (bitRead(controller_data, 10) == 0 && !btn_down) {
    btn_down = true;
    joySt.yAxis = 255;
  }

  if (bitRead(controller_data, 10) == 1 && btn_down) {
    btn_down = false;
    joySt.yAxis = 127;
  }

  // LEFT - BIT 10
  if (bitRead(controller_data, 9) == 0 && !btn_left) {
    btn_left = true;
    joySt.xAxis = 0;
  }

  if (bitRead(controller_data, 9) == 1 && btn_left) {
    btn_left = false;
    joySt.xAxis = 127;
  }

  // RIGHT - BIT 9
  if (bitRead(controller_data, 8) == 0 && !btn_right) {
    btn_right = true;
    joySt.xAxis = 255;
  }

  if (bitRead(controller_data, 8) == 1 && btn_right) {
    btn_right = false;
    joySt.xAxis = 127;
  }

  // A - BIT 8
  if (bitRead(controller_data, 7) == 0)
    joySt.buttons = joySt.buttons | 4;
  else 
    joySt.buttons = joySt.buttons & 251;

  // X - BIT 7
  if (bitRead(controller_data, 6) == 0)
    joySt.buttons = joySt.buttons | 8;
  else
    joySt.buttons = joySt.buttons & 247;


  // L - BIT 6
  if (bitRead(controller_data, 5) == 0)
    joySt.buttons = joySt.buttons | 16;
  else
    joySt.buttons = joySt.buttons & 239;

  // R - BIT 5
  if (bitRead(controller_data, 4) == 0)
    joySt.buttons = joySt.buttons | 32;
  else
    joySt.buttons = joySt.buttons & 223;

  Joystick.setState(&joySt);

}
