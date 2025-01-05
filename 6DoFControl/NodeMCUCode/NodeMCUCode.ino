#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> // Include the ArduinoJson library for parsing JSON
#include <ESP32Servo.h>  // Include the ESP32Servo library for servo control

const char* ssid = "Nov";                   // Your WiFi SSID
const char* password = "sai012345";         // Your WiFi Password
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "slider_values";

WiFiClient espClient;
PubSubClient client(espClient);

const int pwmPins[] = {5, 18, 19, 21, 22, 23};  // Example PWM pins for controlling servos
Servo servos[6];                               // Array to hold 6 Servo objects
int currentAngles[6] = {0, 0, 0, 0, 0, 0};      // Current angles of each servo
int targetAngles[6] = {0, 0, 0, 0, 0, 0};       // Target angles for each servo
const int stepDelay = 15;                       // Delay between each step for smoothness
unsigned long lastUpdate = 0;

void setup() {
  Serial.begin(115200);  // Start Serial communication with the computer

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // Setup MQTT server and callback function
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // Connect to MQTT broker
  reconnect();

  // Attach servos to their respective pins
  for (int i = 0; i < 6; i++) {
    servos[i].attach(pwmPins[i]);  // Attach servo to the respective pin
    servos[i].write(0);            // Set initial position to 0 degrees
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  // Print the received message for debugging
  Serial.print("Message received: ");
  Serial.println(message);

  // Parse the JSON message
  StaticJsonDocument<200> doc;  // Allocate a JSON document
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }

  // Get the slider values from the JSON
  targetAngles[0] = map(doc["val1"], 0, 100, 0, 180);
  targetAngles[1] = map(doc["val2"], 0, 100, 0, 180);
  targetAngles[2] = map(doc["val3"], 0, 100, 0, 180);
  targetAngles[3] = map(doc["val4"], 0, 100, 0, 180);
  targetAngles[4] = map(doc["val5"], 0, 100, 0, 180);
  targetAngles[5] = map(doc["val6"], 0, 100, 0, 180);
}

void reconnect() {
  // Attempt to connect to MQTT broker
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32_DAC")) {
      Serial.println("connected");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 second");
      delay(1000);
    }
  }
}

void loop() {
  // Reconnect to MQTT if connection is lost
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Smoothly update servo positions
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= stepDelay) {
    lastUpdate = currentMillis;
    for (int i = 0; i < 6; i++) {
      if (currentAngles[i] != targetAngles[i]) {
        int step = (targetAngles[i] > currentAngles[i]) ? 1 : -1;
        currentAngles[i] += step;  // Move one step closer to the target
        servos[i].write(currentAngles[i]);  // Update servo position
      }
    }
  }
}
