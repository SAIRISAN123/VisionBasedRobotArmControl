#include <Servo.h>

Servo servos[6];               
int servoPins[6] = {3, 5, 6, 9, 10, 11}; 
int sliderValues[6];            // Array to store parsed slider values

void setup() {
  Serial.begin(9600);           // Serial for receiving data from ESP32

  // Attach each servo to a pin
  for (int i = 0; i < 6; i++) {
    servos[i].attach(servoPins[i]);
  }
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n'); // Read incoming data until newline

    int index = 0;
    int pos = 0;

    // Parse comma-separated values
    while ((pos = data.indexOf(',')) > 0 && index < 6) {
      sliderValues[index++] = data.substring(0, pos).toInt();
      data = data.substring(pos + 1);
    }
    sliderValues[index] = data.toInt(); // Last value after the last comma

    // Map each slider value from 0-100 to 0-180 and write to servos
    for (int i = 0; i < 6; i++) {
      int angle = map(sliderValues[i], 0, 100, 0, 180);
      servos[i].write(angle);
    }

    // Print for debugging
    Serial.print("Received and mapped values: ");
    for (int i = 0; i < 6; i++) {
      Serial.print(sliderValues[i]);
      if (i < 5) Serial.print(", ");
    }
    Serial.println();
  }
}
