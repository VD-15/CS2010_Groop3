#pragma once

#include "../core/EventBus.hpp"

#include <mutex>
#include <algorithm>
#include <type_traits>

namespace vlk
{
	template <class T>
	class HeapAllocator final
	{
		public:
		static HeapAllocator<T>& Get()
		{
			static HeapAllocator<T> instance;
			return instance;
		}

		HeapAllocator<T>(const HeapAllocator<T>&) = delete;
		HeapAllocator<T>(HeapAllocator<T>&&) = delete;
		HeapAllocator<T>& operator=(const HeapAllocator<T>&) = delete;

		template<class... U>
		T* AllocateNew(U&&... args)
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			LogVerbose("HeapAllocator " + TypeToString<T>(), "Allocating new object");

			T* allocation = new T(std::forward<U>(args)...);

			LogVerbose("HeapAllocator " + TypeToString<T>(), "Object allocated at: " + PointerToString(allocation));

			allocations.push_back(allocation);
			return allocation;
		}

		template<class F>
		F ForEach(F f)
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			return std::for_each(allocations.begin(), allocations.end(), f);
		}

		ULong GetCount() const
		{
			return allocations.size();
		}

		void Delete(T* object)
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			LogVerbose("HeapAllocator " + TypeToString<T>(), "Deleting " + PointerToString(object));

			auto it = std::remove(allocations.begin(), allocations.end(), object);

			if (it != allocations.end())
			{
				object->~T();

				allocations.erase(it, allocations.end());
			}
			else
			{
				LogError("HeapAllocator " + TypeToString<T>(), "Failed to remove object: " + PointerToString(object));
			}
		}

		void DeleteAll()
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			allocations.clear();
		}

		private:
		HeapAllocator<T>()
		{
			EventBus<ApplicationExitEvent>::Get().AddEventListener(OnApplicationExit);
		}

		static void OnApplicationExit(ApplicationExitEvent& ev)
		{
			HeapAllocator<T>::Get().DeleteAll();
		}

		std::vector<T*> allocations;
		std::mutex allocMtx;
	};
}