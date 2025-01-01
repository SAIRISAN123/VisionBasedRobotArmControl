#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> // Include the ArduinoJson library for parsing JSON

const char* ssid = "WIFISSID";                   
const char* password = "Password";         
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "slider_values";

WiFiClient espClient;
PubSubClient client(espClient);

const int pwmPins[] = {5, 18, 19, 21, 22, 23}; 

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

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  reconnect();

  for (int i = 0; i < 6; i++) {
    pinMode(pwmPins[i], OUTPUT);
    analogWrite(pwmPins[i], 0); 
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

  
  int val1 = doc["val1"];
  int val2 = doc["val2"];
  int val3 = doc["val3"];
  int val4 = doc["val4"];
  int val5 = doc["val5"];
  int val6 = doc["val6"];


  // Set PWM values based on the slider values (map 0-100 to 0-255 for PWM)
  analogWrite(pwmPins[0], map(val1, 0, 100, 0, 255));
  analogWrite(pwmPins[1], map(val2, 0, 100, 0, 255));
  analogWrite(pwmPins[2], map(val3, 0, 100, 0, 255));
  analogWrite(pwmPins[3], map(val4, 0, 100, 0, 255));
  analogWrite(pwmPins[4], map(val5, 0, 100, 0, 255));
  analogWrite(pwmPins[5], map(val6, 0, 100, 0, 255));
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
