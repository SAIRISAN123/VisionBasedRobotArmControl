import asyncio

import websockets


async def client():
    uri = "ws://localhost:8765/sliders"
    async with websockets.connect(uri) as websocket:
        while True:
            message = await websocket.recv()
            print(f"Received: {message}")


asyncio.get_event_loop().run_until_complete(client())
