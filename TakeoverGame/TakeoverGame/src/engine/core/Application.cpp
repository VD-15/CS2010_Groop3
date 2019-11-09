#include "Application.h"
#include "EventBus.hpp"

using namespace vlk;

namespace
{
	Boolean APPLICATION_RUNNING;
}

void Application::Run()
{
	LogInfo("Application", "Application starting.");
	APPLICATION_RUNNING = true;

	EventBus<ApplicationStartEvent>::Get().PostEvent({});

	while (APPLICATION_RUNNING)
	{
		EventBus<PreUpdateEvent>::Get().PostEvent({});
		EventBus<EarlyUpdateEvent>::Get().PostEvent({});
		EventBus<UpdateEvent>::Get().PostEvent({});
		EventBus<LateUpdateEvent>::Get().PostEvent({});
		EventBus<PostUpdateEvent>::Get().PostEvent({});
	}

	EventBus<ApplicationExitEvent>::Get().PostEvent({});
}

void Application::Exit()
{
	LogInfo("Application", "Application exiting.");
	APPLICATION_RUNNING = false;
}
