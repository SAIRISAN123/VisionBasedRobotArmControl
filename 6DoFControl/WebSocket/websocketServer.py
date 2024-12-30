import asyncio
import websockets
import json
import sys
import os

Example = {'val1': 50, 'val2': 75, 'val3': 25}






async def handle_connection(websocket):
    print("Client connected")
    try:
        while True:

            data_to_send = json.dumps(Example)
            await websocket.send(data_to_send)
            print(f"Sent data: {data_to_send}")
            # try:
            #     message = await asyncio.wait_for(websocket.recv(), timeout=5)
            #     print(f"Received from client: {message}")
            # except asyncio.TimeoutError:
            #     print("No message received from the client within the timeout period.")
            await asyncio.sleep(1)

    except websockets.ConnectionClosed as e:
            print(f"Client disconnected: {e}")

async def ConnectwebSocket():
    server = await websockets.serve(handle_connection, "localhost", 8765)
    print("WebSocket server is running on ws://localhost:8765")
    await server.wait_closed()


# if __name__ == "__main__":
#     asyncio.run(ConnectwebSocket())