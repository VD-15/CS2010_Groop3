using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//Represents an object that can occupy a space on the map and block movement
public class TileOccupierController : MonoBehaviour
{
	public Vector2Int Location { get; private set; }
	private MapController map;

	private void Start()
	{
		this.Location = new Vector2Int(Mathf.FloorToInt(this.transform.position.x), Mathf.FloorToInt(this.transform.position.z));

		this.map = FindObjectOfType<MapController>();

		if (this.map.isComplete)
		{
			this.AddToMap();
		}
		else
		{
			this.map.OnMapComplete += this.AddToMap;
		}

		//Debug.Log("Tile occupier at: " + this.Location);
	}

	private void OnDestroy()
	{
		this.map.SetOccupier(this.Location, null);
	}

	private void AddToMap()
	{
		if (this.map.GetOccupier(this.Location) == null)
		{
			this.map.SetOccupier(this.Location, this);
		}
		else
		{
			Debug.LogError("Map tile already occupied: " + this.Location);
			GameObject.Destroy(this.transform.root.gameObject);
		}
	}

	//Checks if this occupier can move to this spot.
	//Does not validate pathfinding, only occupation and proximity.
	public bool CanMoveTo(Vector2Int newLoc)
	{
		if (newLoc == this.Location) return true;

		//Map tile is not occupied
		if (this.map.GetOccupier(newLoc) == null)
		{
			//Map tile is orthogonal to current position
			if (newLoc.x == this.Location.x)
			{
				//Can only move onto tile directly adjacent
				if (Mathf.Abs(newLoc.y - this.Location.y) == 1)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if (newLoc.y == this.Location.y)
			{
				if (Mathf.Abs(newLoc.x - this.Location.x) == 1)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	//Moves this occupier to a new location
	//Moves should be validated beforehand with TileOccupierController.CanMoveTo(Vector2Int) as this will not check for you.
	public void MoveTo(Vector2Int newLoc)
	{
		this.map.SetOccupier(this.Location, null);
		this.map.SetOccupier(newLoc, this);
		this.Location = newLoc;
	}
}
