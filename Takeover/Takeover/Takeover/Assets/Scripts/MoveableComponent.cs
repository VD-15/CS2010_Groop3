using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveableComponent : MonoBehaviour
{
    private readonly int mapLayerMask = 1 << 9;

    [SerializeField] private float speed;
    private List<MapNode> path;
	private MapController mapController;

    // Start is called before the first frame update
    void Start()
    {
        this.path = new List<MapNode>();
		this.mapController = FindObjectOfType<MapController>();
    }

    // Update is called once per frame
    void Update()
    {
        if (this.path.Count > 0)
        {
            MapNode next = this.path[0];

            //TODO: Height resolution via raycast

            Ray r = new Ray(new Vector3(this.transform.position.x, 5f, this.transform.position.z), Vector3.down);

            float height = this.transform.position.y;
            Vector3 normal = Vector3.up;

            if (Physics.Raycast(r, out RaycastHit hit, 10.0f, mapLayerMask))
            {
                height = hit.point.y;
                normal = hit.normal;
            }

            Vector3 direction = (next.Location - this.transform.position);
            direction.y = 0;

            this.transform.rotation = Quaternion.FromToRotation(Vector3.up, normal) * Quaternion.LookRotation(direction);

            float moveAmount = this.speed * Time.deltaTime;

            if (direction.magnitude <= moveAmount)
            {
                this.transform.position = next.Location;
                this.path.RemoveAt(0);
            }
            else
            {
                this.transform.Translate(direction.normalized * moveAmount, Space.World);
                Vector3 v = this.transform.position;
                v.y = height;
                this.transform.position = v;
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
