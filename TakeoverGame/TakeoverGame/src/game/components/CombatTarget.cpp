#include "CombatTarget.h"

#include <vector>

using namespace tkv;

namespace
{
	//React to any damage being dealt
	void OnLateUpdate(LateUpdateEvent& ev)
	{
		std::vector<CombatTargetComponent*> deadTargets;

		CombatTargetComponent::ForEach([&deadTargets](CombatTargetComponent* c)
		{
			if (c->currentHealth < 0)
			{
				deadTargets.push_back(c);
			}
		});

		for (auto it = deadTargets.begin(); it != deadTargets.end(); it++)
		{
			CombatTargetComponent* c = *it;
			c->GetParent()->Delete();
		}
	}
}

void CombatSystem::Init()
{
	EventBus<LateUpdateEvent>::Get().AddEventListener(OnLateUpdate);
}

void CombatSystem::Destroy()
{
	EventBus<LateUpdateEvent>::Get().RemoveEventListener(OnLateUpdate);
}

CombatTargetComponent::CombatTargetComponent(IEntity* e, const TransformComponent3D* transform, const TeamComponent* team, Double maxHealth) :
	Component<CombatTargetComponent>(e),
	transform(transform),
	team(team),
	maxHealth(maxHealth)
{
	this->currentHealth = this->maxHealth;
}

void CombatTargetComponent::DealDamage(Double amount)
{
	this->currentHealth -= amount;

	if (this->currentHealth > this->maxHealth)
	{
		this->currentHealth = this->maxHealth;
	}
}