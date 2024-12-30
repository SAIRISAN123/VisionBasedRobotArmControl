# run.py
import asyncio
import sys

from WebSocket.websocketServer import ConnectwebSocket



def main():
    asyncio.run(ConnectwebSocket())
    

    print("Application started successfully")

if __name__ == "__main__":
    main()

