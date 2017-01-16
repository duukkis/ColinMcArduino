/*************************************************************
DO WHAT YOU DO BEST
*************************************************************/
int pinAmotor = 12;
int pinAbrake = 9;
int pinAstrength = 3;

int pinBmotor = 13;
int pinBbrake = 8;
int pinBstrength = 11;

int echoPin = 5;
int trigPin = 4;

int speaker = 7;

int green = 2;
int yellow = 10;
int red = 6;

long duration, cm;

int stopped = 0;

byte pietso = 0;

// this depends on battery voltage
int masterspeed = 160;
int alarmCmRed = 10;
int alarmCm = 20;
int alarmCmGreen = 40;

void setup() {
  //Setup Channel A
  pinMode(pinAmotor, OUTPUT); //Initiates Motor Channel A pin
  pinMode(pinAbrake, OUTPUT); //Initiates Brake Channel A pin
  //Setup Channel B
  pinMode(pinBmotor, OUTPUT); //Initiates Motor Channel B pin
  pinMode(pinBbrake, OUTPUT);  //Initiates Brake Channel B pin

  // Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // speaker
  pinMode(speaker, OUTPUT);

  // leds
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  
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
  if(cm < alarmCmRed){
    digitalWrite(green, HIGH);
    digitalWrite(yellow, HIGH);
    digitalWrite(red, HIGH);
  } else if(cm < alarmCm){
    digitalWrite(green, HIGH);
    digitalWrite(yellow, HIGH);
    digitalWrite(red, LOW);
  } else if (cm < alarmCmGreen){
    digitalWrite(green, HIGH);
    digitalWrite(yellow, LOW);
    digitalWrite(red, LOW);
  } else {
    digitalWrite(green, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(red, LOW);
  }

  pietso = analogRead(5);
  if(pietso > 100){
    stopAB();
  }
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
void turnFullRight(){ turnRight(255); }

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
  sensorRead();
  goForward(howMuch);
  int step = 15;
  for(int i = 0; i < milliseconds;i = i + step){
    delay(step);
    sensorRead();
    if(cm < alarmCm){
      alarmBrake();
      sensorRead();
      // if alarm happened at first
      if(i == 0){
        stopped++;
      }
      cm = 60;
    }
  }
  stopMotorA();
}

void alarmBrake(){
  goBackwardMS(200, 50);
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

int turnDelay = 200;

void fullTurn(){
  alarmCm = 5;
  goBackwardLeft(masterspeed,1200);
  delay(turnDelay);
  goForwardRight(masterspeed,1200);
  delay(turnDelay);
  goBackwardLeft(masterspeed,1200);
  delay(turnDelay);
  goForwardRight(masterspeed,1200);
  delay(turnDelay);
  alarmCm = 20;
}

void leftTurn(){
  goBackwardRight(masterspeed,1600);
  delay(turnDelay);
  goForwardLeft(masterspeed,1200);
  delay(turnDelay);
}

void rightTurn(){
  goBackwardLeft(masterspeed,1600);
  delay(turnDelay);
  goForwardRight(masterspeed,1200);
  delay(turnDelay);
}

void startSpeaker(){
  digitalWrite(speaker, LOW);
  digitalWrite(speaker, HIGH);
}
void endSpeaker(){
  digitalWrite(speaker, HIGH);
  digitalWrite(speaker, LOW);
}


void loop(){
  startSpeaker();
  if(stopped == 5){
    stopped = 0;
    goBackwardMS(masterspeed, 1200);
    alarmCm = 20;
  }
  
  int randNumber = random(1, 4);
  switch (randNumber) {
    case 1:
      leftTurn();
      goForwardMS(masterspeed, 5000);
      break;
    case 2:
      rightTurn();
      goForwardMS(masterspeed, 5000);
      break;
    case 3:
      fullTurn();
      goForwardMS(masterspeed, 5000);
      break;
    case 4:
      rightTurn();
      goForwardMS(masterspeed, 5000);
      break;
  }
  alarmCm = 20;
  delay(500);
  endSpeaker();
}


