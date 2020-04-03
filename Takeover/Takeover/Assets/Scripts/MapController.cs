using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum MapNodeSlopeDirection
{
	None,
	X,
	Y
}

public class MapNode
{
	public MapNode(Vector2Int xz, float y, Vector3 normal)
	{
		this.Location = new Vector3(xz.x + 0.5f, y, xz.y + 0.5f);
		this.neighbors = new List<MapNode>(4);
		this.Normal = normal;

		//Dot product shows how close the normal is to the up vector,
		//if the dot product is equal to 1, the normal is facing upward.
		//We use a value of 0.9 to allow some leeway.
		float dot = Vector3.Dot(normal, Vector3.up);
		//If the slope is flat
		if (dot > 0.9f)
		{
			this.Slope = MapNodeSlopeDirection.None;
		}
		else
		{
			//If the tile is sloping in the x direction
			if (Mathf.Abs(normal.x) > 0.01)
			{
				this.Slope = MapNodeSlopeDirection.X;
			}
			else
			{
				this.Slope = MapNodeSlopeDirection.Y;
			}
		}
	}

	public float Height { get { return this.Location.y; } }
	public MapNodeSlopeDirection Slope { get; private set; }
	public Vector3 Location { get; private set; }
	public Vector3 Normal { get; private set; }

	private readonly List<MapNode> neighbors;

	public void AddNeighbor(MapNode n)
	{
		this.neighbors.Add(n);
	}

	public void GetNeighbors(out MapNode[] nodes)
	{
		nodes = new MapNode[this.neighbors.Count];
		this.neighbors.CopyTo(nodes);
	}
}

public class MapController : MonoBehaviour
{
	[SerializeField] private int mapSize;

	private readonly int mapLayerMask = 1 << 9;

	private Dictionary<Vector2Int, MapNode> nodes;
	private Dictionary<Vector2Int, TileOccupierController> occupiers;

	public delegate void MapCompleteFun();
	public event MapCompleteFun OnMapComplete;

	public bool isComplete { get; private set; } = false;

    // Start is called before the first frame update
    void Start()
    {
		this.nodes = new Dictionary<Vector2Int, MapNode>();
		this.occupiers = new Dictionary<Vector2Int, TileOccupierController>();

		//First iteration creats map nodes
        for (int y = -this.mapSize; y < this.mapSize; y++)
		{
			for (int x = -this.mapSize; x < this.mapSize; x++)
			{
				Ray r = new Ray(new Vector3(x + 0.5f, 5.0f, y + 0.5f), Vector3.down);

				if (Physics.Raycast(r, out RaycastHit hit, 10.0f, this.mapLayerMask))
				{
					float height = hit.point.y;

					Vector2Int v = MapController.LocationToMapIndex(hit.point);
					this.nodes.Add(v, new MapNode(v, height, hit.normal));
				}
				else
				{
					Debug.LogError("Node raycast failed: " + x + " " + y);
				}
			}
		}

		//Second iteration links map nodes
		for (int y = -this.mapSize; y < this.mapSize; y++)
		{
			for (int x = -this.mapSize; x < this.mapSize; x++)
			{
				MapNode node = this.GetMapNode(new Vector2Int(x, y));

				 MapNode north = this.GetMapNode(new Vector2Int(x, y + 1));
				 MapNode south = this.GetMapNode(new Vector2Int(x, y - 1));
				 MapNode west = this.GetMapNode(new Vector2Int(x - 1, y));
				 MapNode east = this.GetMapNode(new Vector2Int(x + 1, y));

				if (node == null) continue;

				if (node.Slope == MapNodeSlopeDirection.None)
				{
					//Attempt to link to all directions
					if (north != null && north.Slope != MapNodeSlopeDirection.X && Mathf.Abs(north.Height - node.Height) < 0.325f) node.AddNeighbor(north);
					if (south != null && south.Slope != MapNodeSlopeDirection.X && Mathf.Abs(south.Height - node.Height) < 0.325f) node.AddNeighbor(south);
					if (west != null && west.Slope != MapNodeSlopeDirection.Y && Mathf.Abs(west.Height - node.Height) < 0.325f) node.AddNeighbor(west);
					if (east != null && east.Slope != MapNodeSlopeDirection.Y && Mathf.Abs(east.Height - node.Height) < 0.325f) node.AddNeighbor(east);
				}
				else if (node.Slope == MapNodeSlopeDirection.X)
				{
					//Attempt to link left & right
					if (west != null && west.Slope != MapNodeSlopeDirection.Y && Mathf.Abs(west.Height - node.Height) < 0.325f) node.AddNeighbor(west);
					if (east != null && east.Slope != MapNodeSlopeDirection.Y && Mathf.Abs(east.Height - node.Height) < 0.325f) node.AddNeighbor(east);
				}
				else
				{
					//Attempt to link up & down
					if (north != null && north.Slope != MapNodeSlopeDirection.X && Mathf.Abs(north.Height - node.Height) < 0.325f) node.AddNeighbor(north);
					if (south != null && south.Slope != MapNodeSlopeDirection.X && Mathf.Abs(south.Height - node.Height) < 0.325f) node.AddNeighbor(south);
				}
			}
		}

		this.isComplete = true;

		if (this.OnMapComplete != null)
		{
			OnMapComplete.Invoke();

			//Clear invocation list for safety
			foreach (MapCompleteFun f in OnMapComplete.GetInvocationList())
			{
				OnMapComplete -= f;
			}
		}
    }

