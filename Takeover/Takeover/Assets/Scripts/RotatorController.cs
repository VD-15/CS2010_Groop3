using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RotatorController : MonoBehaviour
{
	[SerializeField] private Vector3 direction;
	[SerializeField] private float speed;

    private void Update()
    {
		this.transform.Rotate(this.direction, Time.deltaTime * this.speed, Space.Self);
    }
}
