#pragma once

#include "../GameManager.h"
#include "../components/TeamComponent.h"
#include "../components/Selectable.h"
#include "../map/Capture.h"
#include "../components/ResourceFabricatorComponent.h"

#include "../../engine/core/GameEntity.hpp"
#include "../../engine/components/TransformComponent.h"
#include "../../engine/components/DrawTextureComponent.h"
#include "../../engine/components/UIComponent.h"


using namespace vlk;

namespace tkv
{
    struct QuarryEntity : public Entity<QuarryEntity>
    {
        QuarryEntity(Vector2 v);
        void OnDelete() override;

        TransformComponent3D* transform;
		DrawTextureComponent2D* draw;
		TeamComponent* team;
		CaptureComponent* capture;
		SelectableComponent* select;
        ResourceFabricatorComponent* fabricate;
    }
}