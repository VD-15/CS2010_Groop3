#include "../engine/core/ValkyrieEngine.h"
#include "../engine/core/Application.h"
#include "../engine/core/Window.h"
#include "../engine/utils/Logger.h"

#include "GameManager.h"

namespace
{
	vlk::Logger logger;
}

int main()
{
	vlk::Init();

	vlk::Window::SetSwapInterval(1);

	

	tkv::GameManager::Init();

	vlk::Application::Run();

	tkv::GameManager::Destroy();

	vlk::Destroy();
}