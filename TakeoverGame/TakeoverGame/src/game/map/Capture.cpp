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
				if ((c->transform->location - cap->transform->location).Magnitude() < c->captureRange)
				{
					if (c->capturingTeam == Team::Neutral)
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

			Double captureAmount = VLKTime::DeltaTime() * numCapturing;

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
							LogInfo("CaptureComponent", "Capture begin");
						}
						else
						{
							if (c->isPaused)
							{
								c->isPaused = false;
								EventBus<CaptureEvent>::Get().PostEvent({ c, CaptureAction::Resume });
								LogInfo("CaptureComponent", "Capture resume");
							}

							c->captureProgress += captureAmount;

							if (c->captureProgress > c->captureThreshold)
							{
								c->team->team = c->capturingTeam;
								c->captureProgress = 0.0;
								EventBus<CaptureEvent>::Get().PostEvent({ c, CaptureAction::Complete });
								LogInfo("CaptureComponent", "Capture complete");
							}
						}
					}
					else
					{
						if (!c->isPaused)
						{
							EventBus<CaptureEvent>::Get().PostEvent({ c, CaptureAction::Pause });
							LogInfo("CaptureComponent", "Capture pause");
						}
					}
				}
				else
				{
					c->captureProgress -= captureAmount;

					if (c->captureProgress < 0.0)
					{
						c->capturingTeam = Team::Neutral;
						c->isPaused = false;
						EventBus<CaptureEvent>::Get().PostEvent({ c, CaptureAction::Fail });
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

CaptureComponent::CaptureComponent(IEntity* e, const TransformComponent2D* transform, TeamComponent* team) :
	Component<CaptureComponent>(e)
{
	this->transform = transform;
	this->team = team;

	this->captureProgress = 0.0;
	this->captureThreshold = 10.0;
	this->captureRange = 64.0f;
	this->capturingTeam = Team::Neutral;
	this->isPaused = false;
}

CaptureEvent::CaptureEvent(const CaptureComponent* const capture, CaptureAction action) :
	capture(capture),
	action(action)
{

}

CaptureContributorComponent::CaptureContributorComponent(IEntity* e, const TransformComponent2D* const transform, const TeamComponent* const team) :
	Component<CaptureContributorComponent>(e),
	transform(transform),
	team(team)
{

}

CaptureVisualizerEntity::CaptureVisualizerEntity(const CaptureComponent* const capture) :
	capture(capture)
{
	this->draw = CreateComponent<DrawTextureComponent2D>(capture->transform, ContentManager<Texture2D>::Get().GetContent("capture_0"));
}

void CaptureVisualizerEntity::OnDelete()
{
	draw->Delete();
}