#pragma once

#include <chrono>

#include "../core/ValkyrieEngine.h"

namespace vlk
{
	//Stores the time elapsed since the object was created.
	class Timer
	{
		public:
		Timer();

		Double GetElapsedTime() const;

		private:
		std::chrono::high_resolution_clock::time_point _myEpoch;
	};

	//Continuous timer that ticks when a given period is reached.
	class Ticker
	{
		public:
		//Create a Ticker with a period of 1.0.
		Ticker();

		//Create a ticker with the given period.
		Ticker(Double period);

		//Advance the Ticker and return how many periods have elapsed.
		ULong Tick(Double delta);

		//Get the total time that has elapsed in this Ticker.
		inline Double GetElapsedTime() const { return _elapsed; }

		//Get the period of this Ticker.
		inline Double GetPeriod() const { return _period; }

		private:
		Double _elapsed;
		Double _period;
	};

	//Counts integer values and ticks when a given period is hit.
	class Counter
	{
		public:
		//Create a counter with a period of 1.
		Counter();

		//Create a counter with the given period.
		Counter(ULong period);

		//Advance the counter and return how many periods have elapsed.
		ULong Tick(ULong count);

		//Get how many ticks this Counter has counted.
		//This does not return the number of periods that have elapsed.
		inline ULong GetTicks() const { return _ticks; }

		//Gets the period of this Counter.
		inline ULong GetPeriod() const { return _period; }

		private:
		ULong _ticks;
		ULong _period;
	};
}