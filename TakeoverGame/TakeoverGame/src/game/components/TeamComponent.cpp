#include "TeamComponent.h"

using namespace tkv;

TeamComponent::TeamComponent(IEntity* e, Team t) :
	Component<TeamComponent>(e)
{
	this->team = t;
}