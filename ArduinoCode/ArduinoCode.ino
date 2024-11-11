#include <Servo.h>

Servo myServo;  // Create a servo object

void setup() {
  pinMode(A1, INPUT);         // Set A1 as input (connect to 3.3V source)
  myServo.attach(9);          // Attach servo to pin 9
  Serial.begin(9600);         // Begin serial communication for debugging
}

void loop() {
  // Read the analog input from A1 (0 to 1023 range on a 10-bit ADC)
  int analogValue = analogRead(A1);

  // Map the 0-675 analog reading (representing 0-3.3V) to the servo range in reverse (180-0 degrees)
  int servoAngle = map(analogValue, 0, 675, 270, 0);
  
  // Constrain the servo angle to ensure it's within bounds
  servoAngle = constrain(servoAngle, 0, 180);

  // Write the angle to the servo
  myServo.write(servoAngle);
  
  // Print the values for debugging
  Serial.print("Analog Value: ");
  Serial.print(analogValue);
  Serial.print(" | Servo Angle: ");
  Serial.println(servoAngle);

  delay(10); // Small delay for stability
}
