using UnityEngine;

public class CameraSwitcher : MonoBehaviour
{
    public Transform view1; // First camera view
    public Transform view2; // Second camera view

    private bool isInView1 = true; // Tracks the current view

    void Start()
    {
        // Start with view1 if assigned
        if (view1 != null)
        {
            MoveToView(view1);
        }
        else
        {
            Debug.LogError("View1 is not assigned in the Inspector.");
        }
    }

    void Update()
    {
        // Check if the "1" key is pressed
        if (Input.GetKeyDown(KeyCode.Alpha1))
        {
            ToggleView();
        }
    }

    // Switches between the two views
    void ToggleView()
    {
        if (isInView1 && view2 != null)
        {
            MoveToView(view2);
            isInView1 = false;
        }
        else if (view1 != null)
        {
            MoveToView(view1);
            isInView1 = true;
        }
    }

    // Instantly move the camera to a specific view
    void MoveToView(Transform view)
    {
        transform.position = view.position;
        transform.rotation = view.rotation;
    }
}
