#include <Servo.h>

const int servoPin = 9; // PWM pin to control the servo
Servo myServo; // Create a Servo object

void setup() {
Serial.begin(9600); // Initialize serial communication at 9600 baud rate
myServo.attach(servoPin); // Attach the servo to the pin
}

void loop() {
// Move the servo from -90 to 90 degrees
for (int setPosition = -90; setPosition <= 90; setPosition++) {
// Map the position to pulse width according to the given specification
float pulseWidthUs = map(setPosition, -90, 90, 500, 2400);
// Write the pulse width to the servo
myServo.writeMicroseconds(pulseWidthUs);

// Delay to allow the servo to reach the position
delay(20); // Pulse cycle is approximately 20ms

// Read the pulse width from the servo
unsigned long measuredPulseWidth = pulseIn(servoPin, HIGH, 25000); // Maximum pulse width is 2400us, so timeout is set to 25000us (25ms)

// Convert pulse width from microseconds to milliseconds
float measuredPulseWidthMs = measuredPulseWidth / 1000.0;

// Convert the measured pulse width back to position
float measuredPosition = map(measuredPulseWidth, 500, 2400, -90, 90);

// Convert the calculated pulse width from microseconds to milliseconds
float calculatedPulseWidthMs = pulseWidthUs / 1000.0;

// Output the set position, calculated pulse width, measured pulse width, and measured position
Serial.print("Set Position (degrees): ");
Serial.print(setPosition);
Serial.print(" -> Calculated Pulse Width (ms): ");
Serial.print(calculatedPulseWidthMs);
Serial.print(" -> Measured Pulse Width (ms): ");
Serial.print(measuredPulseWidthMs);
Serial.print(" -> Measured Position (degrees): ");
Serial.println(measuredPosition);

// Check if the servo has reached -90 or 90 degrees
if (setPosition == -90 || setPosition == 90) {
  delay(3000); // Keep the servo closed for 3s
}
}
}