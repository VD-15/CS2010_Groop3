#pragma once

#include "GameEntity.hpp"

namespace vlk
{
	template <class... T>
	class IFabricator
	{
		public:
		virtual Boolean Fabricate(T... args) const = 0;
	};

	template <class U, class... T>
	class Fabricator : public IFabricator<T...>
	{
		public:
		Fabricator()
		{

		}

		Boolean Fabricate(T... args) const override
		{
			static_assert(std::is_base_of<IEntity, U>::value, "U must be an entity.");
			static_assert(std::is_constructible<Entity<U>, T...>::value, "U must be constructable from arguments T...");

			return (Entity<U>::CreateEntity(args...) != nullptr);
		}
	};
}