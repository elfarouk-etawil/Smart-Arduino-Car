#include <Servo.h>
#include <IRremote.h>

#define SERVO_WAIT_TIME 400

Servo servo;

// Ultrasonic sensor pins
const byte trigPin = A1; 
const byte echoPin = A2;

// Motor driver pins
const byte RightFront = 6;
const byte RightBack = 11;
const byte LeftBack = 3;
const byte LeftFront = 5;

// IR Receiver pin
const byte receiver = 2;

// Speed levels
byte speed;
const byte lowSpeed = 75;
const byte midSpeed = 150;
const byte maxSpeed = 255; 

// Turning time depends on speed
short turnTime;

// Car Mode
byte mode;

// Function to move forward
void moveForward() {
  analogWrite(RightFront, speed);
  analogWrite(RightBack, 0);
  analogWrite(LeftFront, speed);
  analogWrite(LeftBack, 0);
}


// Function to move backward
void moveBackward() {
  analogWrite(RightFront, 0);
  analogWrite(RightBack, speed);
  analogWrite(LeftFront, 0);
  analogWrite(LeftBack, speed);
}


// Function to turn left
void turnRight() {
  analogWrite(RightFront, 0);
  analogWrite(RightBack, speed);
  analogWrite(LeftFront, speed);
  analogWrite(LeftBack, 0);
  delay(turnTime);
}


// Function to turn right
void turnLeft()
{
  analogWrite(RightFront, speed);
  analogWrite(RightBack, 0);
  analogWrite(LeftFront, 0);
  analogWrite(LeftBack, speed);
  delay(turnTime);
}


// Function to stop motors
void stopMotors()
{
  analogWrite(RightFront, 0);
  analogWrite(RightBack, 0);
  analogWrite(LeftFront, 0);
  analogWrite(LeftBack, 0);
}


// Function to calculate distance
long calculateDistance()
{
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return (duration/2) / 29.1;
}


// Calculates distance from the right angle
long calculateRightDistance()
{
  servo.write(30);
  delay(SERVO_WAIT_TIME);
  long distance = calculateDistance();
  servo.write(90);
  delay(SERVO_WAIT_TIME);
  return distance;
}


// Calculates distance from the left angle
long calculateLeftDistance()
{
  servo.write(150);
  delay(SERVO_WAIT_TIME);
  long distance = calculateDistance();
  servo.write(90);
  delay(SERVO_WAIT_TIME);
  return distance;
}


void setup() 
{
  Serial.begin(9600); // Initialize Serial Monitor
  IrReceiver.begin(receiver, ENABLE_LED_FEEDBACK); // Start IR receiver
  Serial.println("IR Receiver ready");

  // Set motor pins as outputs
  pinMode(RightFront, OUTPUT);
  pinMode(RightBack, OUTPUT);
  pinMode(LeftFront, OUTPUT);
  pinMode(LeftBack, OUTPUT);

  // Initialize all motors to off
  stopMotors();
  
  // Set Ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set Servo motor pin
  servo.attach(4);
  
  // Reset servo motor angle
  servo.write(90);
  delay(SERVO_WAIT_TIME);

  mode = 1;
}

void loop()
{
  int command;
  speed = maxSpeed;
  switch(speed)
  {
    case maxSpeed:
      turnTime = 250;
      break;
    case midSpeed:
      turnTime = 500;
      break;
    case lowSpeed:
      turnTime = 800;
      break;
  }
  if (IrReceiver.decode())
  {
      // Print received code in HEX
      Serial.print("Received IR code: ");
      command = IrReceiver.decodedIRData.command;
      Serial.println(command);
      switch (command)
      {
        case 69:
          mode = 1;
          stopMotors();
          break;
        case 70:
          mode = 2;
          break;
      }
      // Resume IR receiver
      IrReceiver.resume();
  }
  if (mode == 1)
  {
    mode1:
    stopMotors();
    while (true)
    {
      if (IrReceiver.decode())
      {
        command = IrReceiver.decodedIRData.command;

        if (command == 70)
        {
          mode = 2;
          break;
        }

        switch (command)
        {
          case 24:
            moveForward();
            break;
          case 82:
            moveBackward();
            break;
          case 90:
            turnRight();
            break;
          case 8:
            turnLeft();
            break;
          case 28:
            stopMotors();
            break;
        }
        IrReceiver.resume();
      }
    } 
  }
  if (mode == 2)
  {
    if (IrReceiver.decode())
    {
      command = IrReceiver.decodedIRData.command;

      if (command == 69)
      {
        mode = 1;
        goto mode1;
      }
      IrReceiver.resume();
    }
    long distance, leftDistance, rightDistance;
    distance = calculateDistance();

    char lastTurn = 'R';
    int backForthTurns = 0;
    bool wentBack;

    if (distance <= 35 || wentBack)
    {
      stopMotors();
      rightDistance = calculateRightDistance();
      leftDistance = calculateLeftDistance();
      if (((rightDistance >= 50 || leftDistance >=50) && calculateDistance() >  10) || wentBack)
      {
        if (rightDistance > leftDistance)
        {
          turnRight();
          if (lastTurn == 'L')
          {
            lastTurn = 'R';
            backForthTurns++;
          }
          if (backForthTurns == 4)
          {
            turnRight();
            backForthTurns = 0;
          }
        }
        else
        {
          turnLeft();
          if (lastTurn == 'R')
          {
            lastTurn = 'L';
            backForthTurns++;
          }
          if (backForthTurns == 4)
          {
            turnLeft();
            backForthTurns = 0;
          }
        }
        wentBack = false;
      }
      else
      {
        moveBackward();
        wentBack = true;
        delay(400);
      }
    }
    else
    {
      moveForward();
    }
  }
}
