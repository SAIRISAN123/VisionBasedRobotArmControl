import subprocess

app = [
    "6DoFControl/ComputerVision/mqttSubsciption.py",          
    "6DoFControl/WebSocket/websocketServer.py",    ]
processes = []

try:
 
    for script in app:
        print(f"Starting {script}...")
        process = subprocess.Popen(["python", script])
        processes.append(process)

    for process in processes:
        process.wait()
except KeyboardInterrupt:
    print("\nTerminating all scripts...")
    for process in processes:
        process.terminate()
except Exception as e:
    print(f"An error occurred: {e}")
finally:
    for process in processes:
        process.kill()
    print("All scripts terminated.")
