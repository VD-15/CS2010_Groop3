#include "UnitComponent.h"

using namespace tkv;

namespace
{
	//have a system to check if units are in range
	void OnLateUpdate(LateUpdateEvent& ev){
		UnitComponent::ForEach([](UnitComponent* c)
		{
			
		});
	
}

void CombatSystem::Init()
{}

void CombatSystem::Destroy()
{}

UnitComponent::UnitComponent(IEntity* e, const ProtoUnit* p, Team t):
							Component<UnitComponent>(e)
							{
	this->team = t;
	this->health = p->health;
	this->damage = p->damage;
	this->range = p->range;
	this->atkspd = p->atkspd;
	this->speed = p->speed;
	this->aggro = p->aggro;
	this->foundEnemy = false;
	}

AttackEvent::AttackEvent(IEntity* e, double damage){
	//raise damaged event of enitiy?
	
}

DamagedEvent::DamagedEvent(double damage){
	this->health -= damage;
}