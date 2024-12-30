import asyncio
import websockets
import json

async def communicate():
    uri = "ws://localhost:8765"
    try:
        async with websockets.connect(uri) as websocket:
            print("Connected to the server")
            while True:
                # Receive data from the server
                data_received = await websocket.recv()
                data = json.loads(data_received)
                print(f"Received from server: {data}")
                
                # Send a response to the server
                # response = "Acknowledged"
                # await websocket.send(response)
                # print(f"Sent to server: {response}")
    except Exception as e:
        print(f"Connection error: {e}")

if __name__ == "__main__":
    asyncio.run(communicate())
