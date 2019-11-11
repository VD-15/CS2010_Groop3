#pragma once

#include "ValkyrieEngine.h"
#include "IGameEntity.h"
#include "GameComponent.hpp"
#include "../memory/ChunkAllocator.hpp"
#include "../memory/HeapAllocator.hpp"

namespace vlk
{
	struct HeapAllocateEntity {};

	template <class T>
	class Entity : public IEntity
	{
		public:
		Entity() {}
		virtual ~Entity() {}

		Entity(const Entity& other) = delete;
		Entity(Entity&& other) = delete;
		Entity& operator=(const Entity& other) = delete;

		template<class... U>
		static T* CreateEntity(U&&... args)
		{
			T* t = nullptr;

			if constexpr (std::is_base_of<HeapAllocateEntity, T>::value)
			{
				LogTrace("Entity " + TypeToString<T>(), "Creating Entity on heap.");

				t = HeapAllocator<T>::Get().AllocateNew(std::forward<U>(args)...);
			}
			else
			{
				LogTrace("Entity " + TypeToString<T>(), "Creating Entity in chunk.");

				t = ChunkAllocator<T>::Get().AllocateNew(std::forward<U>(args)...);
			}

			LogVerbose("Entity " + TypeToString<T>(), "Entity created: " + PointerToString(t));

			return t;
		}

		template<class U, class... V>
		U* CreateComponent(V&&... args)
		{
			LogVerbose("Entity " + TypeToString<T>(), "Creating Component for: " + PointerToString(this));

			static_assert(std::is_base_of<IComponent, U>::value, "U must inherit from IComponent");

			return Component<U>::CreateComponent(this, std::forward<V>(args)...);
		}

		//This will crash the entire application if you're calling this from within a ForEach.
		//Don't call this from within a ForEach. Please.
		void Delete() final override
		{
			LogVerbose("Entity " + TypeToString<T>(), "Deleting Immediately: " + PointerToString(this));

			OnDelete();

			if constexpr (std::is_base_of<HeapAllocateEntity, T>::value)
			{
				HeapAllocator<T>::Get().Delete(dynamic_cast<T*>(this));
			}
			else
			{
				ChunkAllocator<T>::Get().Delete(dynamic_cast<T*>(this));
			}
		}

		protected:
		virtual void OnDelete() = 0;
	};
}
