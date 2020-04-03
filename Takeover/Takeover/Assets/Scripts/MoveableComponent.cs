using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveableComponent : MonoBehaviour
{
    private readonly int mapLayerMask = 1 << 9;

    [SerializeField] private float speed;
    private List<MapNode> path;
	private MapController mapController;
	private TileOccupierController occupierController;

    // Start is called before the first frame update
    void Start()
    {
		//TODO: replace mapnodes with vector3
		this.path = new List<MapNode>();
		this.mapController = FindObjectOfType<MapController>();
		this.occupierController = this.GetComponent<TileOccupierController>();
    }

	private void OnEnable()
	{
		TurnManager.AddTurnEvent(this.OnTurn, TurnStage.Move);
	}

	private void OnDisable()
	{
		TurnManager.RemoveTurnEvent(this.OnTurn, TurnStage.Move);
	}

	//Gradually moves the entity towards the next node in the path
	IEnumerator MoveTowardsTarget()
	{
		using (TurnBlocker block = new TurnBlocker())
		{
			while (true)
			{
				Ray r = new Ray(new Vector3(this.transform.position.x, 5f, this.transform.position.z), Vector3.down);

				float height = this.transform.position.y;
				Vector3 normal = Vector3.up;

				if (Physics.Raycast(r, out RaycastHit hit, 10.0f, mapLayerMask))
				{
					height = hit.point.y;
					normal = hit.normal;
				}

				Vector3 direction = (this.path[0].Location - this.transform.position);
				direction.y = 0;

				float moveAmount = this.speed * Time.deltaTime;

				if (direction.magnitude <= moveAmount)
				{
					this.transform.position = this.path[0].Location;
					this.path.RemoveAt(0);

					break;
				}
				else
				{
					this.transform.rotation = Quaternion.FromToRotation(Vector3.up, normal) * Quaternion.LookRotation(direction);
					this.transform.Translate(direction.normalized * moveAmount, Space.World);
					Vector3 v = this.transform.position;
					v.y = height;
					this.transform.position = v;

					yield return null;
				}
			}
		}
	}

	//Called when a turn is processed
	void OnTurn()
	{
		//Advance path target
		if (this.path.Count > 0)
		{
			Vector2Int newLoc = MapController.LocationToMapIndex(this.path[0].Location);

			if (this.occupierController.CanMoveTo(newLoc))
			{
				this.occupierController.MoveTo(newLoc);
				this.StartCoroutine(this.MoveTowardsTarget());
			}
			else
			{
				Debug.Log("Path occupied, cannot move");
			}
		}
	}

    public void IssueMoveCommand(Vector3 newTarget)
    {
		TileOccupierController c = this.mapController.GetOccupier(MapController.LocationToMapIndex(newTarget));

		if (c == null)
		{
			MapNode goal = this.mapController.GetMapNode(new Vector2Int(Mathf.FloorToInt(newTarget.x), Mathf.FloorToInt(newTarget.z)));
			MapNode start = this.mapController.GetMapNode(new Vector2Int(Mathf.FloorToInt(this.transform.position.x), Mathf.FloorToInt(this.transform.position.z)));
			//Vector3 target = new Vector3(Mathf.Floor(newTarget.x) + 0.5f, newTarget.y, Mathf.Floor(newTarget.z) + 0.5f);

			this.mapController.Pathfind(start, goal, ref this.path);

			if (this.path.Count > 0)
			{
				this.path.RemoveAt(0);
			}
		}
		else
		{
			Debug.Log("Tile occupied");
		}
	}

	public void GetPath(out List<Vector3> path)
	{
		path = new List<Vector3>();

		foreach (MapNode n in this.path)
		{
			path.Add(n.Location);
		}
	}
}
