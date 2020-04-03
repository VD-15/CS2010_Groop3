using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum Resource
{
	Steel,
	Rubber,
	Aluminium,
	Oil,
	Silicone,
	Uranium,
	Power
}

struct ResourceKey
{
	public Resource resource;
	public Team team;
}

public struct ResourceEntry
{
	public long amount;
	public long capacity;
}

public class ResourceManager : MonoBehaviour
{
	private static Dictionary<ResourceKey, ResourceEntry> resources;

	private void Start()
	{
		ResourceManager.resources = new Dictionary<ResourceKey, ResourceEntry>();

		AddResource(Resource.Steel, Team.Team1, 30, 100);
		AddResource(Resource.Steel, Team.Team2, 30, 100);

		AddResource(Resource.Rubber, Team.Team1, 20, 50);
		AddResource(Resource.Rubber, Team.Team2, 20, 50);

		AddResource(Resource.Aluminium, Team.Team1, 10, 50);
		AddResource(Resource.Aluminium, Team.Team2, 10, 50);

		AddResource(Resource.Oil, Team.Team1, 0, 50);
		AddResource(Resource.Oil, Team.Team2, 0, 50);

		AddResource(Resource.Silicone, Team.Team1, 0, 25);
		AddResource(Resource.Silicone, Team.Team2, 0, 25);

		AddResource(Resource.Uranium, Team.Team1, 0, 5);
		AddResource(Resource.Uranium, Team.Team2, 0, 5);

		AddResource(Resource.Power, Team.Team1, 100, 100);
		AddResource(Resource.Power, Team.Team2, 100, 100);
	}

	private static void AddResource(Resource r, Team t, long amount, long capacity)
	{
		ResourceManager.resources.Add(new ResourceKey { resource = r, team = t }, new ResourceEntry { amount = amount, capacity = capacity });
	}

	public static ResourceEntry GetResourceInfo(Resource r, Team t)
	{
		return ResourceManager.resources[new ResourceKey { resource = r, team = t }];
	}

	public static void TakeResources(Resource r, Team t, long amount)
	{
		ResourceKey k = new ResourceKey { resource = r, team = t };

		ResourceEntry e = ResourceManager.resources[k];

		if (e.amount < amount)
		{
			e.amount = 0;

			Debug.LogError("Resource debt created");
		}
		else
		{
			e.amount -= amount;
		}

		ResourceManager.resources[k] = e;
	}

	public static void AddResources(Resource r, Team t, long amount)
	{
		ResourceKey k = new ResourceKey { resource = r, team = t };

		ResourceEntry e = ResourceManager.resources[k];

		e.amount += amount;

		if (e.amount > e.capacity)
		{
			e.amount = e.capacity;
		}

		ResourceManager.resources[k] = e;
	}
}
