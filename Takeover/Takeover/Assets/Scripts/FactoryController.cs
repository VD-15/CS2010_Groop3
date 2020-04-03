using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FactoryController : MonoBehaviour
{
	private TeamController team;
	private CapturableController capturable;
	private int level;
	private int turnProgress;
	private Recipe recipe;

    // Start is called before the first frame update
    void Start()
    {
		this.team = this.GetComponent<TeamController>();
		this.capturable = this.GetComponent<CapturableController>();
		this.level = 1;
		this.turnProgress = 0;
		this.recipe = Recipe.none;
    }

	private void OnEnable()
	{
		TurnManager.AddTurnEvent(this.OnTurn, TurnStage.Production);
	}

	private void OnDisable()
	{
		TurnManager.RemoveTurnEvent(this.OnTurn, TurnStage.Production);
	}

	public int GetLevel()
	{
		return this.level;
	}

	public string GetProductionInfo()
	{
		if (this.team.team != Team.Neutral)
		{
			string s = "Producing: ";
			s += this.recipe.name + "\n";
			s += (this.recipe.turnLength - this.turnProgress) + " Turns Left\n\n";

			if (!this.SpaceCheck())
			{
				s += "No room for unit!";
			}
			else if (!this.PowerCheck())
			{
				s += "Insufficient power!";
			}

			return s;
		}
		else
		{
			return "Producing: Nothing";
		}
	}

	public void ChangeProduction(Recipe r)
	{
		//Check available resources
		foreach (RecipeCost c in r.costs)
		{
			if (ResourceManager.GetResourceInfo(c.resource, this.team.team).amount < c.amount)
			{
				//Insufficient resources
				return;
			}
		}

		//Refund resources
		foreach (RecipeCost c in this.recipe.costs)
		{
			ResourceManager.AddResources(c.resource, this.team.team, c.amount);
		}

		//Take new resources
		foreach (RecipeCost c in r.costs)
		{
			ResourceManager.TakeResources(c.resource, this.team.team, c.amount);
		}

		this.recipe = r;
		this.turnProgress = 0;
	}

	private void OnTurn()
	{
		if (!this.IsProducing())
		{
			return;
		}

		this.turnProgress++;

		if (this.turnProgress >= this.recipe.turnLength)
		{
			Vector2Int location = new Vector2Int();

			if (this.capturable.GetFreeTerritory(ref location))
			{
				if (this.PowerCheck())
				{
					this.turnProgress = 0;

					MapController map = FindObjectOfType<MapController>();

					GameObject g = GameObject.Instantiate(this.recipe.prefab, map.GetMapNode(location).Location, Quaternion.identity);
					g.GetComponent<TeamController>().team = this.team.team;

					ResourceManager.TakeResources(Resource.Power, this.team.team, 1);

					this.recipe = Recipe.none;
				}
			}
		}
	}

	private bool PowerCheck()
	{
		return ResourceManager.GetResourceInfo(Resource.Power, this.team.team).amount > 1;
	}

	private bool SpaceCheck()
	{
		Vector2Int v = new Vector2Int();

		return this.capturable.GetFreeTerritory(ref v);
	}

	public void Upgrade()
	{
		if (this.team.team == Team.Neutral) return;

		switch (this.level)
		{
			case 1:
				if (ResourceManager.GetResourceInfo(Resource.Oil, this.team.team).amount >= 20)
				{
					ResourceManager.TakeResources(Resource.Oil, this.team.team, 20);
					this.level++;
				}
				break;
			case 2:
				if (ResourceManager.GetResourceInfo(Resource.Uranium, this.team.team).amount >= 10)
				{
					ResourceManager.TakeResources(Resource.Uranium, this.team.team, 10); 
					this.level++;
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

	public bool IsProducing()
	{
		return this.recipe.name != Recipe.none.name;
	}
}
