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
	public MapNode(Vector2Int xz, float y, MapNodeSlopeDirection slopeDirection)
	{
		this.Location = new Vector3(xz.x + 0.5f, y, xz.y + 0.5f);
		this.Slope = slopeDirection;
		this.neighbors = new List<MapNode>(4);
	}

	public float Height { get { return this.Location.y; } }
	public MapNodeSlopeDirection Slope { get; private set; }
	public Vector3 Location { get; private set; }

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

    // Start is called before the first frame update
    void Start()
    {
		this.nodes = new Dictionary<Vector2Int, MapNode>();

		//First iteration creats map nodes
        for (int y = -this.mapSize; y < this.mapSize; y++)
		{
			for (int x = -this.mapSize; x < this.mapSize; x++)
			{
				Ray r = new Ray(new Vector3(x + 0.5f, 5.0f, y + 0.5f), Vector3.down);

				if (Physics.Raycast(r, out RaycastHit hit, 10.0f, this.mapLayerMask))
				{
					float height = hit.point.y;

					//Dot product shows how close the normal is to the up vector,
					//if the dot product is equal to 1, the normal is facing upward.
					//We use a value of 0.9 to allow some leeway.
					float dot = Vector3.Dot(hit.normal, Vector3.up);

					MapNodeSlopeDirection slopeDirection;

					//If the slope is flat
					if (dot > 0.9f)
					{
						slopeDirection = MapNodeSlopeDirection.None;
						//Debug.Log("Not sloping: " + x + " " + y);
					}
					else
					{
						//If the tile is sloping in the x direction
						if (Mathf.Abs(hit.normal.x) > 0.01)
						{
							slopeDirection = MapNodeSlopeDirection.X;
							//Debug.Log("Sloping in X direction: " + x + " " + y);
						}
						else
						{
							slopeDirection = MapNodeSlopeDirection.Y;
							//Debug.Log("Sloping in Y direction: " + x + " " + y);
						}						
					}

					Vector2Int v = new Vector2Int(x, y);
					this.nodes.Add(v, new MapNode(v, height, slopeDirection));
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
    }

    // Update is called once per frame
    void Update()
    {
        
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

	public void Pathfind(MapNode start, MapNode goal, ref List<Vector3> pathOut)
	{
		if (start == null || goal == null)
		{
			Debug.LogError("Failed to pathfind from " + start + " to " + goal);
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
				pathOut.Add(currentNode.Location);

				while (cameFrom.ContainsKey(currentNode))
				{
					currentNode = cameFrom[currentNode];
					pathOut.Add(currentNode.Location);
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
				//Debug.Log("Examining neighbor: " + neighbor.Location);

				if (!gScore.TryGetValue(neighbor, out float neighborScore)) neighborScore = Mathf.Infinity;
				//Debug.Log("Neighbor score: " + neighborScore);

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

		Debug.LogError("Failed to pathfind from " + start.Location + " to " + goal.Location);
	}
}
