#include "ValkyrieEngine.h"

#define GLEW_STATIC
#include "GLEW/glew.h"

#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <exception>
#include <sstream>
#include <string>
#include <iomanip>

#include "../graphics/OpenGL/GLRenderer.h"
#include "../components/ColliderComponent.h"
#include "../components/CameraComponent.h"
#include "../components/UIComponent.h"
#include "../utils/Logger.h"
#include "ContentManager.hpp"
#include "VLKTime.h"
#include "VLKRandom.h"
#include "Window.h"
#include "Application.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Game.h"

using namespace vlk;

namespace
{
	Boolean VLK_INITIALIZED = false;
}

void vlk::Init()
{
	if (!VLK_INITIALIZED)
	{
		VLKTime::Init();
		VLKRandom::Init();
		Window::Init();
		Keyboard::Init();
		Mouse::Init();

		//TODO: Expose this somewhere
		Renderer::SetRenderer(GLRenderer::Init, GLRenderer::Draw, GLRenderer::Destroy);
		Renderer::Init();

		CollisionSystem::Init();
		Camera::Init();
		UISystem::Init();

		ContentSystem::Init();

		LogInfo("Valkyrie Engine", "VLKinit took " + std::to_string(VLKTime::AbsVTime<Double>()) + " Seconds");

		VLK_INITIALIZED = true;
	}
}

void vlk::Destroy()
{
	if (VLK_INITIALIZED)
	{
		UISystem::Destroy();
		Camera::Destroy();
		CollisionSystem::Destroy();

		Renderer::Destroy();
		ContentSystem::Destroy();
		Mouse::Destroy();
		Keyboard::Destroy();
		Window::Destroy();
		VLKRandom::Destroy();
		VLKTime::Destroy();
		VLK_INITIALIZED = false;

		LogInfo("Valkyrie Engine", "Destroyed Successfully.");
	}
}

void vlk::LogTrace(const std::string& source, const std::string& message)
{
	#ifdef _DEBUG
	EventBus<LogEvent>::Get().PostEvent(LogEvent(source, vlk::VLK_LOG_LEVEL_TRACE, message));
	#endif
}

void vlk::LogVerbose(const std::string& source, const std::string& message)
{
	#ifdef _DEBUG
	EventBus<LogEvent>::Get().PostEvent(LogEvent(source, vlk::VLK_LOG_LEVEL_VERBOSE, message));
	#endif
}

void vlk::LogInfo(const std::string& source, const std::string& message)
{
	#ifdef _DEBUG
	EventBus<LogEvent>::Get().PostEvent(LogEvent(source, vlk::VLK_LOG_LEVEL_INFO, message));
	#endif
}

void vlk::LogWarning(const std::string& source, const std::string& message)
{
	EventBus<LogEvent>::Get().PostEvent(LogEvent(source, vlk::VLK_LOG_LEVEL_WARNING, message));
}

void vlk::LogError(const std::string& source, const std::string& message)
{
	EventBus<LogEvent>::Get().PostEvent(LogEvent(source, vlk::VLK_LOG_LEVEL_ERROR, message));
}

void vlk::LogFatal(const std::string& source, const std::string& message)
{
	EventBus<LogEvent>::Get().PostEvent(LogEvent(source, vlk::VLK_LOG_LEVEL_FATAL, message));
}

void vlk::LogAnnounce(const std::string& source, const std::string& message)
{
	EventBus<LogEvent>::Get().PostEvent(LogEvent(source, vlk::VLK_LOG_LEVEL_ANNOUNCE, message));
}

std::string vlk::PointerToString(const void* ptr)
{
	std::stringstream ss;
	ss << "0x" << std::hex << ptr;
	return ss.str();
}

std::string vlk::TypeInfoToString(const std::type_info& t)
{
	std::string s(t.name());
	std::stringstream ss(s.substr(6, std::string::npos));
	std::string item;
	while (std::getline(ss, item, ':')) {}
	return item;
}

Boolean vlk::StringEndsWith(const std::string& str, const std::string& end)
{
	if (end.size() > str.size()) return false;
	return std::equal(end.crbegin(), end.crend(), str.crbegin());
}

Boolean vlk::StringBeginsWith(const std::string& str, const std::string& beg)
{
	if (beg.size() > str.size()) return false;
	return std::equal(beg.cbegin(), beg.cend(), str.cbegin());
}

const char* vlk::DuplicateString(const char* src)
{
	UInt len = static_cast<UInt>(strlen(src)) + 1;

	char* str = new char[len];

	strcpy_s(str, len, src);

	return str;
}