#include "inc/stdafx.h"

#include "HighResClock.h"

//-----------------------------------------------------------------------------
// Static members
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constructors and destructors
//-----------------------------------------------------------------------------

wd::HighResClock::HighResClock(
	void)
{
	// Save current time for the first... time... :-|
	Reset();
}

wd::HighResClock::~HighResClock(
	void)
{
}

//-----------------------------------------------------------------------------
// Public methods
//-----------------------------------------------------------------------------

double wd::HighResClock::GetTime(
	void)
{
	// Current timestamp in chrono format.
	const wd::HighResClock::ChronoTimePoint nowTimeStamp = wd::HighResClock::ChronoClock::now();

	// Note that we always get time in mcs and then divide it if we need ms or sec.
	// It's because if we get time in ms or sec, it will be rounded,
	// and we probably don't want to miss so much time if we use high-res clock.
	return static_cast<double>(
		std::chrono::duration_cast<std::chrono::microseconds>(
			nowTimeStamp.time_since_epoch()).count()) / 1'000'000.0;
}

void wd::HighResClock::Reset(
	void)
{
	// Save current time to the member var.
	m_lastTimePoint = wd::HighResClock::ChronoClock::now();
}

double wd::HighResClock::GetElapsedSec(
	void) const
{
	return GetElapsedMcs() / 1'000'000.0;
}

double wd::HighResClock::GetElapsedMs(
	void) const
{
	return GetElapsedMcs() / 1'000.0;
}

double wd::HighResClock::GetElapsedMcs(
	void) const
{
	// Note that we always get time in mcs and then divide it if we need ms or sec.
	// It's because if we get time in ms or sec, it will be rounded,
	// and we probably don't want to miss so much time if we use high-res clock.
	return static_cast<double>(
		std::chrono::duration_cast<std::chrono::microseconds>(
			wd::HighResClock::ChronoClock::now() - m_lastTimePoint).count());
}

//-----------------------------------------------------------------------------
// Protected methods
//-----------------------------------------------------------------------------
