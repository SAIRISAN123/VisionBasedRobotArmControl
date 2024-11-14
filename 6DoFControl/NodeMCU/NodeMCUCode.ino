#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Nov";
const char* password = "sai012345";
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_topic = "slider_values";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);                // Serial for debugging
  Serial1.begin(9600, SERIAL_8N1, -1, 17); // Serial1 for TX (GPIO 17 on ESP32 to Arduino RX)

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
  // Parse JSON payload
  StaticJsonDocument<256> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, payload, length);
  
  if (error) {
    Serial.print("JSON Parsing failed: ");
    Serial.println(error.c_str());
    return;
  }

  // Extract slider values
  int sliderValues[6];
  for (int i = 0; i < 6; i++) {
    sliderValues[i] = jsonDoc["sliders"][i]["value"].as<int>();
  }

  // Send values to Arduino over Serial1
  for (int i = 0; i < 6; i++) {
    Serial1.print(sliderValues[i]);
    if (i < 5) Serial1.print(","); // Comma-separated values
  }
  Serial1.println(); // Newline indicates end of transmission

  // Print for debugging
  Serial.print("Sent values to Arduino: ");
  for (int i = 0; i < 6; i++) {
    Serial.print(sliderValues[i]);
    if (i < 5) Serial.print(", ");
  }
  Serial.println();
}

void reconnect() {
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
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
