#include "UnitEntity.h"

#include "../../engine/core/ContentManager.hpp"
#include "../../engine/components/LightComponent.h"

#include "../../engine/core/VLKTime.h"
using namespace tkv;

void OnUpdate(UpdateEvent& ev)
{
	static Float theta;

	UnitComponent::ForEach([](UnitComponent* c)
	{
		c->transform->rotation *= Quaternion::AxisAngle(Vector3::UP, VLKTime::DeltaTime<Float>());
	});

	theta += VLKTime::DeltaTime<Float>();
}
void UnitSystem::Init()
{
	EventBus<UpdateEvent>::Get().AddEventListener(OnUpdate);
}

void UnitSystem::Destroy()
{
	EventBus<UpdateEvent>::Get().RemoveEventListener(OnUpdate);
}

ProtoUnit::ProtoUnit(double health, double damage, double range, double atkspd,
		double speed, double aggro, bool isCommander)
{
	this->isCommander = isCommander;
}

UnitEntity::UnitEntity(Team t, Vector2 location, ProtoUnit p)
{
	this->transform = CreateComponent<TransformComponent3D>();
	this->draw = CreateComponent<DrawModelComponent3D>(transform, ContentManager<Texture2D>::Get().GetContent("unit"));
	this->team = CreateComponent<TeamComponent>();
	this->capture = CreateComponent<CaptureContributorComponent>(transform, team);
	this->select = CreateComponent<SelectableComponent>(transform, team);
	this->reciever = CreateComponent<CommandRecieverComponent>(transform, select);
	this->logic = CreateComponent<UnitComponent>(p, t);

	this->transform->location = location;
	this->team->team = t;

	this->draw->depth = tkv::DEPTH_UNIT;
	this->draw->size = 32.0f;

	this->reciever->speed = 64.0f;
	this->select->hoverRadius = 32.0f;
	this->select->priority = 1;
}
void UnitEntity::OnDelete()
{
	reciever->Delete();
	select->Delete();
	capture->Delete();
	team->Delete();
	draw->Delete();
	transform->Delete();
	logic->Delete();
}
