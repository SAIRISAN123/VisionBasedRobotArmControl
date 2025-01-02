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
  int val1 = doc["val1"];
  int val2 = doc["val2"];
  int val3 = doc["val3"];
  int val4 = doc["val4"];
  int val5 = doc["val5"];
  int val6 = doc["val6"];

  // Map the slider values (0-100) to servo angles (0-180)
  servos[0].write(map(val1, 0, 100, 0, 180));
  servos[1].write(map(val2, 0, 100, 0, 180));
  servos[2].write(map(val3, 0, 100, 0, 180));
  servos[3].write(map(val4, 0, 100, 0, 180));
  servos[4].write(map(val5, 0, 100, 0, 180));
  servos[5].write(map(val6, 0, 100, 0, 180));
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
}
