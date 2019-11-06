#pragma once
#include <cstdlib>
#include <vector>
#include <iterator>
#include <mutex>

#include "Chunk.hpp"
#include "../core/EventBus.hpp"

namespace vlk
{
	template<class T>
	class ChunkAllocator final
	{
		public:
		static ChunkAllocator<T>& Get()
		{
			static ChunkAllocator<T> instance;
			return instance;
		}

		ChunkAllocator<T>(const ChunkAllocator<T>&) = delete;
		ChunkAllocator<T>(ChunkAllocator<T>&&) = delete;
		ChunkAllocator<T>& operator=(const ChunkAllocator<T>&) = delete;
		
		template<class... U>
		T* AllocateNew(U&&... args)
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			LogVerbose("ChunkAllocator " + TypeToString<T>(), "Allocating new object");

			for (auto it = chunks.begin(); it != chunks.end(); it++)
			{
				Chunk<T>& c = **it;
				if (!c.IsFull())
				{
					T* t = c.InsertNew(std::forward<U>(args)...);

					LogVerbose("ChunkAllocator " + TypeToString<T>(), "Object allocated at: " + PointerToString(t));

					return t;
				}
			}

			Chunk<T>* newChunk = new Chunk<T>();
			newChunk->Allocate();
			T* t = newChunk->InsertNew(std::forward<U>(args)...);

			LogVerbose("ChunkAllocator " + TypeToString<T>(), "Object allocated at: " + PointerToString(t));

			chunks.push_back(newChunk);
			return t;
		}
		
		template <class F>
		F ForEach(F f)
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			for (auto it = chunks.begin(); it != chunks.end(); it++)
			{
				Chunk<T>* c = *it;
				c->ForEach(f);
			}

			return std::move(f);
		}

		template <class U>
		T* Find(U pred)
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			T* t = nullptr;

			for (auto it = chunks.begin(); it != chunks.end(); it++)
			{
				Chunk<T>* c = *it;
				t = c->Find(pred);

				if (t) return t;
			}

			return nullptr;
		}

		ULong GetCount() const
		{
			ULong count = 0;

			for (auto it = chunks.begin(); it != chunks.end(); it++)
			{
				Chunk<T>& c = **it;

				count += c.GetCount();
			}

			return count;
		}

		void Delete(T* object)
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			LogVerbose("ChunkAllocator " + TypeToString<T>(), "Deleting " + PointerToString(object));
			
			for (auto it = chunks.begin(); it != chunks.end(); it++)
			{
				Chunk<T>& c = **it;

				if (c.Contains(object))
				{
					c.Remove(object);

					LogTrace("ChunkAllocator " + TypeToString<T>(), "Object removed from chunk.");

					return;
				}
			}

			LogWarning("ChunkAllocator " + TypeToString<T>(), "Failed to remove " + PointerToString(object));
		}

		void DeleteAll()
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			for (auto it = chunks.begin(); it != chunks.end(); it++)
			{
				Chunk<T>* c = *it;

				//c->RemoveAll();

				c->Free();
				delete c;
			}

			chunks.clear();
		}

		private:

		ChunkAllocator() 
		{
			EventBus<ApplicationExitEvent>::Get().AddEventListener(OnApplicationExit);
		}

		static void OnApplicationExit(ApplicationExitEvent& ev)
		{
			ChunkAllocator<T>::Get().DeleteAll();
		}

		//Probably don't need to worry about a destructor as alocations will be cleared 
		//when program terminates. Along with the object...

		std::vector<Chunk<T>*> chunks;
		std::mutex allocMtx;
	};
}