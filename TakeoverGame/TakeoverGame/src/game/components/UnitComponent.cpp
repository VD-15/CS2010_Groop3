#include "UnitComponent.h"

using namespace tkv;

namespace
{
//have a system to check if units are in range
void OnLateUpdate(LateUpdateEvent& ev){

}
//use on lateupdate - so units react after theyve moved
}


}

void CombatSystem::Init()
{

}

void CombatSystem::Destroy()
{

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
							}

