using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum CaptureState
{
	Idle, //Nothing is happening
	Capturing, //Capturte progress in going up
	Contested, //Capture progress is halted
	Resolving //capture progress is going down
}

public class CapturableController : MonoBehaviour
{
    //Number of turn required for one unit to capture this
    [SerializeField] private int captureLength;

	//If true the building will occupy a 2x2 space as oppose to to the usual 1x1
	//Large buildings assume they're on the intersection of the 4 tiles they occupy.
	[SerializeField] private bool isLargeBuilding;

	//An empty prefab object with a TileOccupierComponent attached
	//Unused if this building is 'small'
	[SerializeField] private GameObject occupierPrefab;

	public delegate void CaptureFun();

	public event CaptureFun OnCapture;

	//Progress of this capture
	private int captureProgress;

	private CaptureState state;

	//The team this object belongs to
	private TeamController team;

	//The areas this building can be captured from.
	private List<Vector2Int> territory;

	private MapController map;

    // Start is called before the first frame update
    void Start()
    {
		this.territory = new List<Vector2Int>();

		this.map = FindObjectOfType<MapController>();

		this.team = this.GetComponent<TeamController>();

		if (this.map.isComplete)
		{
			this.OnMapComplete();
		}
		else
		{
			this.map.OnMapComplete += this.OnMapComplete;
		}
    }

	//Occupy tiles and establish territory
	void OnMapComplete()
	{
		if (this.isLargeBuilding)
		{
			GameObject.Instantiate(this.occupierPrefab, this.transform.position + new Vector3(-0.5f, 0.0f, -0.5f), Quaternion.identity, this.transform);
			GameObject.Instantiate(this.occupierPrefab, this.transform.position + new Vector3( 0.5f, 0.0f, -0.5f), Quaternion.identity, this.transform);
			GameObject.Instantiate(this.occupierPrefab, this.transform.position + new Vector3( 0.5f, 0.0f,  0.5f), Quaternion.identity, this.transform);
			GameObject.Instantiate(this.occupierPrefab, this.transform.position + new Vector3(-0.5f, 0.0f,  0.5f), Quaternion.identity, this.transform);

			//Assumes all 12 adjacent tiles are empty and connected
			this.territory.Add(new Vector2Int(Mathf.FloorToInt(this.transform.position.x - 1.5f), Mathf.FloorToInt(this.transform.position.z - 1.5f)));
			this.territory.Add(new Vector2Int(Mathf.FloorToInt(this.transform.position.x - 0.5f), Mathf.FloorToInt(this.transform.position.z - 1.5f)));
			this.territory.Add(new Vector2Int(Mathf.FloorToInt(this.transform.position.x + 0.5f), Mathf.FloorToInt(this.transform.position.z - 1.5f)));
			this.territory.Add(new Vector2Int(Mathf.FloorToInt(this.transform.position.x + 1.5f), Mathf.FloorToInt(this.transform.position.z - 1.5f)));

			this.territory.Add(new Vector2Int(Mathf.FloorToInt(this.transform.position.x - 1.5f), Mathf.FloorToInt(this.transform.position.z + 0.5f)));
			this.territory.Add(new Vector2Int(Mathf.FloorToInt(this.transform.position.x + 1.5f), Mathf.FloorToInt(this.transform.position.z + 0.5f)));

			this.territory.Add(new Vector2Int(Mathf.FloorToInt(this.transform.position.x - 1.5f), Mathf.FloorToInt(this.transform.position.z - 0.5f)));
			this.territory.Add(new Vector2Int(Mathf.FloorToInt(this.transform.position.x + 1.5f), Mathf.FloorToInt(this.transform.position.z - 0.5f)));

			this.territory.Add(new Vector2Int(Mathf.FloorToInt(this.transform.position.x - 1.5f), Mathf.FloorToInt(this.transform.position.z + 1.5f)));
			this.territory.Add(new Vector2Int(Mathf.FloorToInt(this.transform.position.x - 0.5f), Mathf.FloorToInt(this.transform.position.z + 1.5f)));
			this.territory.Add(new Vector2Int(Mathf.FloorToInt(this.transform.position.x + 0.5f), Mathf.FloorToInt(this.transform.position.z + 1.5f)));
			this.territory.Add(new Vector2Int(Mathf.FloorToInt(this.transform.position.x + 1.5f), Mathf.FloorToInt(this.transform.position.z + 1.5f)));
		}
		else
		{
			this.gameObject.AddComponent<TileOccupierController>();

			Vector2Int v = MapController.LocationToMapIndex(this.transform.position);

			Vector2Int[] vectors =
			{
				new Vector2Int(Mathf.FloorToInt(this.transform.position.x) - 1, Mathf.FloorToInt(this.transform.position.z) - 1),
				new Vector2Int(Mathf.FloorToInt(this.transform.position.x) + 1, Mathf.FloorToInt(this.transform.position.z) - 1),
				new Vector2Int(Mathf.FloorToInt(this.transform.position.x) - 1, Mathf.FloorToInt(this.transform.position.z) + 1),
				new Vector2Int(Mathf.FloorToInt(this.transform.position.x) + 1, Mathf.FloorToInt(this.transform.position.z) + 1),

				new Vector2Int(Mathf.FloorToInt(this.transform.position.x), Mathf.FloorToInt(this.transform.position.z) + 1),
				new Vector2Int(Mathf.FloorToInt(this.transform.position.x), Mathf.FloorToInt(this.transform.position.z) - 1),
				new Vector2Int(Mathf.FloorToInt(this.transform.position.x) + 1, Mathf.FloorToInt(this.transform.position.z)),
				new Vector2Int(Mathf.FloorToInt(this.transform.position.x) - 1, Mathf.FloorToInt(this.transform.position.z))
			};

			foreach (Vector2Int vec in vectors)
			{
				if (this.map.IsAdjacent(v, vec))
				{
					this.territory.Add(vec);
				}
			}

			//Debug.Log("Capturable at " + v + ", Territory size: " + this.territory.Count);
		}
	}

