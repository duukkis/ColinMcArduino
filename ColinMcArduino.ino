/*************************************************************
DO WHAT YOU DO BEST
*************************************************************/
int pinAmotor = 12;
int pinAbrake = 9;
int pinAstrength = 3;

int pinBmotor = 13;
int pinBbrake = 8;
int pinBstrength = 11;

int echoPin = 4;
int trigPin = 5;

int speaker = 7;

long duration, cm;

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

  // speaker
  pinMode(speaker, OUTPUT);

  // wait for 3 s before starting
  delay(3000);
}

/*
read distance
*/
void sensorRead(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;
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

int alarmCm = 40;

void goForwardMS(int howMuch, int milliseconds){
  goForward(howMuch);
  int step = 20;
  for(int i = 0; i < milliseconds;i = i + step){
    sensorRead();
    if(cm < alarmCm){
      alarmBrake();
    } else {
      delay(step);
    }
  }
  stopMotorA();
}

void alarmBrake(){
  goBackwardMS(255, 100);
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

int turnDelay = 400;

void fullTurn(){
  goBackwardLeft(180,1200);
  delay(turnDelay);
  goForwardRight(180,1200);
  delay(turnDelay);
  goBackwardLeft(180,1200);
  delay(turnDelay);
  goForwardRight(180,1200);
  delay(turnDelay);
}

void leftTurn(){
  goBackwardRight(180,1200);
  delay(turnDelay);
  goForwardLeft(180,2000);
  delay(turnDelay);
}

void rightTurn(){
  goBackwardLeft(180,1200);
  delay(turnDelay);
  goForwardRight(180,2000);
  delay(turnDelay);
}


void fullforwardUntil(int untilCm, int speed){
  stopMotorB();
  goForward(speed);
  sensorRead();
  while(cm > untilCm){
    sensorRead();
    delay(20);
  }
  stopMotorA();
}

void doSpeaker(){
  digitalWrite(speaker, HIGH);
  digitalWrite(speaker, LOW);
}


void loop(){
  doSpeaker();
  fullforwardUntil(40, 180);
  int randNumber = random(1, 3);
  switch (randNumber) {
    case 1:
      leftTurn();
      goForwardMS(180, 2000);
      break;
    case 2:
      rightTurn();
      goForwardMS(180, 2000);
      break;
    default:
      fullTurn();
      break;
  }
  
  delay(1000);  
}

