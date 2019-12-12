#include "Resource.h"
#include "../../engine/core/VLKTime.h"

using namespace tkv;

namespace
{
	void OnEarlyUpdate(EarlyUpdateEvent& ev)
	{
		constexpr Team teams[] = { Team::Team1, Team::Team2 };
		constexpr Resource resources[] = { Resource::Steel, Resource::Aluminium, Resource::Rubber };

		const Double deltaTime = VLKTime::DeltaTime();

		for (Team t : teams)
		{
			for (Resource r : resources)
			{
				ResourceInventoryInfo info;
				ResourceSystem::GetResourceInfo(t, r, info);

				if (info.flow < 0)
				{
					Double flowAmount = std::abs(info.flow * deltaTime);

					if (flowAmount > info.amount)
					{
						//Set storage to zero
						ResourceStoreComponent::ForEach([t, r](ResourceStoreComponent* c)
						{
							if (c->team->team == t && c->resource == r)
							{
								c->amount = 0.0;
							}
						});

						//Raise insufficient resource flags to true
						ResourceContributorComponent::ForEach([t, r](ResourceContributorComponent* c)
						{
							if (c->team->team == t)
							{
								for (auto it = c->production.begin(); it != c->production.end(); it++)
								{
									if (it->resource == r)
									{
										it->hasInventory = false;
									}
								}
							}
						});
					}
					else
					{
						Double newTotal = info.amount - flowAmount;
						Double perUnit = newTotal / info.storageCount;

						//Decrease storage
						ResourceStoreComponent::ForEach([perUnit, t, r](ResourceStoreComponent* c)
						{
							if (c->team->team == t && c->resource == r)
							{
								//balance resources
								c->amount = perUnit;
							}
						});

						//lower insufficient resources flag
						ResourceContributorComponent::ForEach([t, r](ResourceContributorComponent* c)
						{
							if (c->team->team == t)
							{
								for (auto it = c->production.begin(); it != c->production.end(); it++)
								{
									if (it->resource == r)
									{
										it->hasInventory = true;
									}
								}
							}
						});
					}
				}
				else if (info.flow > 0)
				{
					Double flowAmount = info.flow * deltaTime;
					/*
					if (info.flow * deltaTime > info.capacity)
					{
						//Fill storage to max
						ResourceStoreComponent::ForEach([perUnit, t, r](ResourceStoreComponent* c)
						{
							if (c->team->team == t && c->resource == r)
							{
								//balance resources
								c->amount = perUnit;
							}
						});
					}
					else
					{
						//Increase storage
					}*/
				}
				else
				{
					//lower insufficient resources flag
					ResourceContributorComponent::ForEach([t, r](ResourceContributorComponent* c)
					{
						if (c->team->team == t)
						{
							for (auto it = c->production.begin(); it != c->production.end(); it++)
							{
								if (it->resource == r)
								{
									it->hasInventory = true;
								}
							}
						}
					});
				}
			}
		}
	}
}

ResourceInventoryInfo::ResourceInventoryInfo()
{
	this->flow = 0.0;
	this->amount = 0.0;
	this->capacity = 0.0;

	this->numProducers = 0;
	this->numConsumers = 0;
	this->storageCount = 0;
}

void ResourceSystem::Init()
{
	//EventBus<EarlyUpdateEvent>::Get().AddEventListener(OnEarlyUpdate);
}

void ResourceSystem::Destroy()
{
	//EventBus<EarlyUpdateEvent>::Get().RemoveEventListener(OnEarlyUpdate);
}

Double ResourceSystem::GetResourcesPerSecond(Team t, Resource r)
{
	Double rps = 0.0;

	ResourceContributorComponent::ForEach([&rps, t, r](ResourceContributorComponent* c)
	{
		if (c->team->team == t)
		{
			for (auto it = c->production.begin(); it != c->production.end(); it++)
			{
				if (it->resource == r)
				{
					rps += it->resourcesPerSecond;
				}
			}
		}
	});

	return rps;
}

Double ResourceSystem::GetResourceCapacity(Team t, Resource r)
{
	Double capacity = 0.0;

	ResourceStoreComponent::ForEach([&capacity, t, r](ResourceStoreComponent* c)
	{
		if (c->team->team == t && c->resource == r)
		{
			capacity += c->capacity;
		}
	});

	return capacity;
}

Double ResourceSystem::GetResourceAmount(Team t, Resource r)
{
	Double amount = 0.0;

	ResourceStoreComponent::ForEach([&amount, t, r](ResourceStoreComponent* c)
	{
		if (c->team->team == t && c->resource == r)
		{
			amount += c->amount;
		}
	});

	return amount;
}

ULong ResourceSystem::GetNumResourceUnits(Team t, Resource r)
{
	ULong units;

	ResourceStoreComponent::ForEach([&units, t, r](ResourceStoreComponent* c)
	{
		if (c->team->team == t && c->resource == r)
		{
			units++;
		}
	});

	return units;
}

void ResourceSystem::GetResourceInfo(Team t, Resource r, ResourceInventoryInfo& info)
{
	info = ResourceInventoryInfo();

	ResourceContributorComponent::ForEach([t, r, &info](ResourceContributorComponent* c)
	{
		if (c->team->team == t)
		{
			for (auto it = c->production.begin(); it != c->production.end(); it++)
			{
				if (it->resource == r)
				{
					info.flow += it->resourcesPerSecond;

					if (it->resourcesPerSecond < 0.0)
					{
						info.numConsumers++;
					}
					else if (it->resourcesPerSecond > 0.0)
					{
						info.numProducers++;
					}
				}
			}
		}
	});

	ResourceStoreComponent::ForEach([t, r, &info](ResourceStoreComponent* c)
	{
		if (c->team->team == t && c->resource == r)
		{
			info.storageCount++;

			info.amount += c->amount;
			info.capacity += c->capacity;
		}
	});
}