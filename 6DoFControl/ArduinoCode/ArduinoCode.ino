#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>  // Include the Wire library for I2C communication

const char* ssid = "Nov";                   // Your WiFi SSID
const char* password = "sai012345";         // Your WiFi Password
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_topic = "slider_values";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  // Initialize I2C
  Wire.begin();  // Start I2C communication (default pins: SDA = 21, SCL = 22)

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
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  // Print the message to the serial monitor
  Serial.print("Received message: ");
  Serial.println(message);

  // Send the received message to the Arduino via I2C
  sendToArduino(message);
}

void sendToArduino(String message) {
  Wire.beginTransmission(8);  // Send data to the Arduino at address 8
  for (int i = 0; i < message.length(); i++) {
    Wire.write(message[i]); // Send each character of the string
  }
  Wire.endTransmission();      // End the transmission
  Serial.println("Message sent to Arduino via I2C");
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
