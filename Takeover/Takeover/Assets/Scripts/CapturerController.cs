using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//Represents a unit that can capture a building
public class CapturerController : MonoBehaviour
{
	private TileOccupierController occupier;

	private TeamController team;

    // Start is called before the first frame update
    void Start()
    {
		this.team = this.GetComponent<TeamController>();
		this.occupier = this.GetComponent<TileOccupierController>();
    }

	public Team GetTeam()
	{
		return this.team.team;
	}

	public Vector2Int GetLocation()
	{
		return this.occupier.Location;
	}
}
