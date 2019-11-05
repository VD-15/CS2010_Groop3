#include "Timers.h"

#include <cmath>

using namespace vlk;

Timer::Timer()
{
	this->_myEpoch = std::chrono::high_resolution_clock::now();
}

Double Timer::GetElapsedTime() const
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::duration<Double>>(now - this->_myEpoch).count();
}

Ticker::Ticker()
{
	_elapsed = 0.0;
	_period = 1.0;
}

Ticker::Ticker(Double period)
{
	_elapsed = 0.0;
	_period = period;
}
/* none of this is going to work and I'm too tired to fix it.
ULong Ticker::Tick(Double delta)
{
	_elapsed += delta;

	return (ULong)std::trunc(_elapsed / _period);
}

Counter::Counter()
{
	_ticks = 0;
	_period = 1;
}

Counter::Counter(ULong period)
{
	_ticks = 0;
	_period = period;
}

ULong Counter::Tick(ULong count)
{
	_ticks += count;

	return _ticks / _period;
}
*/