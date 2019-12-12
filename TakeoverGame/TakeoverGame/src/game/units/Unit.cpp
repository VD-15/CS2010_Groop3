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

UnitEntity::UnitEntity(const Vector3& position, const ProtoUnit* prototype, Team t) :
	transform(CreateComponent<TransformComponent3D>()),
	draw(CreateComponent<DrawModelComponent3D>(transform, ContentManager<Model>::Get().GetContent(prototype->modelName))),
	team(CreateComponent<TeamComponent>(t)),
	select(CreateComponent<SelectableComponent>(transform)),
	capture(CreateComponent<CaptureContributorComponent>(transform, team)),
	reciever(CreateComponent<CommandRecieverComponent>(transform, select)),
	target(CreateComponent<CombatTargetComponent>(transform, team, prototype->health)),
	logic(CreateComponent<UnitComponent>(transform, team, prototype))
{
	this->transform->location = position;
	this->transform->scale = Vector3(2.0f);
	this->select->hoverRadius = 8.0f;
	this->reciever->speed = prototype->moveSpeed;
}

void UnitEntity::OnDelete()
{
	this->transform->Delete();
	this->draw->Delete();
	this->team->Delete();
	this->select->Delete();
	this->capture->Delete();
	this->reciever->Delete();
	this->target->Delete();
	this->logic->Delete();
}