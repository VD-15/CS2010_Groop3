using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public struct RecipeCost
{
	public Resource resource;
	public int amount;
}

public struct Recipe
{
	public string name;

	public GameObject prefab;

	public RecipeCost[] costs;

	public int turnLength;
	public int levelRequired;

	public static Recipe none = new Recipe
	{
		name = "Nothing",
		prefab = null,
		costs = new RecipeCost[] { }
	};
}

public class FactoryManager : MonoBehaviour
{
	[SerializeField] private GameObject dronePrefab;
	[SerializeField] private GameObject gyroPrefab;
	[SerializeField] private GameObject mortarPrefab;
	[SerializeField] private GameObject roamerPrefab;
	[SerializeField] private GameObject tankPrefab;
	[SerializeField] private GameObject UFOPrefab;

	private static List<Recipe> recipes;

    // Start is called before the first frame update
    void Start()
    {
        if (FactoryManager.recipes == null)
		{
			this.InitRecipes();
		}
    }

	public static Recipe GetRecipe(int index)
	{
		return FactoryManager.recipes[index];
	}

	private void InitRecipes()
	{
		recipes = new List<Recipe>();

		recipes.AddRange(new Recipe[]
		{
			new Recipe
			{
				name = "Drone",
				prefab = this.dronePrefab,
				turnLength = 3,
				levelRequired = 1,
				costs = new RecipeCost[]
				{
					new RecipeCost { resource = Resource.Aluminium, amount = 3 },
					new RecipeCost { resource = Resource.Steel, amount = 2 }
				}
			},

			new Recipe
			{
				name = "Gyro",
				prefab = this.gyroPrefab,
				turnLength = 5,
				levelRequired = 2,
				costs = new RecipeCost[]
				{
					new RecipeCost { resource = Resource.Aluminium, amount = 10 },
					new RecipeCost { resource = Resource.Oil, amount = 6 },
					new RecipeCost { resource = Resource.Silicone, amount = 3 }
				}
			},

			new Recipe
			{
				name = "Mothership",
				prefab = this.UFOPrefab,
				turnLength = 10,
				levelRequired = 3,
				costs = new RecipeCost[]
				{
					new RecipeCost { resource = Resource.Aluminium, amount = 20},
					new RecipeCost { resource = Resource.Silicone, amount = 15},
					new RecipeCost { resource = Resource.Uranium, amount = 3}
				}
			},

			new Recipe
			{
				name = "Scout",
				prefab = this.roamerPrefab,
				turnLength = 3,
				levelRequired = 1,
				costs = new RecipeCost[]
				{
					new RecipeCost { resource = Resource.Steel, amount = 2},
					new RecipeCost { resource = Resource.Rubber, amount = 3},
				}
			},

			new Recipe
			{
				name = "Mortar",
				prefab = this.mortarPrefab,
				turnLength = 5,
				levelRequired = 2,
				costs = new RecipeCost[]
				{
					new RecipeCost { resource = Resource.Rubber, amount = 6},
					new RecipeCost { resource = Resource.Oil, amount = 10},
					new RecipeCost { resource = Resource.Steel, amount = 6}
				}
			},

			new Recipe
			{
				name = "Tank",
				prefab = this.tankPrefab,
				turnLength = 10,
				levelRequired = 3,
				costs = new RecipeCost[]
				{
					new RecipeCost { resource = Resource.Steel, amount = 25},
					new RecipeCost {resource = Resource.Oil, amount = 15},
					new RecipeCost {resource = Resource.Uranium, amount = 5}
				}
			}
		});
	}
}
