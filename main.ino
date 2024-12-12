#include <Servo.h>

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

// Speed levels
byte speed;
const byte lowSpeed = 75;
const byte midSpeed = 150;
const byte maxSpeed = 255; 

// Turning time depends on speed
short turnTime;

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
void turnLeft() {
  analogWrite(RightFront, 0);
  analogWrite(RightBack, speed);
  analogWrite(LeftFront, speed);
  analogWrite(LeftBack, 0);
  delay(turnTime);
}


// Function to turn right
void turnRight() {
  analogWrite(RightFront, speed);
  analogWrite(RightBack, 0);
  analogWrite(LeftFront, 0);
  analogWrite(LeftBack, speed);
  delay(turnTime);
}


// Function to stop motors
void stopMotors() {
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
}

void loop() {

  speed = maxSpeed;
  switch(speed)
  {
    case maxSpeed:
      turnTime = 250;
      break;
    case midSpeed:
      turnTime = 400;
      break;
    case lowSpeed:
      turnTime = 600;
      break;
  }

  long distance, leftDistance, rightDistance;
  distance = calculateDistance();

  // Ignore readings that are out of a reasonable range (e.g., 0 cm or > 400 cm).
  if (distance < 5 || distance > 400)
  {
    distance = 400; // Treat as no obstacle
  }

  if (distance <= 30)
  {
    while(true)
    {
      stopMotors();
      rightDistance = calculateRightDistance();
      leftDistance = calculateLeftDistance();
      if (rightDistance >= 50 || leftDistance >=50)
      {
        if (rightDistance > leftDistance)
        {
          turnRight();
        }
        else
        {
          turnLeft();
        }
        break;
      }
      else
      {
        moveBackward();
        delay(400);
      }
    }
  }
  else
  {
    moveForward();
  }
}
