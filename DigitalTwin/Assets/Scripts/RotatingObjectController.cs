// using UnityEngine;
// using System;

// public class RotatingObjectController : MonoBehaviour
// {
//     WebSocketClient _webSocketClient;
//     public string ArmSlider = "";

//     public float minPosition = -45f;
//     public float maxPosition = 45f;

//     float rotationAmount = 0f;
//     public enum Axis
//     {
//         X,
//         Y,
//         Z
//     }
//     public Axis selectedAxis = Axis.X;

//     public class Message
//     {
//         public int val1;
//         public int val2;
//         public int val3;
//         public int val4;
//         public int val5;
//         public int val6;
//     }

//     void Start()
//     {
//         _webSocketClient = FindObjectOfType<WebSocketClient>();
//         if (_webSocketClient == null)
//         {
//             Debug.LogError("WebSocketClient not found in the scene.");
//         }
//     }

//     void Update()
//     {
//         if (_webSocketClient != null)
//         {
//             string message = _webSocketClient.MessageReceived;

//             if (string.IsNullOrEmpty(message))
//             {
//                 Debug.LogError("Received empty message from WebSocket.");
//                 return;
//             }

//             try
//             {
//                 Message msg = JsonUtility.FromJson<Message>(message);

//                 if (msg == null)
//                 {
//                     Debug.LogError("Failed to parse message.");
//                     return;
//                 }

//                 // Use the correct "val" based on the ArmSlider selected
//                 if (ArmSlider == "val1")
//                 {
//                     rotationAmount = msg.val1;
//                 }
//                 else if (ArmSlider == "val2")
//                 {
//                     rotationAmount = msg.val2;
//                 }
//                 else if (ArmSlider == "val3")
//                 {
//                     rotationAmount = msg.val3;
//                 }
//                 else if (ArmSlider == "val4")
//                 {
//                     rotationAmount = msg.val4;
//                 }
//                 else if (ArmSlider == "val5")
//                 {
//                     rotationAmount = msg.val5;
//                 }
//                 else if (ArmSlider == "val6")
//                 {
//                     rotationAmount = msg.val6;
//                 }

//                 // Map the value (0-100) to the rotation range (-45 to 45)
//                 float targetRotation = Mathf.Lerp(minPosition, maxPosition, rotationAmount / 100f);

//                 // Create a vector to apply the rotation on the selected axis
//                 Vector3 rotationVector = Vector3.zero;

//                 // Apply rotation based on selected axis
//                 if (selectedAxis == Axis.X)
//                 {
//                     rotationVector = Vector3.right;  // Rotate around X-axis
//                 }
//                 else if (selectedAxis == Axis.Y)
//                 {
//                     rotationVector = Vector3.up;  // Rotate around Y-axis
//                 }
//                 else if (selectedAxis == Axis.Z)
//                 {
//                     rotationVector = Vector3.forward;  // Rotate around Z-axis
//                 }

//                 // Apply rotation directly to the object's rotation
//                 gameObject.transform.rotation = Quaternion.Euler(
//                     selectedAxis == Axis.X ? targetRotation : gameObject.transform.eulerAngles.x,
//                     selectedAxis == Axis.Y ? targetRotation : gameObject.transform.eulerAngles.y,
//                     selectedAxis == Axis.Z ? targetRotation : gameObject.transform.eulerAngles.z
//                 );
//             }
//             catch (Exception ex)
//             {
//                 Debug.LogError("Error parsing message: " + ex.Message);
//             }
//         }
//         else
//         {
//             Debug.LogError("_webSocketClient is not assigned.");
//         }
//     }
// }





// using UnityEngine;
// using System;

// public class RotatingObjectController : MonoBehaviour
// {
//     WebSocketClient _webSocketClient;
//     public string ArmSlider = "";

//     public float minPosition = -45f;
//     public float maxPosition = 45f;

//     float rotationAmount = 0f;
//     float currentRotationAmount = 0f;  // Stores the current value for smooth rotation
//     public float rotationSpeed = 1f;  // Speed variable to control the rotation speed

//     public enum Axis
//     {
//         X,
//         Y,
//         Z
//     }
//     public Axis selectedAxis = Axis.X;

//     public class Message
//     {
//         public int val1;
//         public int val2;
//         public int val3;
//         public int val4;
//         public int val5;
//         public int val6;
//     }

//     void Start()
//     {
//         _webSocketClient = FindObjectOfType<WebSocketClient>();
//         if (_webSocketClient == null)
//         {
//             Debug.LogError("WebSocketClient not found in the scene.");
//         }
//     }

//     void Update()
//     {
//         if (_webSocketClient != null)
//         {
//             string message = _webSocketClient.MessageReceived;

//             if (string.IsNullOrEmpty(message))
//             {
//                 Debug.LogError("Received empty message from WebSocket.");
//                 return;
//             }

//             try
//             {
//                 Message msg = JsonUtility.FromJson<Message>(message);

