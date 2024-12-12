const byte RightFront = 6;
const byte RightBack = 11;
const byte LeftBack = 3;
const byte LeftFront = 5;

const byte lowSpeed = 75;
const byte midSpeed = 150;
const byte maxSpeed = 255; 


// Function to move forward
void moveForward(int duration, byte speed) {
  analogWrite(RightFront, speed);
  analogWrite(RightBack, 0);
  analogWrite(LeftFront, speed);
  analogWrite(LeftBack, 0);
  delay(duration);
}

// Function to move backward
void moveBackward(int duration, byte speed) {
  analogWrite(RightFront, 0);
  analogWrite(RightBack, speed);
  analogWrite(LeftFront, 0);
  analogWrite(LeftBack, speed);
  delay(duration);
}

// Function to turn left
void turnLeft(int duration, byte speed) {
  analogWrite(RightFront, 0);
  analogWrite(RightBack, speed);
  analogWrite(LeftFront, speed);
  analogWrite(LeftBack, 0);
  delay(duration);
}

// Function to turn right
void turnRight(int duration, int speed) {
  analogWrite(RightFront, speed);
  analogWrite(RightBack, 0);
  analogWrite(LeftFront, 0);
  analogWrite(LeftBack, speed);
  delay(duration);
}

// Function to stop motors
void stopMotors(int duration = 0) {
  analogWrite(RightFront, 0);
  analogWrite(RightBack, 0);
  analogWrite(LeftFront, 0);
  analogWrite(LeftBack, 0);
  if (duration > 0) delay(duration);
}


void setup() {
  // Set motor pins as outputs
  pinMode(RightFront, OUTPUT);
  pinMode(RightBack, OUTPUT);
  pinMode(LeftFront, OUTPUT);
  pinMode(LeftBack, OUTPUT);

  // Initialize all motors to off
  stopMotors();
}

void loop() {

  byte speed = midSpeed;

  moveForward(1000, speed);  // Move forward for 1 second
  moveBackward(1000, speed); // Move backward for 1 second
  turnLeft(1000, speed);     // Turn left for 1 second
  turnRight(1000, speed);    // Turn right for 1 second
  stopMotors(1000);   // Stop for 1 second
}


