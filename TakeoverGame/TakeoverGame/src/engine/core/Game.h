//This was a fucking mess...

//#pragma once
//
//#include <vector>
//#include <functional>
//
//#include "ValkyrieEngine.h"
//#include "GameEntity.hpp"
//#include "GameComponent.hpp"
//
//namespace vlk
//{
//	namespace Game
//	{
//		void Init();
//		void Destroy();
//		
//		template<class T, class... U>
//		T* CreateEntity(U... args)
//		{
//			static_assert(std::is_base_of<IEntity, T>::value, "T must inherit from IEntity");
//			LogVerbose("Game", "Creating entity");
//
//			return new(ChunkAllocator<T>::Get().AllocateNew()) T(args...);
//		}
//
//		template<class T>
//		std::vector<T*> GetEntities()
//		{
//			static_assert(std::is_base_of<IEntity, T>::value, "T must inherit from IEntity");
//
//			return ChunkAllocator<T>::Get().GetAll();
//		}
//		
//		template<class T>
//		void DeleteEntity(T* entity)
//		{
//			static_assert(std::is_base_of<IEntity, T>::value, "T must inherit from IEntity");
//
//			ChunkAllocator<T>::Get().Delete(entity);
//		}
//
//		template<class T, class... U>
//		T* CreateComponent(const IEntity* e, U... args)
//		{
//			static_assert(std::is_base_of<IComponent, T>::value, "T must inherit from IComponent");
//
//			T* t = new(ChunkAllocator<T>::Get().AllocateNew()) T(e, args...);
//
//			return t;
//		}
//
//		template<class T, class F>
//		F ForEachComponent(F f)
//		{
//			static_assert(std::is_base_of<IComponent, T>::value, "T must inherit from IComponent");
//
//			return ChunkAllocator<T>::Get().ForEach(f);
//		}
//
//		template<class T>
//		std::vector<T*> GetComponents()
//		{
//			static_assert(std::is_base_of<IComponent, T>::value, "T must inherit from IComponent");
//
//			return ChunkAllocator<T>::Get().GetAll();
//		}
//
//		template<class T>
//		void DeleteComponent(T* component)
//		{
//			static_assert(std::is_base_of<IComponent, T>::value, "T must inherit from IComponent");
//
//			ChunkAllocator<T>::Get().Delete(component);
//		}
//
//		IEntity* GetEntityByID(ULong ID)
//		{
//
//		}
//
//		IComponent* GetComponentByID(ULong ID)
//		{
//
//		}
//
//		void DeleteEntityByID(ULong ID)
//		{
//			EventBus<DeleteEntityEvent>::Get().PostEvent({ ID });
//		}
//
//		void DeleteComponentByID(ULong cID)
//		{
//			EventBus<DeleteComponentEvent>::Get().PostEvent({ cID });
//		}
//	}
//}