#pragma once

#include "ValkyrieEngine.h"
#include <vector>

namespace vlk
{
	struct Event
	{
		Event()
		{
			isCancelled = false;
		}

		virtual ~Event() {}

		inline operator Boolean() const { return !isCancelled; }

		void Cancel()
		{
			isCancelled = true;
		}

		Boolean IsCancelled() const
		{
			return isCancelled;
		}

		private:
		Boolean isCancelled;
	};

	struct LogEvent : Event
	{ 
		LogEvent(const std::string& source, UInt severity, const std::string& message) :
			Event(),
			source(source),
			severity(severity),
			message(message)
		{

		}

		const std::string& source; 
		const UInt severity; 
		const std::string& message; 
	};

	//Signals that the application has started
	struct ApplicationStartEvent : Event {};

	//Signals that the application is exiting
	struct ApplicationExitEvent : Event {};

	struct PreUpdateEvent : Event {};
	struct EarlyUpdateEvent : Event {};
	struct UpdateEvent : Event {};
	struct LateUpdateEvent : Event {};
	struct PostUpdateEvent : Event {};

	template<class T>
	class EventBus final
	{
		static_assert(std::is_base_of<Event, T>::value, "T must inherit from Event");
		static_assert(std::is_convertible<T*, Event*>::value, "T must publicly inherit from Event");

		//Typedef for any function that recieves the event T
		typedef void EventFun(T& ev);

		public:
		static EventBus<T>& Get()
		{
			static EventBus<T> instance;
			return instance;
		}

		void AddEventListener(EventFun* fun)
		{
			LogInfo("EventBus " + TypeToString<T>(), "Adding event listener: " + PointerToString(fun));
			callbacks.push_back(fun);
		}

		void RemoveEventListener(EventFun* fun)
		{
			LogInfo("EventBus " + TypeToString<T>(), "Removing event listener: " + PointerToString(fun));

			for (auto it = callbacks.begin(); it != callbacks.end(); it++)
			{
				EventFun* ef = (*it);
				
				if (ef == fun)
				{
					callbacks.erase(it);
					return;
				}
			}

			LogError("EventBus " + TypeToString<T>(), "Failed to remove event listener.");
		}

		void PostEvent(T ev)
		{
			for (auto it = callbacks.begin(); it != callbacks.end(); it++)
			{
				if (ev)
				{
					(*it)(ev);
				}
			}
		}

		private:
		EventBus<T>()
		{

		}

		std::vector<EventFun*> callbacks;
	};
}