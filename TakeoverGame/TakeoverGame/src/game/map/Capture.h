#pragma once

#include "../Takover.h"
#include "../GameManager.h"
#include "../components/TeamComponent.h"
#include "../components/FollowComponent.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawModelComponent.h"
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
		CaptureComponent(IEntity* e, const TransformComponent3D* transform, TeamComponent* team);
		void OnDelete() override;

		FollowComponent* follow;
		TransformComponent3D* transform;
		TeamComponent* team;

		Team capturingTeam;
		Double captureProgress;
		Double captureThreshold;
		Float captureRange;
		Boolean isPaused;

		//TODO: Move to seperate Entity
		DrawModelComponent3D* draw;
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
		CaptureContributorComponent(IEntity* e, const TransformComponent3D* const transform, const TeamComponent* const team);
		
		const TransformComponent3D* const transform;
		const TeamComponent* const team;
	};
}