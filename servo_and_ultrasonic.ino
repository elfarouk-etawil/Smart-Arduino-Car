#include <Servo.h>

#define SERVO_WAIT_TIME 400

const byte trigPin = A1; 
const byte echoPin = A2;

Servo servo;


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

long calculateRightDistance()
{
  servo.write(45);
  delay(SERVO_WAIT_TIME);
  long distance = calculateDistance();
  servo.write(90);
  delay(SERVO_WAIT_TIME);
  return distance;
}

long calculateLeftDistance()
{
  servo.write(135);
  delay(SERVO_WAIT_TIME);
  long distance = calculateDistance();
  servo.write(90);
  delay(SERVO_WAIT_TIME);
  return distance;
}

void setup()
{
Serial.begin (9600);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
servo.attach(4);
servo.write(90);
delay(50);
}

void loop()
{
  long distance, leftDistance, rightDistance;
  distance = calculateDistance();
  
  if (distance <= 30)
  {
    rightDistance = calculateRightDistance();
    leftDistance = calculateLeftDistance();
  }
  delay(2000);
  servo.write(90);
  delay(500);
}
