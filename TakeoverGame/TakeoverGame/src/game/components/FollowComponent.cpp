#include "FollowComponent.h"

#include "../../engine/core/VLKTime.h"

using namespace tkv;

namespace
{
	void OnLateUpdate(LateUpdateEvent& ev)
	{
		Float dt = VLKTime::DeltaTime<Float>();

		FollowComponent::ForEach([dt](FollowComponent* c)
		{
			if (c->followType & TKV_FOLLOW_X) c->follower->location.x = c->target->location.x;
			if (c->followType & TKV_FOLLOW_Y) c->follower->location.y = c->target->location.y;
			if (c->followType & TKV_FOLLOW_Z) c->follower->location.z = c->target->location.z;
		});
	}
}

void FollowSystem::Init()
{
	EventBus<LateUpdateEvent>::Get().AddEventListener(OnLateUpdate);
}

void FollowSystem::Destroy()
{
	EventBus<LateUpdateEvent>::Get().RemoveEventListener(OnLateUpdate);
}

FollowComponent::FollowComponent(IEntity* e, const TransformComponent3D* target, TransformComponent3D* follower) :
	Component<FollowComponent>(e),
	follower(follower)
{
	this->target = target;

	this->followType = TKV_FOLLOW_X | TKV_FOLLOW_Y | TKV_FOLLOW_Z;
}