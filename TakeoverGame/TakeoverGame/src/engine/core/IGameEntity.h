#pragma once

#include "ValkyrieEngine.h"

namespace vlk
{
	class IEntity
	{
		public:
		IEntity();
		virtual ~IEntity();

		Boolean enabled;

		virtual void Delete() = 0;
	};
}
