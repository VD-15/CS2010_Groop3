#pragma once

#include "ValkyrieEngine.h"
#include "EventBus.hpp"

namespace vlk
{
	namespace VLKTime
	{
		void Init();
		void Destroy();

		void OnStart(ApplicationStartEvent& ev);
		void Tick(PreUpdateEvent& ev);

		//Time since last game tick was completed.
		template <class T = Double>
		T DeltaTime();

		//Time since VLKTime::Init() was called. Accurate to the nearest game tick.
		template <class T = Double>
		T VTime();

		//Tike since VLKTime::Init() was called. Accurate to the system clock.
		template <class T = Double>
		T AbsVTime();
	}
}