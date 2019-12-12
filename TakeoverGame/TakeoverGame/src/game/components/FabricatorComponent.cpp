#include "FabricatorComponent.h"
#include "../units/Unit.h"
#include "../../engine/core/VLKTime.h"
using namespace tkv;

namespace 
{
	void OnUpdate(UpdateEvent& ev)
	{
		UnitFabricatorComponent::ForEach([](UnitFabricatorComponent* c)
		{
			if((c->team->team != Team::Neutral))
			{
				c->cooldown -= VLKTime::DeltaTime();

				while (c->cooldown < 0.0)
				{
					UnitEntity::CreateEntity(c->transform->location + Vector3(-64.0f, 0.0f, 0.0f), c->production, c->team->team);
					c->cooldown += 5.0;
				}
			}			
		});
	}
}

void FabricatorSystem::Init()
{
	EventBus<UpdateEvent>::Get().AddEventListener(OnUpdate);
}

void FabricatorSystem::Destroy()
{
	EventBus<UpdateEvent>::Get().RemoveEventListener(OnUpdate);
}

UnitFabricatorComponent::UnitFabricatorComponent(IEntity* e, const TransformComponent3D* transform, const TeamComponent* t, const ProtoUnit* unit) :
	Component<UnitFabricatorComponent>(e),
	transform(transform),
	team(t)
{
	this->production = unit;
	this->cooldown = 5.0;
	this->spawnLocation = this->transform->location + Vector3(32.0f, 0.0f, 0.0f);
}

void UnitFabricatorComponent::ChangeUnit(const ProtoUnit* p)
{
	this->production = p;
	this->cooldown = 5.0;
}