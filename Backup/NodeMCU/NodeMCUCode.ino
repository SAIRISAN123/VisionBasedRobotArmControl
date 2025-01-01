#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Nov";                   // Your WiFi SSID
const char* password = "sai012345";         // Your WiFi Password
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_topic = "slider_values";


// this sample code provided by www.programmingboss.com
#define RXp2 16
#define TXp2 17






















WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);  // Start Serial communication with the computer
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);

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

  // Print the received message for debugging
  Serial.print("Message received: ");
  Serial.println(message);

  // Send the message to the Arduino via UART (Serial2)
  sendToArduino(message);
}

void sendToArduino(String message) {
  // Send the message as a string via UART (Serial2) to the Arduino
  Serial2.println(message);
  delay(10);

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
