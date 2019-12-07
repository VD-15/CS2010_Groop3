#include "UnitComponent.h"

using namespace tkv;

namespace
{
	//have a system to check if units are in range
	void OnLateUpdate(LateUpdateEvent& ev){

		UnitComponent::ForEach([](UnitComponent* c)
		{
			//check if unit is off cooldown
			if (this->cooldown <= 0){
				//check if unit is an enemy
				if (this->team->team != c->team->team){
					//get distance between enemy and this unit
					Float distance = Vector2::Magnitude(c->transform->location - this->transform->location);

					//check if the enemy is in aggro range
					if ((distance <= this->aggro)){
						//damage the enemy unit
						c->DamageEnemy(this->damage);
						//start the cooldown at the unit's attack speed
						this->cooldown = this->atkspd;
					}
				}
			}
		});
	void OnUpdate(UpdateEvent& ev){
		this->cooldown -= VLKTime::DeltaTime<Float>();
	}
}

void CombatSystem::Init()
{
	EventBus<LateUpdateEvent>::Get().AddEventListener(OnLateUpdate);
	EventBus<UpdateEvent>::Get().AddEventListener(OnUpdate);
}

void CombatSystem::Destroy()
{
	EventBus<LateUpdateEvent>::Get().RemoveEventListener(OnLateUpdate);
	EventBus<UpdateEvent>::Get().RemoveEventListener(OnUpdate);
}

UnitComponent::UnitComponent(IEntity* e, const ProtoUnit* p, Team t):
							Component<UnitComponent>(e){
	this->transform = CreateComponent<TransformComponent3D>();
	this->team = t;
	this->health = p->health;
	this->damage = p->damage;
	this->range = p->range;
	this->atkspd = p->atkspd;
	this->speed = p->speed;
	this->aggro = p->aggro;
	this->cooldown = 0;
	}
	

UnitComponent::DamageEnemy(double damage){
	this->health -= damage;
	if (this->health <= 0){
		this->GetParent()->OnDelete();
	}
}
/* not used in mvp
void UnitComponent::UpgadeUnit(){
	this->health *= 1.5;
	this->damage *= 1.5;
	this->tier +=1;
} */
