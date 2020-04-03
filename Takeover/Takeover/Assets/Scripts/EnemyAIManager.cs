using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyAIManager : MonoBehaviour
{
	//The number of actions the AI can make per turn
	[SerializeField] private int difficulty;
	[SerializeField] private Team team;

	private MapController map;

	private void OnEnable()
	{
		TurnManager.AddTurnEvent(this.OnTurn, TurnStage.EnemyAI);
	}

	private void OnDisable()
	{
		TurnManager.RemoveTurnEvent(this.OnTurn, TurnStage.EnemyAI);
	}

	// Start is called before the first frame update
	void Start()
    {
		this.map = GameObject.FindObjectOfType<MapController>();
    }

	public void OnTurn()
	{
		UnitController[] units = GameObject.FindObjectsOfType<UnitController>();
		FactoryController[] factories = GameObject.FindObjectsOfType<FactoryController>();
		ResourceProducerController[] producers = GameObject.FindObjectsOfType<ResourceProducerController>();

		//Contain units this AI does NOT control
		List<UnitController> hostiles = new List<UnitController>();

		//Contains units this AI DOES control
		List<UnitController> friendlies = new List<UnitController>();

		//Contains factories the AI controls
		List<FactoryController> property = new List<FactoryController>();

		//Contains resource quarries the AI controls
		List<ResourceProducerController> resources = new List<ResourceProducerController>();

		CapturableController[] capturables = GameObject.FindObjectsOfType<CapturableController>();

		foreach (UnitController unit in units)
		{
			if (unit.GetTeam() == this.team)
			{
				friendlies.Add(unit);
			}
			else if (unit.GetTeam() != Team.Neutral)
			{
				hostiles.Add(unit);
			}
		}

		foreach (FactoryController factory in factories)
		{
			if (factory.GetTeam() == this.team)
			{
				property.Add(factory);
			}
		}

		foreach (ResourceProducerController producer in producers)
		{
			if (producer.GetTeam() == this.team)
			{
				resources.Add(producer);
			}
		}

		int numCommandsIssued = 0;

		//Issue unit commands
		foreach (UnitController u in friendlies)
		{
			if (numCommandsIssued > this.difficulty)
			{
				return;
			}

			bool hasCommand = false;

			MoveableComponent moveable = u.GetComponent<MoveableComponent>();

			//Check for adjacent hostile units and attack them
			foreach (UnitController h in hostiles)
			{
				if (Vector3.Distance(h.transform.position, u.transform.position) < 1.1f)
				{
					//Stop moving
					if (moveable != null)
					{
						moveable.IssueMoveCommand(u.transform.position);
					}

					u.IssueAttackCommand(h);
					hasCommand = true;
					numCommandsIssued++;
					break;
				}
			}

			if (!hasCommand)
			{
				CapturableController target = null;
				Vector2Int targetTerritory = new Vector2Int();
				float distance = float.PositiveInfinity;

				//Capture new factories/quarries
				foreach (CapturableController cap in capturables)
				{
					if (cap.GetTeam() != this.team) //Look for hostile/neutral capturables
					{
						float newDistance = Vector3.Distance(u.transform.position, cap.transform.position);

						if (newDistance < distance) //Select the closest one
						{

							Vector2Int vec = MapController.LocationToMapIndex(u.transform.position);

							if (cap.IsTerritory(vec))
							{
								target = cap;
								distance = newDistance;
								targetTerritory = vec;
							}
							else if (cap.GetFreeTerritory(ref vec))
							{
								target = cap;
								distance = newDistance;
								targetTerritory = vec;
							}
						}
					}
				}

				if (target != null && moveable != null)
				{
					moveable.IssueMoveCommand(this.map.GetMapNode(targetTerritory).Location);
					numCommandsIssued++;
				}
			}
			//If any exist, attack them
			//If none exist:

			//Check if occupying neutral/hostile territory
			//If true, stay
			//If false, move to nearest
		}

		foreach (ResourceProducerController c in resources)
		{
			//Upgrade if it has the resources
			//(Check is done inside function, so we don't need to do it here)
			c.Upgrade();
		}

		foreach (FactoryController c in property)
		{
			if (numCommandsIssued > this.difficulty)
			{
				return;
			}

			if (!c.IsProducing())
			{
				//Upgrade if it has the resources
				//(Check is done inside function, so we don't need to do it here)
				c.Upgrade();

				List<Recipe> validRecipes = new List<Recipe>();

				for (int i = 0; i < 6; i++)
				{
					Recipe r = FactoryManager.GetRecipe(i);

					if (r.levelRequired <= c.GetLevel())
					{
						bool hasResources = true;

						foreach (RecipeCost cost in r.costs)
						{
							if (ResourceManager.GetResourceInfo(cost.resource, Team.Team2).amount < cost.amount)
							{
								hasResources = false;
								break;
							}
						}

						//Add a bit of randomness to AI choices
						if (hasResources && Random.value > 0.5f)
						{
							validRecipes.Add(r);
						}
					}
				}

				Recipe highest = Recipe.none;

				foreach (Recipe r in validRecipes)
				{
					if (r.levelRequired > highest.levelRequired)
					{
						highest = r;
					}
				}

				c.ChangeProduction(highest);
				numCommandsIssued++;
			}
		}
	}
}
