#pragma once

#include "ValkyrieEngine.h"

namespace vlk
{
	//TODO: dynamically allocated / deleted components

	class IEntity
	{
		public:
		IEntity();
		virtual ~IEntity();

		Boolean enabled;

		virtual void Delete() = 0;
	};
}
