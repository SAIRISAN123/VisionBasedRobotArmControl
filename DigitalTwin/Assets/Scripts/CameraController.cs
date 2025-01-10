using UnityEngine;

public class CameraController : MonoBehaviour
{
    public Transform fixedView; // The fixed camera view position and rotation
    private bool isFixedView = false; // Start with free move mode

    public float moveSpeed = 5f; // Speed of camera movement in free mode
    public float scrollSpeed = 2f; // Speed adjustment with scroll wheel
    public float mouseSensitivity = 100f; // Mouse sensitivity for camera rotation

    private float rotationX = 0f; // Vertical rotation
    private float rotationY = 0f; // Horizontal rotation

    void Start()
    {
        // Lock the cursor for better camera control
        Cursor.lockState = CursorLockMode.Locked;
        Cursor.visible = false;

        if (!isFixedView && Camera.main != null)
        {
            // Set initial rotation values based on current camera rotation
            rotationX = Camera.main.transform.eulerAngles.x;
            rotationY = Camera.main.transform.eulerAngles.y;
        }
        else if (fixedView != null)
        {
            Debug.LogError("Fixed view is not assigned.");
        }
    }

    void Update()
    {
        // Toggle between fixed and free movement modes
        if (Input.GetKeyDown(KeyCode.Space))
        {
            isFixedView = !isFixedView;

            if (isFixedView && fixedView != null)
            {
                SetToFixedView();
            }
        }

        if (!isFixedView)
        {
            FreeMove();
            AdjustCameraWithMouse();
        }
    }

    void SetToFixedView()
    {
        // Snap the camera to the fixed view position and rotation
        transform.position = fixedView.position;
        transform.rotation = fixedView.rotation;

        // Reset rotation values
        rotationX = transform.eulerAngles.x;
        rotationY = transform.eulerAngles.y;
    }

    void FreeMove()
    {
        // Adjust move speed with the scroll wheel
        float scrollInput = Input.GetAxis("Mouse ScrollWheel");
        if (scrollInput != 0)
        {
            moveSpeed += scrollInput * scrollSpeed;
            moveSpeed = Mathf.Clamp(moveSpeed, 1f, 20f); // Clamp the speed between 1 and 20
        }

        // Handle WASD movement
        float moveX = Input.GetAxis("Horizontal") * moveSpeed * Time.deltaTime; // A/D or Left/Right
        float moveZ = Input.GetAxis("Vertical") * moveSpeed * Time.deltaTime;   // W/S or Up/Down

        // Move the camera in its local space
        transform.Translate(new Vector3(moveX, 0, moveZ), Space.Self);
    }

    void AdjustCameraWithMouse()
    {
        // Get mouse movement input
        float mouseX = Input.GetAxis("Mouse X") * mouseSensitivity * Time.deltaTime;
        float mouseY = Input.GetAxis("Mouse Y") * mouseSensitivity * Time.deltaTime;

        // Update rotation values
        rotationY += mouseX;
        rotationX -= mouseY;
        rotationX = Mathf.Clamp(rotationX, -90f, 90f); // Limit vertical rotation

        // Apply rotation to the camera
        transform.localRotation = Quaternion.Euler(rotationX, rotationY, 0f);
    }
}
