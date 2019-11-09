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
		CaptureComponent(IEntity* e, const TransformComponent2D* transform, TeamComponent* team);

		const TransformComponent2D* transform;
		TeamComponent* team;

		Team capturingTeam;
		Double captureProgress;
		Double captureThreshold;
		Float captureRange;
		Boolean isPaused;
	};

	enum class CaptureAction
	{
		Begin,
		Complete,
		Fail,
		Pause,
		Resume
	};

	struct CaptureEvent : public Event
	{
		CaptureEvent(const CaptureComponent* const capture, CaptureAction action);

		const CaptureComponent* const capture;
		const CaptureAction action;
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
		void OnDelete() override;

		const CaptureComponent* const capture;
		DrawTextureComponent2D* draw;
	};
}