	public void UpdateCapture(bool friendlyPresent, bool hostilePresent, Team capturingTeam)
	{
		switch (this.state)
		{
			case CaptureState.Idle:
				if (hostilePresent)
				{
					if (friendlyPresent)
					{
						this.state = CaptureState.Contested;
					}
					else
					{
						this.state = CaptureState.Capturing;
					}
				}
				break;
			case CaptureState.Capturing:
				if (!hostilePresent)
				{
					this.state = CaptureState.Resolving;
				}
				else if (friendlyPresent)
				{
					this.state = CaptureState.Contested;
				}
				break;
			case CaptureState.Contested:
				if (hostilePresent)
				{
					if (!friendlyPresent)
					{
						this.state = CaptureState.Capturing;
					}
				}
				else
				{
					this.state = CaptureState.Resolving;
				}
				break;
			case CaptureState.Resolving:
				if (hostilePresent)
				{
					if (friendlyPresent)
					{
						this.state = CaptureState.Contested;
					}
					else
					{
						this.state = CaptureState.Capturing;
					}
				}
				break;
			default:
				break;
		}

		switch (this.state)
		{
			case CaptureState.Capturing:
				this.captureProgress++;
				if (this.captureProgress >= this.captureLength)
				{
					this.OnCapture?.Invoke();
					this.captureProgress = 0;
					this.state = CaptureState.Idle;
					this.team.team = capturingTeam;
				}
				break;
			case CaptureState.Resolving:
				this.captureProgress--;
				if (this.captureProgress <= 0)
				{
					this.captureProgress = 0;
					this.state = CaptureState.Idle;
				}
				break;
			default:
				break;
		}
	}

	public Team GetTeam()
	{
		return this.team.team;
	}

	public CaptureState GetState()
	{
		return this.state;
	}

	public double GetCaptureProgress()
	{
		return (double)this.captureProgress / (double)this.captureLength;
	}

	public bool IsTerritory(Vector2Int index)
	{
		return this.territory.Contains(index);
	}

	public void GetTerritory(out List<Vector2Int> vecs)
	{
		vecs = new List<Vector2Int>(this.territory);
	}

	public bool GetFreeTerritory(ref Vector2Int vec)
	{
		foreach (Vector2Int v in this.territory)
		{
			TileOccupierController occ = map.GetOccupier(v);

			if (occ == null)
			{
				vec = v;
				return true;
			}
		}

		return false;
	}
}
