#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Nov";                   // Your WiFi SSID
const char* password = "sai012345";         // Your WiFi Password
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_topic = "finger_distance";

WiFiClient espClient;
PubSubClient client(espClient);
const int dacPin = 25;                      // DAC output pin (use GPIO25 or GPIO26 on ESP32)

void setup() {
  Serial.begin(115200);

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
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  // Convert the received payload to integer distance value
  int distanceValue = message.toInt();
  distanceValue = constrain(distanceValue, 0, 255);  // Limit distanceValue to DAC range (0-255)
  
  
  // Set DAC output voltage based on distance value (0-255 for 0-3.3V)
  dacWrite(dacPin, distanceValue);

  // Print the values for debugging
  Serial.print("Received Distance: ");
  Serial.print(distanceValue);
  Serial.print(" -> DAC Output: ");
  Serial.println(distanceValue);
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
