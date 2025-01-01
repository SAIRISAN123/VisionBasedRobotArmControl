import paho.mqtt.client as mqtt
import json

BROKER = "broker.hivemq.com"
PORT = 1883
TOPIC = "slider_values"
client = mqtt.Client()

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to HiveMQ broker!")
        client.subscribe(TOPIC)
        print(f"Subscribed to topic: {TOPIC}")
    else:
        print(f"Failed to connect, return code {rc}")

def on_message(client, userdata, msg):
    data_sliders = json.loads(msg.payload.decode())
    #print(f"Message received on topic {msg.topic}: {data_sliders}")
    
    # Write the received data to a JSON file
    with open("shared_data.json", "w") as file:
        json.dump(data_sliders, file)

client.on_connect = on_connect
client.on_message = on_message

try:
    print("Connecting to broker...")
    client.connect(BROKER, PORT, 60)
except Exception as e:
    print(f"Connection failed: {e}")
    exit(1)

client.loop_forever()
