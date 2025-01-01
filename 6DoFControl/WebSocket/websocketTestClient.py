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
                data = json.loads(data_received)  # Parse JSON data
                print(f"Received from server: {data}")
                
                # You can process or use the received data here
                # Example: Send a simple acknowledgment (uncomment if needed)
                # response = json.dumps({"status": "received"})
                # await websocket.send(response)
                # print(f"Sent to server: {response}")

    except websockets.ConnectionClosed as e:
        print(f"Connection closed: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    asyncio.run(communicate())
