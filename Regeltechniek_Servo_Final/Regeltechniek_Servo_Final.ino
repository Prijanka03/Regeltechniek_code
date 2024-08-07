#include <Servo.h>

const int servoPin = 9; // PWM pin to control the servo
Servo myServo; // Create a Servo object

// PID constants
const float Kp = 0.25894; // Proportional gain
const float Ki = 11.6209; // Integral gain

// PID variables
float setPoint = 0; // Desired position in degrees
float input = 0; // Current position in degrees
float output = 0; // Output pulse width
float integral = 0; // Integral term

// Timing variables
unsigned long previousMillis = 0; // To keep track of time
const long moveDuration = 2000; // Time to move the servo (milliseconds)
const long waitDuration = 30000; // Time to wait at each position (milliseconds)
bool movingToOpen = true; // Flag to indicate if the servo is moving to the open position

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud rate
  myServo.attach(servoPin); // Attach the servo to the pin
}

void loop() {
  unsigned long currentMillis = millis();

  if (movingToOpen) {
    setPoint = -90; // Open position
  } else {
    setPoint = 90; // Closed position
  }

  // Move the servo to the set point
  if (currentMillis - previousMillis < moveDuration) {
    // Calculate proportional and integral control
    unsigned long measuredPulseWidth = pulseIn(servoPin, HIGH, 25000); // Read the current pulse width
    input = map(measuredPulseWidth, 500, 2400, -90, 90);

    float error = setPoint - input;
    integral += error;
    output = Kp * error + Ki * integral;

    float pulseWidthUs = map(output, -90, 90, 500, 2400);
    pulseWidthUs = constrain(pulseWidthUs, 500, 2400);

    myServo.writeMicroseconds(pulseWidthUs);

    float calculatedPulseWidthMs = pulseWidthUs / 1000.0;

    Serial.print("Set Position (degrees): ");
    Serial.print(setPoint);
    Serial.print(" -> Calculated Pulse Width (ms): ");
    Serial.print(calculatedPulseWidthMs);
    Serial.print(" -> Measured Pulse Width (ms): ");
    Serial.print(measuredPulseWidth / 1000.0);
    Serial.print(" -> Measured Position (degrees): ");
    Serial.println(input);
  } else {
    // Delay after reaching the set point
    if (currentMillis - previousMillis < (moveDuration + waitDuration)) {
      // Waiting period
      delay(20); // Short delay to keep the loop running smoothly
    } else {
      // Update timing for next action
      previousMillis = currentMillis;
      movingToOpen = !movingToOpen; // Toggle position
    }
  }
}