//                 if (msg == null)
//                 {
//                     Debug.LogError("Failed to parse message.");
//                     return;
//                 }

                
//                 if (ArmSlider == "val1")
//                 {
//                     rotationAmount = msg.val1;
//                 }
//                 else if (ArmSlider == "val2")
//                 {
//                     rotationAmount = msg.val2;
//                 }
//                 else if (ArmSlider == "val3")
//                 {
//                     rotationAmount = msg.val3;
//                 }
//                 else if (ArmSlider == "val4")
//                 {
//                     rotationAmount = msg.val4;
//                 }
//                 else if (ArmSlider == "val5")
//                 {
//                     rotationAmount = msg.val5;
//                 }
//                 else if (ArmSlider == "val6")
//                 {
//                     rotationAmount = msg.val6;
//                 }


//                 float targetRotation = Mathf.Lerp(minPosition, maxPosition, rotationAmount / 100f);


//                 currentRotationAmount = Mathf.SmoothStep(currentRotationAmount, targetRotation, rotationSpeed * Time.deltaTime);

                
//                 Vector3 rotationVector = Vector3.zero;

                
//                 if (selectedAxis == Axis.X)
//                 {
//                     rotationVector = Vector3.right; 
//                 }
//                 else if (selectedAxis == Axis.Y)
//                 {
//                     rotationVector = Vector3.up;  
//                 }
//                 else if (selectedAxis == Axis.Z)
//                 {
//                     rotationVector = Vector3.forward;  
//                 }

                
//                 gameObject.transform.rotation = Quaternion.Euler(
//                     selectedAxis == Axis.X ? currentRotationAmount : gameObject.transform.eulerAngles.x,
//                     selectedAxis == Axis.Y ? currentRotationAmount : gameObject.transform.eulerAngles.y,
//                     selectedAxis == Axis.Z ? currentRotationAmount : gameObject.transform.eulerAngles.z
//                 );
//             }
//             catch (Exception ex)
//             {
//                 Debug.LogError("Error parsing message: " + ex.Message);
//             }
//         }
//         else
//         {
//             Debug.LogError("_webSocketClient is not assigned.");
//         }
//     }
// }

















using UnityEngine;
using System;

public class RotatingObjectController : MonoBehaviour
{
    WebSocketClient _webSocketClient;
    public string ArmSlider = "";

    public float minPosition = -45f;
    public float maxPosition = 45f;

    float rotationAmount = 0f;
    float currentRotationAmount = 0f;  // Stores the current value for smooth rotation
    public float rotationSpeed = 1f;  // Speed variable to control the rotation speed

    public enum Axis
    {
        X,
        Y,
        Z
    }
    public Axis selectedAxis = Axis.X;

    public class Message
    {
        public int val1;
        public int val2;
        public int val3;
        public int val4;
        public int val5;
        public int val6;
    }

    void Start()
    {
        _webSocketClient = FindObjectOfType<WebSocketClient>();
        if (_webSocketClient == null)
        {
            Debug.LogError("WebSocketClient not found in the scene.");
        }
    }

    void Update()
    {
        if (_webSocketClient != null)
        {
            string message = _webSocketClient.MessageReceived;

            if (string.IsNullOrEmpty(message))
            {
                Debug.LogError("Received empty message from WebSocket.");
                return;
            }

            try
            {
                Message msg = JsonUtility.FromJson<Message>(message);

                if (msg == null)
                {
                    Debug.LogError("Failed to parse message.");
                    return;
                }

                // Determine the rotation amount based on ArmSlider selection
                switch (ArmSlider)
                {
                    case "val1": rotationAmount = msg.val1; break;
                    case "val2": rotationAmount = msg.val2; break;
                    case "val3": rotationAmount = msg.val3; break;
                    case "val4": rotationAmount = msg.val4; break;
                    case "val5": rotationAmount = msg.val5; break;
                    case "val6": rotationAmount = msg.val6; break;
                    default: Debug.LogError("Invalid ArmSlider value."); return;
                }

                // Map the value (0-100) to the rotation range (-45 to 45)
                float targetRotation = Mathf.Lerp(minPosition, maxPosition, rotationAmount / 100f);

                // Smooth rotation speed with easing function
                currentRotationAmount = Mathf.SmoothStep(currentRotationAmount, targetRotation, rotationSpeed * Time.deltaTime);

                // Create a vector to apply the rotation on the selected axis
                Vector3 rotationVector = Vector3.zero;

                // Apply rotation based on selected axis
                if (selectedAxis == Axis.X)
                {
                    rotationVector = new Vector3(currentRotationAmount, 0, 0);
                }
                else if (selectedAxis == Axis.Y)
                {
                    rotationVector = new Vector3(0, currentRotationAmount, 0);
                }
                else if (selectedAxis == Axis.Z)
                {
                    rotationVector = new Vector3(0, 0, currentRotationAmount);
                }

                // Apply the local rotation relative to the parent
                gameObject.transform.localRotation = Quaternion.Euler(rotationVector);
            }
            catch (Exception ex)
            {
                Debug.LogError("Error parsing message: " + ex.Message);
            }
        }
        else
        {
            Debug.LogError("_webSocketClient is not assigned.");
        }
    }
}
