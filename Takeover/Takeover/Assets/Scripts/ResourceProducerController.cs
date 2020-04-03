using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ResourceProducerController : MonoBehaviour
{
	[SerializeField] private Resource[] resources;
	[SerializeField] private int[] amounts;

	private int level;

	private TeamController team;

    // Start is called before the first frame update
    void Start()
    {
		this.team = this.GetComponent<TeamController>();
		this.level = 1;

		if (this.resources.Length != 3) Debug.LogError("Invalid number of resources."); 
		if (this.amounts.Length != 3) Debug.LogError("Invalid number of resources."); 
    }

	private void OnEnable()
	{
		TurnManager.AddTurnEvent(this.OnTurn, TurnStage.Resource);
	}

	private void OnDisable()
	{
		TurnManager.RemoveTurnEvent(this.OnTurn, TurnStage.Resource);
	}

	private void OnTurn()
	{
		Debug.Log("Resource actor update");

		if (this.team.team != Team.Neutral)
		{
			for (int i = 0; i < this.level; i++)
			{
				ResourceManager.AddResources(this.resources[i], this.team.team, this.amounts[i]);
			}
		}
	}

	public Resource GetResource(int index)
	{
		return this.resources[index];
	}

	public int GetAmountPerTurn(int index)
	{
		return this.amounts[index];
	}

	public int GetLevel()
	{
		return this.level;
	}

	public void Upgrade()
	{
		if (this.team.team == Team.Neutral) return;

		switch (this.level)
		{
			case 1:
				if (ResourceManager.GetResourceInfo(Resource.Steel, this.team.team).amount >= 30)
				{
					ResourceManager.TakeResources(Resource.Steel, this.team.team, 30);
					this.level++;
				}
				break;
			case 2:
				if (ResourceManager.GetResourceInfo(Resource.Oil, this.team.team).amount >= 40)
				{
					ResourceManager.TakeResources(Resource.Oil, this.team.team, 40);
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
}
