import cv2
import mediapipe as mp
import paho.mqtt.client as mqtt
import math

# MQTT setup
MQTT_BROKER = "test.mosquitto.org"  # Replace with your MQTT broker address
MQTT_PORT = 1883
MQTT_TOPIC = "finger_distance"

client = mqtt.Client()
client.connect(MQTT_BROKER, MQTT_PORT, 60)

# MediaPipe hands setup
mp_hands = mp.solutions.hands
hands = mp_hands.Hands()

# OpenCV camera setup
cap = cv2.VideoCapture(0)

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        continue
    
    # Flip the frame horizontally for better user experience
    frame = cv2.flip(frame, 1)
    
    # Convert the frame to RGB
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    
    # Process the frame to detect hands
    result = hands.process(rgb_frame)

    if result.multi_hand_landmarks:
        for landmarks in result.multi_hand_landmarks:
            # Get the coordinates for the thumb tip and index tip (landmarks 4 and 8)
            thumb_tip = landmarks.landmark[mp_hands.HandLandmark.THUMB_TIP]
            index_tip = landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP]
            
            # Calculate the Euclidean distance between the thumb and index finger tips
            thumb_x, thumb_y = thumb_tip.x, thumb_tip.y
            index_x, index_y = index_tip.x, index_tip.y
            
            distance = math.sqrt((thumb_x - index_x) ** 2 + (thumb_y - index_y) ** 2)
            
            # Convert the distance to a suitable range (e.g., 0 to 100) for your servo control
            distance_value = int(distance * 1000)
            
            print (distance_value)
            # Publish the distance value to MQTT
            client.publish(MQTT_TOPIC, distance_value)
            
            # Draw landmarks on the frame for visualization
            for landmark in landmarks.landmark:
                x, y = int(landmark.x * frame.shape[1]), int(landmark.y * frame.shape[0])
                cv2.circle(frame, (x, y), 5, (0, 255, 0), -1)

            # Draw line between thumb and index fingers
            cv2.line(frame, 
                     (int(thumb_x * frame.shape[1]), int(thumb_y * frame.shape[0])), 
                     (int(index_x * frame.shape[1]), int(index_y * frame.shape[0])), 
                     (255, 0, 0), 2)
    
    # Show the frame with landmarks
    cv2.imshow("Hand Tracking", frame)
    
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
