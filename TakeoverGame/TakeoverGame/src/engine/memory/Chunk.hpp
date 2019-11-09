#pragma once

#include <stdlib.h>
#include <array>

#include "../core/ValkyrieEngine.h"

namespace vlk
{
	//Number of elements a chunk holds.
	static const ULong CHUNK_SIZE = 64;

	template <class T>
	class Chunk
	{
		public:
		Chunk<T>() :
			allocations {false}
		{
			LogVerbose("Chunk " + TypeToString<T>(), "Creating new chunk.");
			start = nullptr;
		}

		//Delete copy and move constructors as well as assignment operator.
		//We don't want chunks to be cloned as that would be HORRIBLY inefficient.
		Chunk<T>(const Chunk<T>& old) = delete;
		Chunk<T>(Chunk<T>&& old) = delete;
		Chunk<T>& operator=(const Chunk<T>& other) = delete;

		~Chunk<T>()
		{
			LogVerbose("Chunk " + TypeToString<T>(), "Deleting chunk.");
		}

		//Tells this chunk to allocate memory for itself.
		void Allocate()
		{
			LogTrace("Chunk " + TypeToString<T>(), "Allocating chunk.");

			start = (T*)malloc(CHUNK_SIZE * sizeof(T));
			instances.reserve(CHUNK_SIZE);

			LogVerbose("Chunk " + TypeToString<T>(), "Allocated at: " + PointerToString(start));
		}

		//Tells this chunk to free the memory it's holding.
		void Free()
		{
			free(start);
			start = nullptr;
		}

		//Insert a new element into this chunk.
		template<class... U>
		T* InsertNew(U&&... args)
		{
			LogVerbose("Chunk " + TypeToString<T>(), "Inserting new object.");

			//Find first free allocation
			for (ULong i = 0; i < CHUNK_SIZE; i++)
			{
				if (!allocations[i])
				{
					//Mark region as allocated
					allocations[i] = true;
					T* where = start + i;

					T* t = new(where) T(std::forward<U>(args)...);

					//Add to instances
					instances.push_back(where);

					return where;
				}
			}

			//If no allocation is found, return nullptr
			return nullptr;
		}

		template<class U>
		T* Find(U pred)
		{
			auto it = std::find_if(allocations.begin(), allocations.end(), pred);

			if (it == allocations.end())
			{
				return nullptr;
			}
			else
			{
				return *it;
			}
		}

		//Removes an element from this chunk.
		void Remove(T* allocation)
		{
			LogVerbose("Chunk " + TypeToString<T>(), "Deleting object " + PointerToString(allocation) + " from chunk: " + PointerToString(start));

			for (ULong i = 0; i < CHUNK_SIZE; i++)
			{
				if ((start + i) == allocation)
				{
					//Call destructor
					allocation->~T();

					//Mark region as unallocated
					allocations[i] = false;

					//Remove from instances
					instances.erase(std::remove(instances.begin(), instances.end(), allocation), instances.end());

					return;
				}
			}

			LogError("Chunk " + TypeToString<T>(), "Failed to remove object from chunk: " + PointerToString(allocation));
		}

		//Removes all elements from this chunk.
		void RemoveAll()
		{
			LogVerbose("Chunk " + TypeToString<T>(), "Deleting all objects in chunk: " + PointerToString(start));

			for (ULong i = 0; i < CHUNK_SIZE; i++)
			{
				if (allocations[i])
				{
					T* t = start + i;
					t->~T();
					allocations[i] = false;
				}
			}

			instances.clear();
		}

		//Returns true if this chunk is full.
		Boolean IsFull() const
		{
			return instances.size() >= instances.capacity();
		}

		//Returns true if this chunk is empty.
		Boolean IsEmpty() const
		{
			return instances.size() == 0;
		}

		//Gets the number of allocations this chunk holds
		ULong GetCount() const
		{
			return instances.size();
		}

		//Returns true if the pointer lies inside of this chunk.
		Boolean Contains(T* t) const
		{
			return (t >= start && t <= start + (CHUNK_SIZE - 1));
		}

		template<class F>
		F ForEach(F f)
		{
			return std::for_each(instances.begin(), instances.end(), f);
		}

		template<class F>
		T* FindIf(F f)
		{
			for (auto it = instances.begin(); it != instances.end(); it++)
			{
				if (f(*it))
				{
					return (*it);
				}
			}

			return nullptr;
		}

		private:
		std::array<Boolean, CHUNK_SIZE> allocations;
		std::vector<T*> instances;
		T* start;
	};
}