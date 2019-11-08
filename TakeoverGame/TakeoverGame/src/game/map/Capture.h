#pragma once

#include "../GameManager.h"
#include "../components/TeamComponent.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/UIComponent.h"

namespace tkv
{
	namespace CaptureSystem
	{
		void Init();
		void Destroy();
	}

	struct CaptureComponent : public Component<CaptureComponent>
	{
		CaptureComponent(IEntity* e, const TransformComponent2D* transform);

		const TransformComponent2D* transform;
		TeamComponent* team;

		Double captureProgress;
		Double captureThreshold;
	};

	struct CaptureBeginEvent : public Event
	{
		CaptureBeginEvent(const CaptureComponent* const capture);

		const CaptureComponent* const capture;
	};

	struct CaptureCompleteEvent : public Event
	{
		CaptureCompleteEvent(const CaptureComponent* const capture);

		const CaptureComponent* const capture;
	};

	struct CaptureStopEvent : public Event
	{
		CaptureStopEvent(const CaptureComponent* const capture);

		const CaptureComponent* const capture;
	};

	struct CaptureContestEvent : public Event
	{
		CaptureContestEvent(const CaptureComponent* const capture);

		const CaptureComponent* const capture;
	};

	struct CaptureVisualizerEntity : public Entity<CaptureVisualizerEntity>
	{
		CaptureVisualizerEntity(const CaptureComponent* const capture);
		void Delete() override;

		const CaptureComponent* const capture;


	};
}