#include "IGameEntity.h"

using namespace vlk;

IEntity::IEntity()
{
	this->enabled = true;
}

IEntity::~IEntity()
{

}
//
//const std::string& IEntity::GetTag() const
//{
//	static std::string tag;
//	return tag;
//}