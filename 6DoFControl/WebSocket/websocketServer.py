import json
import asyncio
import websockets
import os

async def send_json_data(connection, json_file_path, last_data):
    last_modified_time = os.path.getmtime(json_file_path)
    
    while True:
        await asyncio.sleep(0.1)
        
        current_modified_time = os.path.getmtime(json_file_path)
        if current_modified_time != last_modified_time:
            last_modified_time = current_modified_time
            
            with open(json_file_path, 'r') as file:
                content = file.read()
                
                if not content:  
                    print("Error: JSON file is empty.")
                    await connection.send(json.dumps(last_data))  # Send previous data
                    continue
                    
                data = json.loads(content)
                print("JSON data loaded successfully:", data)
                last_data = data  # Update last_data with the current data
                await connection.send(json.dumps(data))  

async def handle_connection(connection):
    json_file_path = 'shared_data.json'
    last_data = {}  # Initialize last_data

    if not os.path.exists(json_file_path):
        print(f"File not found: {json_file_path}")
        await connection.send("Error: JSON file not found.")
        return
        
    await send_json_data(connection, json_file_path, last_data)
        
async def websocket_server():
    server = await websockets.serve(handle_connection, "localhost", 8765)
    print("WebSocket server is running on ws://localhost:8765")
    await server.wait_closed()

if __name__ == "__main__":
    asyncio.run(websocket_server())
