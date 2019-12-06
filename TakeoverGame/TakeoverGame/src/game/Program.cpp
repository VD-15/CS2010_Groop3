#include "../engine/core/ValkyrieEngine.h"
#include "../engine/core/Application.h"
#include "../engine/core/Window.h"
#include "../engine/core/EventBus.hpp"
#include "../engine/utils/Logger.h"

#include "GameManager.h"

using namespace vlk;

namespace
{
	vlk::Logger logger;
	std::mutex mtx;

	void OnLog(LogEvent& ev)
	{
		std::lock_guard guard(mtx);
		logger.Log(ev.source, ev.severity, ev.message);
	}
}

int main()
{
	//Create logger
	logger.Open("logs/latest.log");
	logger.EnableSecondaryOutput();
	logger.SetLogLevel(VLK_LOG_LEVEL_VERBOSE);

	EventBus<LogEvent>::Get().AddEventListener(OnLog);

	vlk::Init();

	//Lock framerate to refresh rate
	vlk::Window::SetSwapInterval(1);
	//vlk::Window::HideCursor();

	tkv::GameManager::Init();

	vlk::Application::Run();

	tkv::GameManager::Destroy();

	vlk::Destroy();
}