//**********************************************************************************************************************
#pragma once

#include "types.h"

//**********************************************************************************************************************
namespace uu //univubi
{

//**********************************************************************************************************************
class Time
{
public:
	/// \def  SecondsInDay
	/// \brief  The number of seconds in a day
	static time_t const SecondsInDay = (24 * 60 * 60);

	/// \def  SecondsInYear
	/// \brief  The number of seconds in a year
	static time_t const SecondsInYear = (365 * SecondsInDay);

	/// \def  MilliSecondsInSecond
	/// \brief  The number of milliseconds in a second
	static time_t const MilliSecondsInSecond = 1000;

	/// \def  MicroSecondsInMilliSecond
	/// \brief  The number of microseconds in a millisecond
	static time_t const MicroSecondsInMilliSecond = 1000;

	/// \def  NanoSecondsInMicroSecond
	/// \brief  The number of nanoseconds in a microsecond
	static time_t const NanoSecondsInMicroSecond = 1000;

	/// \def  NanoSecondsInMilliSecond
	/// \brief  The number of nanoseconds in a millisecond
	static time_t const NanoSecondsInMilliSecond = 1000000;

	/// \def  MicroSecsInSec
	/// \brief  The number of microseconds in a second
	static time_t const MicroSecsInSec = (MilliSecondsInSecond * MicroSecondsInMilliSecond);

	/// \def  NanoSecsInSec
	/// \brief  The number of nanoseconds in a second
	static time_t const NanoSecsInSec = (1000 * MicroSecsInSec);

	/// \def  ReferenceTime2000InSec
	/// \brief  The number of seconds elapsed between Epoch(Thu Jan 01 1970 00:00:00) and 01/01/2000 (Sat Jan 01 2000 00:00:00)
	static time_t const ReferenceTime2000InSec = 946684800;

public:
	static u64 GetSynchTime();

};

} //namespace univubi
