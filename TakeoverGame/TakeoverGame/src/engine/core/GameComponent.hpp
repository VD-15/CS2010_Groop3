#pragma once

#include "ValkyrieEngine.h"
#include "IGameComponent.h"
#include "IGameEntity.h"
#include "../memory/ChunkAllocator.hpp"
#include "../memory/HeapAllocator.hpp"

namespace vlk
{
	struct HeapAllocateComponent {};

	template<class T>
	class Component : public IComponent
	{
		public:
		Component(IEntity* e) : IComponent(e) {}
		virtual ~Component() {}

		Component() = delete;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;

		template<class... V>
		static T* CreateComponent(IEntity* entity, V&&... args)
		{
			LogVerbose("Component " + TypeToString<T>(), "Creating Component");
			
			T* c;

			if constexpr (std::is_base_of<HeapAllocateComponent, T>::value)
			{
				LogTrace("Component " + TypeToString<T>(), "Allocating component on heap");

				c = HeapAllocator<T>::Get().AllocateNew(entity, std::forward<V>(args)...);
			}
			else
			{
				LogTrace("Component " + TypeToString<T>(), "Allocating component in chunk");

				c = ChunkAllocator<T>::Get().AllocateNew(entity, std::forward<V>(args)...);
			}

			return c;
		}

		template<class F>
		static F ForEach(F f)
		{
			LogTrace("Component " + TypeToString<T>(), "Executing ForEach");

			if constexpr (std::is_base_of<HeapAllocateComponent, T>::value)
			{
				return HeapAllocator<T>::Get().ForEach(f);
			}
			else
			{
				return ChunkAllocator<T>::Get().ForEach(f);
			}
		}

		static T* Get(IEntity* e)
		{
			T* t = nullptr;

			if constexpr (std::is_base_of<HeapAllocateComponent, T>::value)
			{
				return HeapAllocator<T>::Get().Find([e](T* t)
				{
					return t->GetParent() == e;
				});
			}
			else
			{
				return ChunkAllocator<T>::Get().Find([e](T* t)
				{
					return t->GetParent() == e;
				});
			}
		}

		static ULong GetCount()
		{
			if constexpr (std::is_base_of<HeapAllocateComponent, T>::value)
			{
				return HeapAllocator<T>::Get().GetCount();
			}
			else
			{
				return ChunkAllocator<T>::Get().GetCount();
			}
		}

		virtual void Delete()
		{
			LogVerbose("Component " + TypeToString<T>(), "Deleting component: " + TypeToString<T>());

			if constexpr (std::is_base_of<HeapAllocateComponent, T>::value)
			{
				HeapAllocator<T>::Get().Delete(dynamic_cast<T*>(this));
			}
			else
			{
				ChunkAllocator<T>::Get().Delete(dynamic_cast<T*>(this));
			}
		}
	};
}