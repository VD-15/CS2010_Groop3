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

		Team capturingTeam;
		Double captureProgress;
		Double captureThreshold;
		Float captureRange;
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

	struct CaptureContributorComponent : public Component<CaptureContributorComponent>
	{
		CaptureContributorComponent(IEntity* e, const TransformComponent2D* const transform, const TeamComponent* const team);
		
		const TransformComponent2D* const transform;
		const TeamComponent* const team;
	};

	struct CaptureVisualizerEntity : public Entity<CaptureVisualizerEntity>
	{
		CaptureVisualizerEntity(const CaptureComponent* const capture);
		void Delete() override;

		const CaptureComponent* const capture;
		DrawTextureComponent2D* draw;
	};
}