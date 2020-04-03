using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//Controls camera movement
public class CameraController : MonoBehaviour
{
	//Maximum speed at which the camera moves horizontally in units per second
	[SerializeField] private float moveSpeed;

	//The amount the camera moves up/down when the mouse is scrolled
	[SerializeField] private float scrollAmount;

	//The speed at which the camera will move towards the y target
	[SerializeField] private float scrollSpeed;

	//The speed at which the camera will rotate
	[SerializeField] private float rotateSpeed;

	//Minimun height for the camera
	[SerializeField] private float yMin;

	//Maximum height for the camera
	[SerializeField] private float yMax;

	//The x bounds to which camera movement will be restricted in both directions
	[SerializeField] private float xClamp;

	//The z bounds to which camera movement will be restricted in both directions
	[SerializeField] private float zClamp;

	[SerializeField] private float minAngle;
	[SerializeField] private float maxAngle;

	//Direction the camera is moving in
	private Vector3 moveDir;

	//Y position the camera is moving towards
	private float targetY;

	//Child of this parent that owns a camera component
	private Camera cameraComp;

    // Start is called before the first frame update
    void Start()
    {
		this.moveDir = new Vector3();

		this.cameraComp = this.GetComponentInChildren<Camera>();
		this.targetY = this.transform.position.y;
    }

    // Update is called once per frame
    void Update()
    {
		this.MoveCamera();
		this.ClampCamera();
		this.RotateCamera();
	}

	void MoveCamera()
	{
		this.moveDir.Set(0f, 0f, 0f);
		float rotAmount = 0f;

		if (Input.GetKey(KeyCode.W)) this.moveDir.z += 1f;
		if (Input.GetKey(KeyCode.S)) this.moveDir.z -= 1f;
		if (Input.GetKey(KeyCode.A)) this.moveDir.x -= 1f;
		if (Input.GetKey(KeyCode.D)) this.moveDir.x += 1f;
		//if (Input.GetKey(KeyCode.Q)) rotAmount -= 1f;
		//if (Input.GetKey(KeyCode.E)) rotAmount += 1f;

		if (this.moveDir.sqrMagnitude > 1f) this.moveDir.Normalize();

		this.moveDir *= Time.deltaTime * this.moveSpeed;

		this.targetY += Input.mouseScrollDelta.y * this.scrollAmount;

		this.transform.Rotate(Vector3.up, rotAmount * rotateSpeed * Time.deltaTime);
		this.transform.Translate(this.moveDir, Space.Self);
	}

	void ClampCamera()
	{
		this.targetY = Mathf.Clamp(this.targetY, this.yMin, this.yMax);
		float newX = Mathf.Clamp(this.transform.position.x, -this.xClamp, this.xClamp);
		float newZ = Mathf.Clamp(this.transform.position.z, -this.zClamp, this.zClamp);

		float newY = Mathf.Lerp(this.transform.position.y, this.targetY, 0.1f);

		//this.transform.position.Set(...) won't work here :(
		this.transform.position = new Vector3(newX, newY, newZ);
	}

	void RotateCamera()
	{
		float newAngle = Mathf.LerpAngle(this.minAngle, this.maxAngle, Mathf.InverseLerp(this.yMin, this.yMax, this.transform.position.y));

		this.cameraComp.transform.localRotation = Quaternion.AngleAxis(newAngle, Vector3.right);
	}
}
