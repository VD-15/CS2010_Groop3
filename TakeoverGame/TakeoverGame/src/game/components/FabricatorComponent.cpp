#include "FabricatorComponent.h"
#include "UnitEntity.h"
using namespace tkv;

namespace {
	void OnUpdate(UpdateEvent& ev){
		UnitFabricatorComponent::ForEach([](UnitFabricatorComponent* c)
		{
			if((c->team->team != Neutral) && (c->cooldown <= 0)){
				UnitEntity::CreateEntity(c->team->team, c->transform->location, c->unit);
				c->cooldown=5;
			}			
		});
		
		ResourceFabricatorComponent::ForEach([](ResourceFabricatorComponent* c)
		{
			if((c->team->team != Neutral) && (c->cooldown <= 0)){
				//not sure how to get the resource variable from each of the players to increment the resource
			}
		});
		
	}
}

void FabricatorSystem::Init(){
	EventBus<UpdateEvent>::Get().AddEventListener(OnUpdate);
}

void FabricatorSystem::Destroy(){
	EventBus<UpdateEvent>::Get().RemoveEventListener(OnUpdate);
}

UnitFabricatorComponent::UnitFabricatorComponent(IEntity* e, TransformComponent3D* transform, ProtoUnit* unit, Team t):
	Component<UnitFabricatorComponent>(e){
	this->team = t;
	this->unit = unit;
	this->transform = t;
	this->transform->location.x += 5.0f;
	this->transform->location.z += 5.0f;
	this->cooldown = 5;
}

UnitFabricatorComponent::ChangeUnit(ProtoUnit* p){
	this->unit = p;
}

ResourceFabricatorComponent::ResourceFabricatorComponent(IEntity* e):
	Component<ResourceFabricatorComponent>(e){
	this->team = t;
	this->cooldown = 5;
}