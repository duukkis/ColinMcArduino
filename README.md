# ColinMcArduino

`code()`
/*************************************************************
1. Download this: https://www.arduino.cc/en/main/software
2. Write some good code into loop function
*************************************************************/
int pinAmotor = 12;
int pinAbrake = 9;
int pinAstrength = 3;

int pinBmotor = 13;
int pinBbrake = 8;
int pinBstrength = 11;

int echoPin = 4;
int trigPin = 5;

long duration, cm, inches;

void setup() {
  //Setup Channel A
  pinMode(pinAmotor, OUTPUT); //Initiates Motor Channel A pin
  pinMode(pinAbrake, OUTPUT); //Initiates Brake Channel A pin
  //Setup Channel B
  pinMode(pinBmotor, OUTPUT); //Initiates Motor Channel B pin
  pinMode(pinBbrake, OUTPUT);  //Initiates Brake Channel B pin

  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // wait for 3 s before starting
  delay(3000);
}

int sensorRead(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
}

void turnLeft(int howMuch){
  stopMotorB();
  digitalWrite(pinBmotor, HIGH); //Establishes forward direction of Channel B
  digitalWrite(pinBbrake, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, howMuch);   //Spins the motor on Channel B at full speed
}

void turnRight(int howMuch){
  stopMotorB();
  digitalWrite(pinBmotor, LOW); //Establishes forward direction of Channel B
  digitalWrite(pinBbrake, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, howMuch);   //Spins the motor on Channel B at full speed
}

void turnFullLeft(){ turnLeft(255); }
void turnHalfLeft(){ turnLeft(123); }

void turnFullRight(){ turnRight(255); }
void turnHalfRight(){ turnRight(123); }

void goForward(int howMuch){
  stopMotorA();
  digitalWrite(pinAmotor, LOW); // Establishes forward direction of Channel A
  digitalWrite(pinAbrake, LOW);   // Disengage the Brake for Channel A
  analogWrite(pinAstrength, howMuch);   // Spins the motor on Channel A at full speed
}
void goBackward(int howMuch){
  stopMotorA();
  digitalWrite(pinAmotor, HIGH); // Establishes forward direction of Channel B
  digitalWrite(pinAbrake, LOW);   // Disengage the Brake for Channel B
  analogWrite(pinAstrength, howMuch);   // Spins the motor on Channel B at full speed
}

void stopMotorA(){ digitalWrite(pinAbrake, HIGH); } //Engage the Brake for Channel A
void stopMotorB(){ digitalWrite(pinBbrake, HIGH); } //Engage the Brake for Channel B

void stopAB(){
  stopMotorA();
  stopMotorB();  
}

void goForwardMS(int howMuch, int milliseconds){
  goForward(howMuch);
  delay(milliseconds);
  stopMotorA();
}
void goBackwardMS(int howMuch, int milliseconds){
  goBackward(howMuch);
  delay(milliseconds);
  stopMotorA();  
}

int wheelTurnDelay = 100;

void goForwardRight(int howMuch, int milliseconds){
  turnFullRight();
  goForwardMS(howMuch, milliseconds);
  delay(wheelTurnDelay);
  stopMotorB();
}
void goForwardLeft(int howMuch, int milliseconds){
  turnFullLeft();
  goForwardMS(howMuch, milliseconds);
  delay(wheelTurnDelay);
  stopMotorB();
}
void goBackwardRight(int howMuch, int milliseconds){
  turnFullRight();
  goBackwardMS(howMuch, milliseconds);
  delay(wheelTurnDelay);
  stopMotorB();
}
void goBackwardLeft(int howMuch, int milliseconds){
  turnFullLeft();
  goBackwardMS(howMuch, milliseconds);
  delay(wheelTurnDelay);
  stopMotorB();
}


void fullTurn(){
  int turnDelay = 400;
  goForwardRight(180,1200);
  delay(turnDelay);
  goBackwardLeft(180,1200);
  delay(turnDelay);
  goForwardRight(180,1200);
  delay(turnDelay);
  goBackwardLeft(180,1200);
  delay(turnDelay);
}


// here is thew action
void loop(){
  delay(3000);
  sensorRead();
  delay(250);
  fullTurn();
  
}
`code()`
