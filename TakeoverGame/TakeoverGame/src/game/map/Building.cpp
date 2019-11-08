#include "Building.h"

using namespace tkv;

CaptureComponent::CaptureComponent(IEntity* e, const TransformComponent2D* transform) :
	Component<CaptureComponent>(e)
{
	this->transform = transform;
	this->team = TeamComponent::CreateComponent(e);

	this->captureProgress = 0.0;
	this->captureThreshold = 10.0;
}