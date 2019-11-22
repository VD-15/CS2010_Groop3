#include "TestUnitEntity.h"

#include "../../engine/core/ContentManager.hpp"
#include "../../engine/components/LightComponent.h"

#include "../../engine/core/VLKTime.h"

using namespace tkv;

void OnUpdate(UpdateEvent& ev)
{
	static Float theta;

	TestComponent::ForEach([](TestComponent* c)
	{
		//c->transform->rotation *= Quaternion::AxisAngle(Vector3::UP, VLKTime::DeltaTime<Float>());
	});

	theta += VLKTime::DeltaTime<Float>();
}

void TestSystem::Init()
{
	EventBus<UpdateEvent>::Get().AddEventListener(OnUpdate);
}

void TestSystem::Destroy()
{
	EventBus<UpdateEvent>::Get().RemoveEventListener(OnUpdate);
}

TestComponent::TestComponent(IEntity* e, TransformComponent3D* transform) :
	Component<TestComponent>(e),
	transform(transform)
{

}

TestUnitEntity::TestUnitEntity(Team t, Vector2 location)
{
	this->transform = CreateComponent<TransformComponent2D>();
	this->draw = CreateComponent<DrawTextureComponent2D>(transform, ContentManager<Texture2D>::Get().GetContent("unit"));
	this->team = CreateComponent<TeamComponent>();
	this->capture = CreateComponent<CaptureContributorComponent>(transform, team);
	this->select = CreateComponent<SelectableComponent>(transform, team);
	this->reciever = CreateComponent<CommandRecieverComponent>(transform, select);

	this->transform->location = location;
	this->team->team = t;

	this->draw->depth = tkv::DEPTH_UNIT;
	this->draw->size = 32.0f;

	this->reciever->speed = 64.0f;
	this->select->hoverRadius = 32.0f;
	this->select->priority = 1;
}

void TestUnitEntity::OnDelete()
{
	reciever->Delete();
	select->Delete();
	capture->Delete();
	team->Delete();
	draw->Delete();
	transform->Delete();
}

TestUnitEntity2::TestUnitEntity2(const Vector3& location)
{
	this->transform = CreateComponent<TransformComponent3D>();
	this->draw = CreateComponent<DrawModelComponent3D>(transform, ContentManager<Model>::Get().GetContent("monkey"));
	this->test = CreateComponent<TestComponent>(transform);

	this->transform->location = location;
	this->transform->rotation = Quaternion::AxisAngle(Vector3::UP, vlk::Pi);
}

void TestUnitEntity2::OnDelete()
{
	this->test->Delete();
	this->draw->Delete();
	this->transform->Delete();
}