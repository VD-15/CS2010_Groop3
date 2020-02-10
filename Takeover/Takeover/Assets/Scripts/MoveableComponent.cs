﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveableComponent : MonoBehaviour
{
    [SerializeField] private float speed;
    private List<Vector3> path;
	private MapController mapController;

    // Start is called before the first frame update
    void Start()
    {
        this.path = new List<Vector3>();
		this.mapController = FindObjectOfType<MapController>();
    }

    // Update is called once per frame
    void Update()
    {
        if (this.path.Count > 0)
        {
            Vector3 next = this.path[0];

            Vector3 direction = (next - this.transform.position);

            this.transform.rotation = Quaternion.LookRotation(direction, Vector3.up);

            float moveAmount = this.speed * Time.deltaTime;

            if (direction.magnitude <= moveAmount)
            {
                this.transform.position = next;
                this.path.RemoveAt(0);
            }
            else
            {
                this.transform.Translate(direction.normalized * moveAmount, Space.World);
            }
        }
    }

    public void IssueMoveCommand(Vector3 newTarget)
    {
		MapNode goal = this.mapController.GetMapNode(new Vector2Int(Mathf.FloorToInt(newTarget.x), Mathf.FloorToInt(newTarget.z)));
		MapNode start = this.mapController.GetMapNode(new Vector2Int(Mathf.FloorToInt(this.transform.position.x), Mathf.FloorToInt(this.transform.position.z)));
		//Vector3 target = new Vector3(Mathf.Floor(newTarget.x) + 0.5f, newTarget.y, Mathf.Floor(newTarget.z) + 0.5f);

		this.mapController.Pathfind(start, goal, ref this.path);

		//this.path.Add(goal.Location);
	}
}
