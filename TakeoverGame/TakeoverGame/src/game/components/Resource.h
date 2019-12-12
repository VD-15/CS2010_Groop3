#pragma once

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"

#include "TeamComponent.h"

using namespace vlk;

namespace tkv
{
	enum class Resource
	{
		None,

		Steel,
		Aluminium,
		Rubber,

		Oil,
		Silicone,

		Uranium
	};

	struct ResourceInventoryInfo
	{
		ResourceInventoryInfo();

		Double flow;
		Double amount;
		Double capacity;

		ULong numProducers;
		ULong numConsumers;
		ULong storageCount;
	};

	namespace ResourceSystem
	{
		void Init();
		void Destroy();

		//Gets the net resource flow per second
		Double GetResourcesPerSecond(Team t, Resource r);

		//Gets the total combined capacity of all resource stores
		Double GetResourceCapacity(Team t, Resource r);

		//Gets the number of stored resources
		Double GetResourceAmount(Team t, Resource r);

		//Gets the number of storage units
		ULong GetNumResourceUnits(Team t, Resource r);

		void GetResourceInfo(Team t, Resource r, ResourceInventoryInfo& info);
	}

	//Utility struct for ResourceStoreComponent
	struct ResourceContribution
	{
		//The resource being produced / consumed
		Resource resource;

		//The number of resources per second being produced / consumed
		//Should be positive for producers & negative for consumers
		Double resourcesPerSecond;

		//For producers: true if there was adequate storage to put resources last EarlyUpdate
		//For consumers: true if there were resources available to consume in the last EarlyUpdate
		Bool hasInventory;
	};

	//Stores a finite amount of a single resource
	struct ResourceStoreComponent : public Component<ResourceStoreComponent>
	{
		ResourceStoreComponent(IEntity* e, const TeamComponent* t, Resource r, Double capacity);

		//Resource being stored in this unit
		Resource resource;

		//Amount of the resource being stored in this unit
		Double amount;

		//Capacity of this storage unit
		Double capacity;

		const TeamComponent* team;
	};

	//A component that produces or consumes a constant amount of resources per second
	struct ResourceContributorComponent : public Component<ResourceContributorComponent>
	{
		ResourceContributorComponent(IEntity* e, Team t);

		std::array<ResourceContribution, 3> production;
		
		const TeamComponent* const team;
	};
}