#pragma once

#include "../engine/core/ValkyrieEngine.h"

using namespace vlk;

namespace tkv
{
	constexpr Float DEPTH_MAP =			-100.0f;
	constexpr Float DEPTH_BUILDING =	-99.0f;
	constexpr Float DEPTH_UNIT =		0.0f;
	constexpr Float DEPTH_SELECTION =	98.0f;
	constexpr Float DEPTH_CAPTURE =		99.0f;
	constexpr Float DEPTH_CURSOR =		100.0f;
}

//The To-Do List:
//TODO: Add commanders
//TODO: Add win/loss
//TODO: Add Unit damage
//TODO: Add unit health
//TODO: Factory Production
//TODO: Resource Quarries
//TODO: Simulated Multiplayer