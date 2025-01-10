import paho.mqtt.client as mqtt

BROKER = "localhost" #"broker.hivemq.com"  
PORT = 1885                  
TOPIC = "temperature/1"      


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to HiveMQ broker!")
        client.subscribe(TOPIC) 
        print(f"Subscribed to topic: {TOPIC}")
    else:
        print(f"Failed to connect, return code {rc}")


def on_message(client, userdata, msg):
    print(f"Message received on topic {msg.topic}: {msg.payload.decode()}")


client = mqtt.Client()


client.on_connect = on_connect
client.on_message = on_message


try:
    print("Connecting to broker...")
    client.connect(BROKER, PORT, 60)
except Exception as e:
    print(f"Connection failed: {e}")
    exit(1)

client.loop_forever()
