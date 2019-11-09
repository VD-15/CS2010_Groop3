#include "TeamComponent.h"

using namespace tkv;

TeamComponent::TeamComponent(IEntity* e) :
	Component<TeamComponent>(e)
{
	this->team = Team::Neutral;
}