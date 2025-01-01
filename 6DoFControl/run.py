import subprocess

app = [
    "6DoFControl\Server\mqttSubsciption.py",          
    "6DoFControl\Server\websocketServer.py"   ]
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
