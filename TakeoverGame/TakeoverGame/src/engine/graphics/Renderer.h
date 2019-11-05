#pragma once

#include "../core/ValkyrieEngine.h"
#include "../utils/Vector.h"
#include "../utils/Color.h"
#include "../core/EventBus.hpp"

#include <memory>

namespace vlk
{
	typedef void VLKrenderFun();
	namespace Renderer
	{
		void SetRenderer(VLKrenderFun* onInit, VLKrenderFun* onDraw, VLKrenderFun* onDestroy);

		void Init();
		void Draw(PostUpdateEvent& ev);
		void Destroy();
	}
}