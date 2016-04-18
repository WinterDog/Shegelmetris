#pragma once

// High-resolution time methods.
#include <chrono>

namespace wd
{
	//-----------------------------------------------------------------------------
	// High-resolution clock to properly measure frame times.
	// Uses std::chrono internally.
	//-----------------------------------------------------------------------------
	class HighResClock
	{
	private:
		// Short type names to avoid super-long strings.
		typedef std::chrono::high_resolution_clock	ChronoClock;
		typedef ChronoClock::time_point				ChronoTimePoint;

	private:
		// Time point of the clock creation or the last Reset() call.
		// When we request time, we actually get difference between this time point and the current time.
		ChronoTimePoint								m_lastTimePoint;

	public:
		HighResClock(
			void);

		~HighResClock(
			void);

	public:
		// Returns current timestamp (in seconds) from the beginning of an epoch
		// (usually 01.01.1970, but may vary, do not count on that).
		// Useful if we need absolute time value for comparison.
		static double GetTime(
			void);

		// Save current timestamp to the member.
		void Reset(
			void);

		// Get time passed since the last Reset() call in seconds.
		double GetElapsedSec(
			void) const;
		// Get time passed since the last Reset() call in milliseconds.
		double GetElapsedMs(
			void) const;
		// Get time passed since the last Reset() call in microseconds.
		double GetElapsedMcs(
			void) const;

	private:
	};
};
