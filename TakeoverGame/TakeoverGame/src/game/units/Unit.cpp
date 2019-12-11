#include "Unit.h"
#include "../../engine/core/ContentManager.hpp"


using namespace tkv;

namespace
{
	void OnUpdate(UpdateEvent& ev)
	{

	}
}

void UnitSystem::Init()
{
	EventBus<UpdateEvent>::Get().AddEventListener(OnUpdate);
}

void UnitSystem::Destroy()
{
	EventBus<UpdateEvent>::Get().RemoveEventListener(OnUpdate);
}

UnitComponent::UnitComponent(IEntity* e, const TransformComponent3D* transform, const TeamComponent* team, const ProtoUnit* prototype) :
	Component<UnitComponent>(e),
	transform(transform),
	team(team),
	prototype(prototype)
{

}

UnitEntity::UnitEntity(const Vector3& position, const ProtoUnit* prototype, Team team) :
	transform(CreateComponent<TransformComponent3D>()),
	draw(CreateComponent<DrawModelComponent3D>(transform, ContentManager<Model>::Get().GetContent(prototype->modelName)))
{

}