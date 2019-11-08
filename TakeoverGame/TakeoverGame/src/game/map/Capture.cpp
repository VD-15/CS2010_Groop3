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

			if (numCapturing > 0)
			{
				if (numContesting == 0)
				{
					c->captureProgress += captureAmount;
				}


			}
			else
			{
				c->captureProgress -= captureAmount;
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

CaptureComponent::CaptureComponent(IEntity* e, const TransformComponent2D* transform) :
	Component<CaptureComponent>(e)
{
	this->transform = transform;
	this->team = TeamComponent::CreateComponent(e);

	this->captureProgress = 0.0;
	this->captureThreshold = 10.0;
	this->captureRange = 64.0f;
}

CaptureBeginEvent::CaptureBeginEvent(const CaptureComponent* const capture) :
	capture(capture)
{

}

CaptureCompleteEvent::CaptureCompleteEvent(const CaptureComponent* const capture) :
	capture(capture)
{

}

CaptureStopEvent::CaptureStopEvent(const CaptureComponent* const capture) :
	capture(capture)
{

}

CaptureContestEvent::CaptureContestEvent(const CaptureComponent* const capture) :
	capture(capture)
{

}

CaptureVisualizerEntity::CaptureVisualizerEntity(const CaptureComponent* const capture) :
	capture(capture)
{
	this->draw = CreateComponent<DrawTextureComponent2D>(capture->transform, ContentManager<Texture2D>::Get().GetContent("capture_0"));
}

void CaptureVisualizerEntity::Delete()
{
	draw->Delete();
}