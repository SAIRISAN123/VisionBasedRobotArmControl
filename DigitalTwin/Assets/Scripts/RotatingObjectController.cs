using UnityEngine;

public class RotatingObjectController : MonoBehaviour
{
    public GameObject targetObject; // The object around which this object will rotate
    public Vector3 rotationAxis = Vector3.up; // The axis to rotate around (default is the Y-axis)
    public float rotationSpeed = 10f; // Speed of rotation in degrees per second

    private Vector3 offset; // Offset from the target object

    void Start()
    {
        if (targetObject == null)
        {
            Debug.LogError("Target object is not assigned!");
            enabled = false;
            return;
        }

        // Calculate the initial offset between this object and the target object
        offset = transform.position - targetObject.transform.position;
    }

    void Update()
    {
        if (targetObject != null)
        {
            // Maintain the fixed position relative to the target object
            transform.position = targetObject.transform.position + offset;

            // Rotate around the target object's position on the specified axis
            transform.RotateAround(targetObject.transform.position, rotationAxis, rotationSpeed * Time.deltaTime);
        }
    }
}
