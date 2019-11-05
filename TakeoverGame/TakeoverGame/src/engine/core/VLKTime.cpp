#include "VLKTime.h"
#include "EventBus.hpp"

#include <chrono>

using namespace vlk;

namespace
{
	std::chrono::high_resolution_clock::time_point lastTick;
	std::chrono::high_resolution_clock::time_point thisTick;
	std::chrono::high_resolution_clock::time_point myEpoch;
	Double deltaTime = 0.0;
	Double vTime = 0.0;
}

void VLKTime::Init()
{
	myEpoch = std::chrono::high_resolution_clock::now();
	deltaTime = 0.0;
	vTime = 0.0;

	EventBus<ApplicationStartEvent>::Get().AddEventListener(OnStart);
	EventBus<PreUpdateEvent>::Get().AddEventListener(Tick);
}

void VLKTime::Destroy()
{
	EventBus<ApplicationStartEvent>::Get().RemoveEventListener(OnStart);
	EventBus<PreUpdateEvent>::Get().RemoveEventListener(Tick);
}

void VLKTime::OnStart(ApplicationStartEvent& ev)
{
	lastTick = std::chrono::high_resolution_clock::now();
	thisTick = lastTick;
}

void VLKTime::Tick(PreUpdateEvent& ev)
{
	LogTrace("VLKTime", "Ticking.");
	lastTick = thisTick;
	thisTick = std::chrono::high_resolution_clock::now();
	deltaTime = std::chrono::duration_cast<std::chrono::duration<Double>>(thisTick - lastTick).count();
	vTime = std::chrono::duration_cast<std::chrono::duration<Double>>(thisTick - myEpoch).count();
}

template<>
Double VLKTime::DeltaTime<Double>()
{
	return deltaTime;
}

template<>
Float VLKTime::DeltaTime<Float>()
{
	return static_cast<Float>(deltaTime);
}

template<>
Double VLKTime::VTime<Double>()
{
	return vTime;
}

template<>
Float VLKTime::VTime<Float>()
{
	return static_cast<Float>(vTime);
}

template<>
Double VLKTime::AbsVTime<Double>()
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::duration<Double>>(now - myEpoch).count();
}

template<>
Float VLKTime::AbsVTime<Float>()
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::duration<Float>>(now - myEpoch).count();
}