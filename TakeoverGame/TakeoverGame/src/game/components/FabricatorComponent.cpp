#include "FabricatorComponent.h"

using namespace tkv;

namespace {

}

void FabricatorSystem::Init(){

}

void FabricatorSystem::Destroy(){

}

UnitFabricatorComponent::UnitFabricatorComponent(IEntity* e, Team t):
	Component<UnitFabricatorComponent>(e){
	this->team = t

}
