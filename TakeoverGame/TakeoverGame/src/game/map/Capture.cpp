#include "Capture.h"
#include "../../engine/core/ContentManager.hpp"
#include "../../engine/core/VLKTime.h"

using namespace tkv;

namespace
{
	void OnUpdate(UpdateEvent& ev)
	{
		auto capture = CaptureComponent::ForEach([](CaptureComponent* c)
		{
			Int numCapturing = 0;
			Int numContesting = 0;

			auto find = CaptureContributorComponent::ForEach([&c, &numCapturing, &numContesting](CaptureContributorComponent* cap)
			{
				if (cap->team->team == c->team->team) return;

				if (Distance(c->follow->target->location, cap->transform->location) < c->captureRange)
				{
					if (c->capturingTeam == Team::Neutral && cap->team->team != c->team->team)
					{
						c->capturingTeam = cap->team->team;
					}

					if (cap->team->team == c->capturingTeam)
					{
						numCapturing++;
					}
					else
					{
						numContesting++;
					}
				}
			});

			Double captureAmount = VLKTime::DeltaTime() * (numCapturing + 1);
			c->transform->location.y = 12.5f + (c->captureProgress * 10.0f / c->captureThreshold);

			if (c->capturingTeam != Team::Neutral)
			{
				if (numCapturing > 0)
				{
					if (numContesting == 0)
					{
						if (c->captureProgress <= 0.0)
						{
							c->captureProgress = captureAmount;
							EventBus<CaptureEvent>::Get().PostEvent({ c, CaptureAction::Begin });

							//c->draw = DrawTextureComponent2D::CreateComponent(c->GetParent(), c->transform, ContentManager<Texture2D>::Get().GetContent("cap_0"));
							//c->draw->depth = tkv::DEPTH_CAPTURE;	
							//c->draw->origin = Vector2(0.5f, 0.0f);
							c->transform->scale = Vector3(5.0f);

							LogInfo("CaptureComponent", "Capture begin");
						}
						else
						{
							if (c->isPaused)
							{
								c->isPaused = false;
								EventBus<CaptureEvent>::Get().PostEvent({ c, CaptureAction::Resume });

								//c->draw->color = Color::WHITE;

								LogInfo("CaptureComponent", "Capture resume");
							}

							c->captureProgress += captureAmount;

							//c->draw->texture = ContentManager<Texture2D>::Get().GetContent("cap_" + std::to_string(static_cast<Int>(c->captureProgress * 10.0 / c->captureThreshold)));

							if (c->captureProgress > c->captureThreshold)
							{
								c->team->team = c->capturingTeam;
								c->capturingTeam = Team::Neutral;
								c->captureProgress = 0.0;
								EventBus<CaptureEvent>::Get().PostEvent({ c, CaptureAction::Complete });

								c->transform->scale = Vector3(0.0f);

								LogInfo("CaptureComponent", "Capture complete");
							}
						}
					}
					else
					{
						if (!c->isPaused)
						{
							EventBus<CaptureEvent>::Get().PostEvent({ c, CaptureAction::Pause });

							//c->draw->color = Color(0.5f, 0.5f, 0.5f, 1.0f);

							LogInfo("CaptureComponent", "Capture pause");
						}
					}
				}
				else
				{
					c->captureProgress -= captureAmount;

					//c->draw->texture = ContentManager<Texture2D>::Get().GetContent("cap_" + std::to_string(static_cast<Int>(c->captureProgress * 10.0 / c->captureThreshold)));

					if (c->captureProgress < 0.0)
					{
						c->capturingTeam = Team::Neutral;
						c->isPaused = false;
						EventBus<CaptureEvent>::Get().PostEvent({ c, CaptureAction::Fail });

						c->transform->scale = Vector3(0.0f);

						LogInfo("CaptureComponent", "Capture fail");
					}
				}
			}
		});
	}
}

void CaptureSystem::Init()
{
	EventBus<UpdateEvent>::Get().AddEventListener(OnUpdate);
}

void CaptureSystem::Destroy()
{
	EventBus<UpdateEvent>::Get().RemoveEventListener(OnUpdate);
}

CaptureComponent::CaptureComponent(IEntity* e, const TransformComponent3D* t, TeamComponent* team) :
	Component<CaptureComponent>(e)
{
	this->team = team;

	this->transform = TransformComponent3D::CreateComponent(e);
	this->follow = FollowComponent::CreateComponent(e, t, this->transform);
	this->draw = DrawModelComponent3D::CreateComponent(e, this->transform, ContentManager<Model>::Get().GetContent("capture"));

	this->captureProgress = 0.0;
	this->captureThreshold = 5.0;
	this->captureRange = 64.0f;
	this->capturingTeam = Team::Neutral;
	this->isPaused = false;
	this->transform->scale = Vector3(0.0f);
	this->transform->location.y = 25.0f;
	this->follow->followType = TKV_FOLLOW_X | TKV_FOLLOW_Z;

}

void CaptureComponent::OnDelete()
{
	this->follow->Delete();
	this->transform->Delete();
	this->draw->Delete();
}

CaptureEvent::CaptureEvent(const CaptureComponent* const capture, CaptureAction action) :
	capture(capture),
	action(action)
{

}

CaptureContributorComponent::CaptureContributorComponent(IEntity* e, const TransformComponent3D* const transform, const TeamComponent* const team) :
	Component<CaptureContributorComponent>(e),
	transform(transform),
	team(team)
{

}