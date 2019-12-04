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
{
	EventBus<DamagedEvent>::Get().AddEventListener(DamagedEvent);
}

void CombatSystem::Destroy()
{
	EventBus<DamagedEvent>::Get().RemoveEventListener(DamagedEvent);
}

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

AttackEvent::AttackEvent(UnitComponent* c, double damage){
	c->EventBus<DamagedEvent>::Get().PostEvent({/*?*/});
	
}

DamagedEvent::DamagedEvent(double damage){
	this->health -= damage;
}
