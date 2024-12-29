import asyncio
import json
import threading
import os
import sys

import websockets

# Add the ComputerVision folder to the system path
sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'ComputerVision'))

# Import the json_data from Vision
from Vision import json_data


clients = {}

data = {}
data = json_data




async def websocket_server(websocket, path):
    if path not in clients:
        clients[path] = set()
    clients[path].add(websocket)
    print(f"Client connected to {path}")
    try:
        async for message in websocket:
            pass
    finally:
        clients[path].remove(websocket)
        if not clients[path]:
            del clients[path]


def run_websocket_server():
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    start_server = websockets.serve(websocket_server, "localhost", 8765)
    loop.run_until_complete(start_server)
    loop.run_forever()


def send_through_websocket(data):
    data = json.dumps(data)
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    loop.run_until_complete(send_to_all_clients(data, "/"))



def send_slider_data_through_websocket(data):
    data = json.dumps(data)
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    loop.run_until_complete(send_to_all_clients(data, "/sliders"))


async def send_to_all_clients(data, path):
    if path in clients and clients[path]:
        await asyncio.gather(*(client.send(data) for client in clients[path]))
        print(f"Sent data to {len(clients[path])} clients")


# Start the WebSocket server in a thread
server_thread = threading.Thread(target=run_websocket_server)
server_thread.daemon = True
server_thread.start()
print("WebSocket server started")


while True:
    send_slider_data_through_websocket(data)
    print("Data sent")
    asyncio.sleep(1)
