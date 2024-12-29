
import cv2
import mediapipe as mp
import math
import json
import paho.mqtt.client as mqtt

# Initialize MediaPipe Hands module
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(min_detection_confidence=0.7, min_tracking_confidence=0.7)
mp_drawing = mp.solutions.drawing_utils

# Function to calculate distance between two points
def calculate_distance(p1, p2):
    return math.sqrt((p2[0] - p1[0])**2 + (p2[1] - p1[1])**2)

# Open webcam
cap = cv2.VideoCapture(0)

# Slider and Button Configuration
num_sliders = 6
slider_width = 300
slider_height = 10
slider_spacing = 60
button_width = 60
button_height = 30

# Initial Y-position for sliders and button arrangement
slider_start_y = 120
button_start_y = 20

# Initialize slider values
slider_values = [50] * num_sliders

# Initialize button states
button_states = [False] * num_sliders
button_colors = [(0, 0, 255)] * num_sliders

# MQTT Configuration
mqtt_broker = "broker.hivemq.com"  # Replace with your MQTT broker address
mqtt_port = 1883
mqtt_topic = "slider_values"  # Replace with your MQTT topic
mqtt_client = mqtt.Client()

# Connect to the MQTT broker
mqtt_client.connect(mqtt_broker, mqtt_port)



# Function to reset all buttons except the one selected
def reset_button_states(selected_index):
    global button_states, button_colors
    for i in range(num_sliders):
        if i == selected_index:
            button_states[i] = True
            button_colors[i] = (0, 255, 0)
        else:
            button_states[i] = False
            button_colors[i] = (0, 0, 255)

while cap.isOpened():
    ret, frame = cap.read()
    
    if not ret:
        break
    
    # Flip the frame horizontally for a more natural interaction
    frame = cv2.flip(frame, 1)
    
    # Convert the frame to RGB for MediaPipe
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    results = hands.process(rgb_frame)
    
    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            index_finger = (hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP].x,
                            hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP].y)
            
            index_x = int(index_finger[0] * frame.shape[1])
            index_y = int(index_finger[1] * frame.shape[0])
            
            mp_drawing.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)

            for i in range(num_sliders):
                slider_left = 50
                slider_right = slider_left + slider_width
                slider_top = slider_start_y + i * slider_spacing

                button_left = 50 + i * (button_width + 10)
                button_right = button_left + button_width
                button_top = button_start_y
                button_bottom = button_top + button_height

                if button_left < index_x < button_right and button_top < index_y < button_bottom:
                    if not button_states[i]:
                        reset_button_states(i)
                else:
                    button_colors[i] = (0, 255, 0) if button_states[i] else (0, 0, 255)

                if button_states[i] and index_y > slider_top - 50 and index_y < slider_top + 50:
                    slider_values[i] = int(min(max((index_x - slider_left) / (slider_right - slider_left) * 100, 0), 100))
    
    for i in range(num_sliders):
        slider_left = 50
        slider_right = slider_left + slider_width
        slider_top = slider_start_y + i * slider_spacing

        cv2.rectangle(frame, (slider_left, slider_top - 10), (slider_right, slider_top + 10), (255, 255, 255), 2)
        cv2.rectangle(frame, (slider_left, slider_top - 10),
                      (slider_left + int((slider_right - slider_left) * (slider_values[i] / 100)), slider_top + 10),
                      (0, 255, 0), -1)
        
        cv2.putText(frame, f"Slider {i+1}: {slider_values[i]}", (10, slider_top - 30),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1, cv2.LINE_AA)
        
        button_left = 50 + i * (button_width + 10)
        button_right = button_left + button_width
        button_top = button_start_y
        button_bottom = button_top + button_height
        cv2.rectangle(frame, (button_left, button_top), (button_right, button_bottom), button_colors[i], -1)
        cv2.putText(frame, "Select", (button_left + 5, button_top + 20),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1, cv2.LINE_AA)


    # Prepare JSON data
    json_data = {}
    for i in range(num_sliders):
        json_data[f'val{i+1}'] = slider_values[i]

    # Publish JSON data to MQTT topic
    mqtt_client.publish(mqtt_topic, json.dumps(json_data))

    # Show the frame
    cv2.imshow("Multiple Sliders and Buttons", frame)
    
    if cv2.waitKey(1) & 0xFF == 27:
        break

# Release resources
cap.release()
cv2.destroyAllWindows()
mqtt_client.disconnect()
