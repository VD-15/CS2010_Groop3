#include "Renderer.h"
#include "../core/EventBus.hpp"

using namespace vlk;

namespace
{
	VLKrenderFun* OnInit;
	VLKrenderFun* OnDraw; 
	VLKrenderFun* OnDestroy;
}

void Renderer::SetRenderer(VLKrenderFun* onInit, VLKrenderFun* onDraw, VLKrenderFun* onDestroy)
{
	OnInit = onInit;
	OnDraw = onDraw;
	OnDestroy = onDestroy;

	LogInfo("Renderer", "Init() set to: " + PointerToString(OnInit));
	LogInfo("Renderer", "Draw() set to: " + PointerToString(OnDraw));
	LogInfo("Renderer", "Destroy() set to: " + PointerToString(OnDestroy));
}

void Renderer::Init()
{
	LogInfo("Renderer", "Initializing.");

	if (!OnInit)
	{
		LogFatal("Renderer", "Init() function was not set before initializing.");
		throw std::exception("Init() function was null.");
	}

	OnInit();

	EventBus<PostUpdateEvent>::Get().AddEventListener(Draw);
}

void Renderer::Draw(PostUpdateEvent& ev)
{
	LogTrace("Renderer", "Drawing.");

	OnDraw();
}

void Renderer::Destroy()
{
	LogInfo("Renderer", "Destroying.");

	OnDestroy();
	EventBus<PostUpdateEvent>::Get().AddEventListener(Draw);
}