	public TileOccupierController GetOccupier(Vector2Int location)
	{
		if (this.occupiers.ContainsKey(location))
		{
			return this.occupiers[location];
		}
		else
		{
			return null;
		}
	}

	public void SetOccupier(Vector2Int location, TileOccupierController controller)
	{
		this.occupiers[location] = controller;
	}

	public MapNode GetMapNode(Vector2Int location)
	{
		if (this.nodes.ContainsKey(location))
		{
			return this.nodes[location];
		}
		else
		{
			return null;
		}
	}

	public bool IsAdjacent(Vector2Int l1, Vector2Int l2)
	{
		MapNode m1 = this.GetMapNode(l1);
		MapNode m2 = this.GetMapNode(l2);

		//This method doesn't take corners into account
		m1.GetNeighbors(out MapNode[] neighbours);

		foreach (MapNode m in neighbours)
		{
			if (m.Location == m2.Location)
			{
				return true;
			}
		}

		return false;
	}

	public static Vector2Int LocationToMapIndex(Vector3 location)
	{
		return new Vector2Int(Mathf.FloorToInt(location.x), Mathf.FloorToInt(location.z));
	}

	public void Pathfind(MapNode start, MapNode goal, ref List<MapNode> pathOut)
	{
		if (start == null || goal == null)
		{
			Debug.LogWarning("Failed to pathfind from " + start.Location.ToString() + " to " + goal.Location.ToString());
			return;
		}

		List<MapNode> openSet = new List<MapNode>();
		openSet.Add(start);

		//For node n, cameFrom[n] is the node immediately preceding it on the cheapest path from start to n currently known.
		Dictionary<MapNode, MapNode> cameFrom = new Dictionary<MapNode, MapNode>();

		// For node n, gScore[n] is the cost of the cheapest path from start to n currently known.
		//Default value of infinity
		Dictionary<MapNode, float> gScore = new Dictionary<MapNode, float>();
		gScore[start] = 0f;

		//For node n, fScore[n] := gScore[n] + h(n).
		//Default value of infinity
		Dictionary<MapNode, float> fScore = new Dictionary<MapNode, float>();
		fScore[start] = 0f;

		while (openSet.Count != 0)
		{
			MapNode currentNode = openSet[0];
			float currentFScore = Mathf.Infinity;

			fScore.TryGetValue(currentNode, out currentFScore);

			//Get the node in openSet with the lowest fscore value;
			for (int i = 1; i < openSet.Count; i++)
			{
				MapNode node = openSet[i];

				if (!fScore.TryGetValue(node, out float f)) f = Mathf.Infinity;

				if (f < currentFScore)
				{
					currentNode = node;
					currentFScore = f;
				}
			}

			//Debug.Log("Examining node: " + currentNode.Location);

			if (currentNode == goal)
			{
				pathOut.Clear();
				pathOut.Add(currentNode);

				while (cameFrom.ContainsKey(currentNode))
				{
					currentNode = cameFrom[currentNode];
					pathOut.Add(currentNode);
				}

				pathOut.Reverse();

				return;
			}

			openSet.Remove(currentNode);

			currentNode.GetNeighbors(out MapNode[] neighbors);

			//Debug.Log("Examining " + neighbors.Length + " neighbors:");

			if (!gScore.TryGetValue(currentNode, out float tenativeGScore)) tenativeGScore = Mathf.Infinity;

			tenativeGScore += 1f;

			//Debug.Log("Tenative score: " + tenativeGScore);

			foreach (MapNode neighbor in neighbors)
			{
				if (this.GetOccupier(LocationToMapIndex(neighbor.Location))) continue;

				if (!gScore.TryGetValue(neighbor, out float neighborScore)) neighborScore = Mathf.Infinity;

				if (tenativeGScore < neighborScore)
				{
					cameFrom[neighbor] = currentNode;
					gScore[neighbor] = tenativeGScore;
					fScore[neighbor] = neighborScore + Vector3.Distance(goal.Location, neighbor.Location);

					if (!openSet.Contains(neighbor))
					{
						//Debug.Log("Added neighbor to open set: " + neighbor.Location);

						openSet.Add(neighbor);
					}
				}
			}
		}

		Debug.LogWarning("Failed to pathfind from " + start.Location.ToString() + " to " + goal.Location.ToString());
	}
}
