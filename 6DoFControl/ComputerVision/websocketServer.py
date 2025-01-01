import asyncio
import websockets
import json
import os

connected_clients = set()

async def handle_connection(websocket):
    connected_clients.add(websocket)
    print("Client connected")
    try:
        while True:
            
            if os.path.exists("shared_data.json"):
                with open("shared_data.json", "r") as file:
                    data = json.load(file)
            else:
                data = {}

            
            data_to_send = json.dumps(data)
            await websocket.send(data_to_send)
            print(f"Sent data: {data_to_send}")
            await asyncio.sleep(1)

    except websockets.ConnectionClosed as e:
        print(f"Client disconnected: {e}")
        connected_clients.remove(websocket)

async def start_websocket_server():
    server = await websockets.serve(handle_connection, "localhost", 8765)
    print("WebSocket server is running on ws://localhost:8765")
    await server.wait_closed()

asyncio.run(start_websocket_server())
