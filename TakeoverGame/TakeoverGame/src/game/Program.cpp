#include "../engine/core/ValkyrieEngine.h"
#include "../engine/core/Application.h"

int main()
{
	vlk::Init();

	vlk::Application::Run();

	vlk::Destroy();
}